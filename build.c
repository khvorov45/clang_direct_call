#include "cbuild.h"

#define function static
#define global_variable static

typedef int32_t  i32;
typedef uint64_t u64;

typedef struct PathLastModValue {
    u64      thisFileLastMod;
    prb_Str* directIncludes;
    bool     haveLastModWithDeps;
    bool     beingResolved;
    u64      lastModWithDeps;
} PathLastModValue;

typedef struct FileLastMod {
    char*            key;
    PathLastModValue value;
} FileLastMod;

global_variable prb_Str globalLLVMRootDir;
global_variable prb_Str globalClangSrcDir;
global_variable prb_Str globalBuildDir;
global_variable prb_Str* globalAllFilesInSrc;
global_variable FileLastMod* globalLastModTable;

function prb_Str
replaceSeps(prb_Arena* arena, prb_Str str) {
    prb_Str newname = prb_fmt(arena, "%.*s", prb_LIT(str));
    for (i32 ind = 0; ind < newname.len; ind++) {
        if (prb_charIsSep(newname.ptr[ind])) {
            ((char*)newname.ptr)[ind] = '_';
        }
    }
    return newname;
}

function bool
isSrcFile(prb_Str path) {
    bool result = prb_strEndsWith(path, prb_STR(".cpp")) || prb_strEndsWith(path, prb_STR(".c"));
    return result;
}

function prb_Str
readFile(prb_Arena* arena, prb_Str path) {
    prb_ReadEntireFileResult readRes = prb_readEntireFile(arena, path);
    if (!readRes.success) {
        prb_writelnToStdout(arena, path);
        prb_assert(!"couldn't read the file");
    }
    prb_Str str = prb_strFromBytes(readRes.content);
    return str;
}

function void
resolveEntry(FileLastMod* entry) {
    prb_assert(!entry->value.beingResolved);
    if (!entry->value.haveLastModWithDeps) {
        entry->value.beingResolved = true;
        prb_Str* directIncludes = entry->value.directIncludes;
        i32      directIncludesCount = arrlen(directIncludes);
        for (i32 includeIndex = 0; includeIndex < directIncludesCount; includeIndex++) {
            prb_Str includeName = directIncludes[includeIndex];
            i32     includeIndex = shgeti(globalLastModTable, includeName.ptr);
            if (includeIndex != -1) {
                FileLastMod* includeEntry = globalLastModTable + includeIndex;
                if (!includeEntry->value.beingResolved) {
                    resolveEntry(includeEntry);
                    entry->value.lastModWithDeps = prb_max(entry->value.lastModWithDeps, includeEntry->value.lastModWithDeps);
                }
            }
        }
        entry->value.beingResolved = false;
        entry->value.haveLastModWithDeps = true;
    }
}

function void
resolveFileLastMod(prb_Arena* arena) {
    prb_TempMemory temp = prb_beginTempMemory(arena);
    prb_Arena      includesArena = prb_createArenaFromArena(arena, 10 * prb_MEGABYTE);

    i32 pathCount = arrlen(globalAllFilesInSrc);
    for (i32 pathIndex = 0; pathIndex < pathCount; pathIndex++) {
        prb_TempMemory tempPath = prb_beginTempMemory(arena);

        prb_Str           path = globalAllFilesInSrc[pathIndex];
        prb_FileTimestamp pathLastMod = prb_getLastModified(arena, path);
        prb_assert(pathLastMod.valid);
        PathLastModValue val = {.thisFileLastMod = pathLastMod.timestamp, .lastModWithDeps = pathLastMod.timestamp};

        prb_Str        content = readFile(arena, path);
        prb_StrScanner scanner = prb_createStrScanner(content);
        while (prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_STR("#include ")}, prb_StrScannerSide_AfterMatch)) {
            prb_assert(scanner.afterMatch.len > 0);
            char quoteCh1 = scanner.afterMatch.ptr[0];
            if (quoteCh1 == '"' || quoteCh1 == '<') {
                char quoteCh2 = '"';
                if (quoteCh1 == '<') {
                    quoteCh2 = '>';
                }

                prb_assert(prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_fmt(arena, "%c", quoteCh1)}, prb_StrScannerSide_AfterMatch));
                prb_assert(prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_fmt(arena, "%c", quoteCh2)}, prb_StrScannerSide_AfterMatch));

                prb_Str includedFile = prb_fmt(&includesArena, "%.*s", prb_LIT(scanner.betweenLastMatches));
                arrput(val.directIncludes, includedFile);
            }
        }

        prb_Str name = prb_getLastEntryInPath(path);
        shput(globalLastModTable, name.ptr, val);

        prb_endTempMemory(tempPath);
    }

    for (i32 pathIndex = 0; pathIndex < pathCount; pathIndex++) {
        resolveEntry(globalLastModTable + pathIndex);
    }

    prb_endTempMemory(temp);
    prb_arenaChangeUsed(arena, includesArena.used);
}

typedef struct CompileObjsResult {
    prb_Str objs;
    bool    anyRecompiled;
} CompileObjsResult;

function CompileObjsResult
compileObjs(prb_Arena* arena, prb_Str outdir, prb_Str* srcFiles, i32 srcFileCount) {
    prb_TempMemory temp = prb_beginTempMemory(arena);
    prb_Arena      objListArena = prb_createArenaFromArena(arena, 1 * prb_MEGABYTE);
    prb_GrowingStr objListBuilder = prb_beginStr(&objListArena);

    prb_Process* objProcs = 0;
    for (i32 srcIndex = 0; srcIndex < srcFileCount; srcIndex++) {
        prb_Str srcpath = srcFiles[srcIndex];
        prb_assert(isSrcFile(srcpath));
        prb_Str filename = prb_getLastEntryInPath(srcpath);
        prb_Str outname = prb_replaceExt(arena, filename, prb_STR("obj"));
        prb_Str out = prb_pathJoin(arena, outdir, outname);
        prb_addStrSegment(&objListBuilder, " %.*s", prb_LIT(out));

        // NOTE(khvorov) Recompile if src or any of its includes are newer than out (or if out does not exist)
        bool              shouldRecompile = true;
        prb_FileTimestamp outLastMod = prb_getLastModified(arena, out);
        if (outLastMod.valid) {
            i32 srcEntryIndex = shgeti(globalLastModTable, filename.ptr);
            prb_assert(srcEntryIndex != -1);
            FileLastMod* srcLastModEntry = globalLastModTable + srcEntryIndex;
            prb_assert(srcLastModEntry->value.haveLastModWithDeps);
            shouldRecompile = srcLastModEntry->value.lastModWithDeps > outLastMod.timestamp;
        }

        if (shouldRecompile) {
            prb_Str defines = prb_STR(
                "-DLLVM_ON_UNIX -DPACKAGE_NAME=\"LLVM\" -DPACKAGE_VERSION=\"420.69\" "
                "-DHAVE_FCNTL_H=1 -DHAVE_UNISTD_H=1 -DLLVM_ENABLE_THREADS=1 -DHAVE_SYSEXITS_H=1 "
                "-DHAVE_SYS_STAT_H=1 -DLLVM_WINDOWS_PREFER_FORWARD_SLASH=1 -DHAVE_SYS_MMAN_H=1 "
                "-DHAVE_FUTIMENS=1 -DLLVM_ENABLE_CRASH_DUMPS=1 -DHAVE_GETRUSAGE=1 -DHAVE_SYS_RESOURCE_H=1 "
                "-DHAVE_GETPAGESIZE=1 -DHAVE_MALLINFO2=1 -DHAVE_PTHREAD_H -DHAVE_ERRNO_H -DHAVE_STRERROR_R "
                "-DBUG_REPORT_URL=\"hawtdawgadverntures.xyz\" -DCLANG_SPAWN_CC1=0 "
                "-DCLANG_INSTALL_LIBDIR_BASENAME=\"\" -DENABLE_X86_RELAX_RELOCATIONS=1 -DDEFAULT_SYSROOT=\"\" "
                "-DCLANG_RESOURCE_DIR=\"\" -DPPC_LINUX_DEFAULT_IEEELONGDOUBLE=0 -DCLANG_DEFAULT_OPENMP_RUNTIME=\"libomp\" "
                "-DCLANG_DEFAULT_LINKER=\"\" -DLLVM_HOST_TRIPLE=\"x86_64-unknown-linux-gnu\" -DCLANG_DEFAULT_RTLIB=\"\" "
                "-DCLANG_DEFAULT_UNWINDLIB=\"\" -DCLANG_DEFAULT_CXX_STDLIB=\"\" -DLLVM_DEFAULT_TARGET_TRIPLE=\"x86_64-unknown-linux-gnu\" "
                "-DC_INCLUDE_DIRS=\"\" -DCLANG_DEFAULT_PIE_ON_LINUX=1 -DCLANG_DEFAULT_OBJCOPY=\"objcopy\" -DGCC_INSTALL_PREFIX=\"\" "
                "-DLLVM_VERSION_STRING=\"420.69\" -DCLANG_OPENMP_NVPTX_DEFAULT_ARCH=\"sm_35\" "
                "-DCLANG_SYSTEMZ_DEFAULT_ARCH=\"z10\" -DLLVM_ENABLE_ABI_BREAKING_CHECKS=1 -DLLVM_VERSION_MAJOR=69 "
                "-DLLVM_VERSION_MINOR=420 -DLLVM_VERSION_PATCH=1337 "
                "-DBLAKE3_NO_AVX512=1 -DBLAKE3_NO_AVX2 -DBLAKE3_NO_SSE41 -DBLAKE3_NO_SSE2"
            );
            prb_Str flags = prb_STR("-std=c++17");
            if (prb_strEndsWith(srcpath, prb_STR(".c"))) {
                flags = prb_STR("");
            }
            prb_Str cmd = prb_fmt(arena, "clang -g %.*s %.*s -Werror -Wfatal-errors -c %.*s -o %.*s", prb_LIT(defines), prb_LIT(flags), prb_LIT(srcpath), prb_LIT(out));
            prb_writelnToStdout(arena, cmd);
            prb_Process proc = prb_createProcess(cmd, (prb_ProcessSpec) {});
            arrput(objProcs, proc);
        }
    }
    prb_Str objList = prb_endStr(&objListBuilder);

    i32                 totalObjCount = arrlen(objProcs);
    prb_CoreCountResult chunkSize = prb_getCoreCount(arena);
    prb_assert(chunkSize.success);
    i32 chunkCount = (totalObjCount / chunkSize.cores) + 1;
    i32 procsDoneCount = 0;
    for (i32 chunkIndex = 0; chunkIndex < chunkCount; chunkIndex++) {
        prb_Process* thisProcSet = objProcs + procsDoneCount;
        i32          procsLeft = totalObjCount - procsDoneCount;
        i32          thisChunkSize = prb_min(procsLeft, chunkSize.cores);
        prb_assert(prb_launchProcesses(arena, thisProcSet, thisChunkSize, prb_Background_Yes));
        prb_assert(prb_waitForProcesses(thisProcSet, thisChunkSize));
        procsDoneCount += thisChunkSize;
    }
    prb_assert(procsDoneCount == totalObjCount);

    bool              anyRecompiled = totalObjCount > 0;
    CompileObjsResult result = {objList, anyRecompiled};

    prb_endTempMemory(temp);
    prb_arenaChangeUsed(arena, objList.len + 1);
    return result;
}

function void
execCmd(prb_Arena* arena, prb_Str cmd) {
    prb_writelnToStdout(arena, cmd);
    prb_Process proc = prb_createProcess(cmd, (prb_ProcessSpec) {});
    prb_assert(prb_launchProcesses(arena, &proc, 1, prb_Background_No));
}

function CompileObjsResult
compileObjsThatStartWith(prb_Arena* arena, prb_Str startsWith) {
    prb_Str outdir = prb_pathJoin(arena, globalBuildDir, startsWith);

    prb_Str* files = 0;
    for (i32 srcIndex = 0; srcIndex < arrlen(globalAllFilesInSrc); srcIndex++) {
        prb_Str path = globalAllFilesInSrc[srcIndex];
        prb_Str name = prb_getLastEntryInPath(path);
        if (isSrcFile(name) && prb_strStartsWith(name, startsWith)) {
            arrput(files, path);
        }
    }

    prb_assert(arrlen(files) > 0);

    CompileObjsResult objResult = compileObjs(arena, outdir, files, arrlen(files));
    return objResult;
}

typedef struct CompileStaticLibResult {
    prb_Str outfile;
    bool    recompiled;
} CompileStaticLibResult;

function CompileStaticLibResult
compileStaticLib(prb_Arena* arena, prb_Str startsWith) {
    prb_Str        outname = prb_fmt(arena, "%.*s.lib", prb_LIT(startsWith));
    prb_Str        outfile = prb_pathJoin(arena, globalBuildDir, outname);
    prb_TempMemory temp = prb_beginTempMemory(arena);

    CompileObjsResult objResult = compileObjsThatStartWith(arena, startsWith);
    bool              recompile = objResult.anyRecompiled || !prb_isFile(arena, outfile);
    if (recompile) {
        prb_assert(prb_removePathIfExists(arena, outfile));
        prb_Str libCmd = prb_fmt(arena, "ar rcs %.*s %.*s", prb_LIT(outfile), prb_LIT(objResult.objs));
        execCmd(arena, libCmd);
    } else {
        prb_writeToStdout(prb_fmt(arena, "skip %.*s\n", prb_LIT(outname)));
    }

    prb_endTempMemory(temp);
    CompileStaticLibResult result = {outfile, recompile};
    return result;
}

function prb_Str
compileExe(prb_Arena* arena, prb_Str startsWith, CompileStaticLibResult* deps, i32 depsCount, prb_Str outname) {
    prb_Str        outnameWithExt = prb_fmt(arena, "%.*s.exe", prb_LIT(outname));
    prb_Str        outfile = prb_pathJoin(arena, globalBuildDir, outnameWithExt);
    prb_TempMemory temp = prb_beginTempMemory(arena);

    prb_GrowingStr depsBuilder = prb_beginStr(arena);
    bool           anyDepRecompiled = false;
    for (i32 depIndex = 0; depIndex < depsCount; depIndex++) {
        CompileStaticLibResult dep = deps[depIndex];
        if (dep.recompiled) {
            anyDepRecompiled = true;
        }
        prb_addStrSegment(&depsBuilder, " %.*s", prb_LIT(dep.outfile));
    }
    prb_Str depsStr = prb_endStr(&depsBuilder);

    CompileObjsResult objResult = compileObjsThatStartWith(arena, startsWith);
    if (anyDepRecompiled || objResult.anyRecompiled || !prb_isFile(arena, outfile)) {
        prb_assert(prb_removePathIfExists(arena, outfile));
        prb_Str libCmd = prb_fmt(arena, "clang -o %.*s %.*s %.*s -lstdc++ -lm", prb_LIT(outfile), prb_LIT(objResult.objs), prb_LIT(depsStr));
        execCmd(arena, libCmd);
    } else {
        prb_writeToStdout(prb_fmt(arena, "skip %.*s\n", prb_LIT(outnameWithExt)));
    }

    prb_endTempMemory(temp);
    return outfile;
}

typedef struct RunTableGenSpec {
    prb_Str tableGenExe;
    prb_Str in;
    prb_Str out;
    prb_Str includes;
    prb_Str args;
} RunTableGenSpec;

function void
runTableGen(prb_Arena* arena, void* data) {
    RunTableGenSpec* spec = (RunTableGenSpec*)data;

    prb_Str tableGenExe = spec->tableGenExe;
    prb_Str in = spec->in;
    prb_Str out = spec->out;
    prb_Str includes = spec->includes;
    prb_Str args = spec->args;

    prb_TempMemory temp = prb_beginTempMemory(arena);

    prb_Str includePaths = {};
    {
        prb_GrowingStr includePathsBuilder = prb_beginStr(arena);
        prb_StrScanner scanner = prb_createStrScanner(includes);
        while (prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_STR(" "), .alwaysMatchEnd = true}, prb_StrScannerSide_AfterMatch)) {
            prb_addStrSegment(&includePathsBuilder, " -I%.*s/%.*s", prb_LIT(globalLLVMRootDir), prb_LIT(scanner.betweenLastMatches));
        }
        includePaths = prb_endStr(&includePathsBuilder);
    }

    prb_Str inpath = prb_pathJoin(arena, globalLLVMRootDir, in);
    prb_Str outpath = prb_pathJoin(arena, globalClangSrcDir, out);

    if (!prb_isFile(arena, outpath)) {
        prb_Str cmd = prb_fmt(
            arena,
            "%.*s %.*s %.*s %.*s -o %.*s",
            prb_LIT(tableGenExe),
            prb_LIT(args),
            prb_LIT(includePaths),
            prb_LIT(inpath),
            prb_LIT(outpath)
        );

        execCmd(arena, cmd);

        // NOTE(khvorov) Flatten includes in the output
        prb_ReadEntireFileResult outread = prb_readEntireFile(arena, outpath);
        prb_assert(outread.success);
        prb_Str outstr = prb_strFromBytes(outread.content);

        prb_Arena      flatoutArena = prb_createArenaFromArena(arena, prb_arenaFreeSize(arena) - 100 * prb_KILOBYTE);
        prb_GrowingStr flatoutBuilder = prb_beginStr(&flatoutArena);
        prb_StrScanner scanner = prb_createStrScanner(outstr);
        while (prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_STR("#include \"")}, prb_StrScannerSide_AfterMatch)) {
            prb_addStrSegment(&flatoutBuilder, "%.*s", prb_LIT(scanner.betweenLastMatches));
            prb_assert(prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_STR("\"")}, prb_StrScannerSide_AfterMatch));
            prb_Str includedFile = scanner.betweenLastMatches;
            prb_Str pathFromRoot = {};
            if (prb_strStartsWith(includedFile, prb_STR("clang"))) {
                pathFromRoot = prb_pathJoin(arena, prb_STR("clang/include"), includedFile);
            } else {
                prb_assert(prb_strStartsWith(includedFile, prb_STR("llvm")));
                pathFromRoot = prb_pathJoin(arena, prb_STR("llvm/include"), includedFile);
            }
            prb_Str flatpath = replaceSeps(arena, pathFromRoot);
            prb_addStrSegment(&flatoutBuilder, "#include \"%.*s\"", prb_LIT(flatpath));
        }
        prb_addStrSegment(&flatoutBuilder, "%.*s", prb_LIT(scanner.afterMatch));

        prb_Str flatout = prb_endStr(&flatoutBuilder);
        prb_assert(prb_writeEntireFile(arena, outpath, flatout.ptr, flatout.len));
    }

    prb_endTempMemory(temp);
}

function void
writeTargetDef(prb_Arena* arena, prb_Str in, prb_Str out) {
    prb_TempMemory temp = prb_beginTempMemory(arena);

    prb_Str                  inpath = prb_pathJoin(arena, globalLLVMRootDir, in);
    prb_ReadEntireFileResult inread = prb_readEntireFile(arena, inpath);
    prb_assert(inread.success);
    prb_Str instr = prb_strFromBytes(inread.content);

    prb_Str macro = {};
    {
        prb_StrScanner scanner = prb_createStrScanner(instr);
        prb_assert(prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_STR("#ifndef")}, prb_StrScannerSide_AfterMatch));
        prb_assert(prb_strScannerMove(&scanner, (prb_StrFindSpec) {.mode = prb_StrFindMode_LineBreak}, prb_StrScannerSide_AfterMatch));
        macro = prb_strTrim(scanner.betweenLastMatches);
    }

    prb_StrFindSpec at = {.pattern = prb_STR("@")};

    prb_GrowingStr    outBuilder = prb_beginStr(arena);
    prb_StrFindResult placeholder = prb_strFind(instr, at);
    prb_assert(placeholder.found);
    prb_addStrSegment(&outBuilder, "%.*s", prb_LIT(placeholder.beforeMatch));
    prb_addStrSegment(&outBuilder, "%.*s(X86)", prb_LIT(macro));
    placeholder = prb_strFind(placeholder.afterMatch, at);
    prb_assert(placeholder.found);
    prb_addStrSegment(&outBuilder, "%.*s", prb_LIT(placeholder.afterMatch));
    prb_Str outstr = prb_endStr(&outBuilder);
    prb_Str outpath = prb_pathJoin(arena, globalClangSrcDir, out);
    prb_assert(prb_writeEntireFile(arena, outpath, outstr.ptr, outstr.len));
    prb_endTempMemory(temp);
}

typedef struct TableGenArgs {
    prb_Str exe;
    char*   in;
    char*   out;
    char*   include;
    char*   args;
} TableGenArgs;

int
main() {
    prb_TimeStart scriptStart = prb_timeStart();

    prb_Arena  arena_ = prb_createArenaFromVmem(4ll * prb_GIGABYTE);
    prb_Arena* arena = &arena_;

    {
        prb_Str rootdir = prb_getParentDir(arena, prb_STR(__FILE__));
        globalLLVMRootDir = prb_pathJoin(arena, rootdir, prb_STR("llvm-project"));
        globalClangSrcDir = prb_pathJoin(arena, rootdir, prb_STR("clang_src"));
        globalBuildDir = prb_pathJoin(arena, rootdir, prb_STR("build"));
    }

    globalAllFilesInSrc = prb_getAllDirEntries(arena, globalClangSrcDir, prb_Recursive_No);

    globalLastModTable = 0;
    resolveFileLastMod(arena);

    prb_createDirIfNotExists(arena, globalBuildDir);
    prb_createDirIfNotExists(arena, globalClangSrcDir);

    if (false) {
        writeTargetDef(
            arena,
            prb_STR("llvm/include/llvm/Config/Targets.def.in"),
            prb_STR("llvm_include_llvm_Config_Targets.def")
        );

        writeTargetDef(
            arena,
            prb_STR("llvm/include/llvm/Config/AsmPrinters.def.in"),
            prb_STR("llvm_include_llvm_Config_AsmPrinters.def")
        );

        writeTargetDef(
            arena,
            prb_STR("llvm/include/llvm/Config/AsmParsers.def.in"),
            prb_STR("llvm_include_llvm_Config_AsmParsers.def")
        );

        writeTargetDef(
            arena,
            prb_STR("llvm/include/llvm/Config/Disassemblers.def.in"),
            prb_STR("llvm_include_llvm_Config_Disassemblers.def")
        );

        writeTargetDef(
            arena,
            prb_STR("llvm/include/llvm/Config/TargetMCAs.def.in"),
            prb_STR("llvm_include_llvm_Config_TargetMCAs.def")
        );
    }

    // NOTE(khvorov) Static libs we need for tablegen
    CompileStaticLibResult supportLibFile = compileStaticLib(arena, prb_STR("llvm_lib_Support"));
    CompileStaticLibResult clangSupportLibFile = compileStaticLib(arena, prb_STR("clang_lib_Support"));
    CompileStaticLibResult tableGenLibFile = compileStaticLib(arena, prb_STR("llvm_lib_TableGen"));

    CompileStaticLibResult depsOfTablegen[] = {tableGenLibFile, clangSupportLibFile, supportLibFile};

    // NOTE(khvorov) Compile just the table gen
    prb_Str llvmTableGenExe = compileExe(arena, prb_STR("llvm_utils_TableGen"), depsOfTablegen, prb_arrayCount(depsOfTablegen), prb_STR("llvmTableGen"));
    prb_Str clangTableGenExe = compileExe(arena, prb_STR("clang_utils_TableGen"), depsOfTablegen, prb_arrayCount(depsOfTablegen), prb_STR("clangTableGen"));

    // NOTE(khvorov) Generate the files we need table gen for
    TableGenArgs tableGenArgs[] = {
        {llvmTableGenExe, "clang/include/clang/Driver/Options.td", "clang_include_clang_Driver_Options.inc", "llvm/include", "-gen-opt-parser-defs"},
        {llvmTableGenExe, "llvm/include/llvm/Frontend/OpenMP/OMP.td", "llvm_include_llvm_Frontend_OpenMP_OMP.h.inc", "llvm/include", "--gen-directive-decl"},
        {llvmTableGenExe, "llvm/include/llvm/Frontend/OpenMP/OMP.td", "llvm_include_llvm_Frontend_OpenMP_OMP.inc", "llvm/include", "--gen-directive-impl"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Attributes.td", "llvm_include_llvm_IR_Attributes.inc", "llvm/include", "-gen-attrs"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicEnums.inc", "llvm/include", "-gen-intrinsic-enums"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsAArch64.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=aarch64"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsARM.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=arm"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsX86.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=x86"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsWebAssembly.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=wasm"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsAMDGPU.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=amdgcn"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsBPF.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=bpf"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsDirectX.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=dx"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsHexagon.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=hexagon"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsMips.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=mips"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsNVPTX.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=nvvm"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsPowerPC.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=ppc"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsR600.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=r600"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsRISCV.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=riscv"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsS390.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=s390"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsVE.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=ve"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsXCore.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=xcore"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicImpl.inc", "llvm/include", "-gen-intrinsic-impl"},
        {llvmTableGenExe, "llvm/include/llvm/IR/Intrinsics.td", "llvm_include_llvm_IR_IntrinsicsLoongArch.h", "llvm/include", "-gen-intrinsic-enums -intrinsic-prefix=loongarch"},
        {llvmTableGenExe, "llvm/lib/Target/X86/X86.td", "X86GenRegisterInfo.inc", "llvm/lib/Target/X86 llvm/include", "-gen-register-info"},
        {llvmTableGenExe, "llvm/lib/Target/X86/X86.td", "X86GenInstrInfo.inc", "llvm/lib/Target/X86 llvm/include", "-gen-instr-info -instr-info-expand-mi-operand-info=0"},
        {llvmTableGenExe, "llvm/lib/Target/X86/X86.td", "X86GenSubtargetInfo.inc", "llvm/lib/Target/X86 llvm/include", "-gen-subtarget"},
        {llvmTableGenExe, "llvm/lib/Target/X86/X86.td", "X86GenMnemonicTables.inc", "llvm/lib/Target/X86 llvm/include", "-gen-x86-mnemonic-tables -asmwriternum=1"},
        {llvmTableGenExe, "llvm/lib/Target/X86/X86.td", "X86GenDAGISel.inc", "llvm/lib/Target/X86 llvm/include", "-gen-dag-isel"},
        {llvmTableGenExe, "llvm/lib/Target/X86/X86.td", "X86GenRegisterBank.inc", "llvm/lib/Target/X86 llvm/include", "-gen-register-bank"},
        {llvmTableGenExe, "llvm/lib/Target/X86/X86.td", "X86GenFastISel.inc", "llvm/lib/Target/X86 llvm/include", "-gen-fast-isel"},
        {llvmTableGenExe, "llvm/lib/Target/X86/X86.td", "X86GenEVEX2VEXTables.inc", "llvm/lib/Target/X86 llvm/include", "-gen-x86-EVEX2VEX-tables"},
        {llvmTableGenExe, "llvm/lib/Target/X86/X86.td", "X86GenCallingConv.inc", "llvm/lib/Target/X86 llvm/include", "-gen-callingconv"},
        {llvmTableGenExe, "llvm/lib/Target/X86/X86.td", "X86GenGlobalISel.inc", "llvm/lib/Target/X86 llvm/include", "-gen-global-isel"},
        {llvmTableGenExe, "llvm/lib/Target/X86/X86.td", "X86GenAsmWriter.inc", "llvm/lib/Target/X86 llvm/include", "-gen-asm-writer"},
        {llvmTableGenExe, "llvm/lib/Target/X86/X86.td", "X86GenAsmWriter1.inc", "llvm/lib/Target/X86 llvm/include", "-gen-asm-writer -asmwriternum=1"},
        {llvmTableGenExe, "llvm/lib/Target/X86/X86.td", "X86GenAsmMatcher.inc", "llvm/lib/Target/X86 llvm/include", "-gen-asm-matcher"},
        {clangTableGenExe, "clang/include/clang/Basic/Diagnostic.td", "clang_include_clang_Basic_DiagnosticCommonKinds.inc", "clang/include/clang/Basic", "-gen-clang-diags-defs -clang-component=Common"},
        {clangTableGenExe, "clang/include/clang/Basic/Diagnostic.td", "clang_include_clang_Basic_DiagnosticDriverKinds.inc", "clang/include/clang/Basic", "-gen-clang-diags-defs -clang-component=Driver"},
        {clangTableGenExe, "clang/include/clang/Basic/Diagnostic.td", "clang_include_clang_Basic_DiagnosticFrontendKinds.inc", "clang/include/clang/Basic", "-gen-clang-diags-defs -clang-component=Frontend"},
        {clangTableGenExe, "clang/include/clang/Basic/Diagnostic.td", "clang_include_clang_Basic_DiagnosticLexKinds.inc", "clang/include/clang/Basic", "-gen-clang-diags-defs -clang-component=Lex"},
        {clangTableGenExe, "clang/include/clang/Basic/Diagnostic.td", "clang_include_clang_Basic_DiagnosticASTKinds.inc", "clang/include/clang/Basic", "-gen-clang-diags-defs -clang-component=AST"},
        {clangTableGenExe, "clang/include/clang/Basic/Diagnostic.td", "clang_include_clang_Basic_DiagnosticAnalysisKinds.inc", "clang/include/clang/Basic", "-gen-clang-diags-defs -clang-component=Analysis"},
        {clangTableGenExe, "clang/include/clang/Basic/Diagnostic.td", "clang_include_clang_Basic_DiagnosticCommentKinds.inc", "clang/include/clang/Basic", "-gen-clang-diags-defs -clang-component=Comment"},
        {clangTableGenExe, "clang/include/clang/Basic/Diagnostic.td", "clang_include_clang_Basic_DiagnosticCrossTUKinds.inc", "clang/include/clang/Basic", "-gen-clang-diags-defs -clang-component=CrossTU"},
        {clangTableGenExe, "clang/include/clang/Basic/Diagnostic.td", "clang_include_clang_Basic_DiagnosticParseKinds.inc", "clang/include/clang/Basic", "-gen-clang-diags-defs -clang-component=Parse"},
        {clangTableGenExe, "clang/include/clang/Basic/Diagnostic.td", "clang_include_clang_Basic_DiagnosticSemaKinds.inc", "clang/include/clang/Basic", "-gen-clang-diags-defs -clang-component=Sema"},
        {clangTableGenExe, "clang/include/clang/Basic/Diagnostic.td", "clang_include_clang_Basic_DiagnosticSerializationKinds.inc", "clang/include/clang/Basic", "-gen-clang-diags-defs -clang-component=Serialization"},
        {clangTableGenExe, "clang/include/clang/Basic/Diagnostic.td", "clang_include_clang_Basic_DiagnosticRefactoringKinds.inc", "clang/include/clang/Basic", "-gen-clang-diags-defs -clang-component=Refactoring"},
        {clangTableGenExe, "clang/include/clang/Basic/Diagnostic.td", "clang_include_clang_Basic_DiagnosticGroups.inc", "clang/include/clang/Basic", "-gen-clang-diag-groups"},
        {clangTableGenExe, "clang/include/clang/Basic/StmtNodes.td", "clang_include_clang_AST_StmtNodes.inc", "clang/include", "-gen-clang-stmt-nodes"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_Basic_AttrList.inc", "clang/include", "-gen-clang-attr-list"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_Sema_AttrParsedAttrList.inc", "clang/include", "-gen-clang-attr-parsed-attr-list"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_Basic_AttrSubMatchRulesList.inc", "clang/include", "-gen-clang-attr-subject-match-rule-list"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_Basic_AttrHasAttributeImpl.inc", "clang/include", "-gen-clang-attr-has-attribute-impl"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_Sema_AttrParsedAttrKinds.inc", "clang/include", "-gen-clang-attr-parsed-attr-kinds"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_Sema_AttrSpellingListIndex.inc", "clang/include", "-gen-clang-attr-spelling-index"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_AST_AttrVisitor.inc", "clang/include", "-gen-clang-attr-ast-visitor"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_AST_Attrs.inc", "clang/include", "-gen-clang-attr-classes"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_Serialization_AttrPCHRead.inc", "clang/include", "-gen-clang-attr-pch-read"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_Serialization_AttrPCHWrite.inc", "clang/include", "-gen-clang-attr-pch-write"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_lib_AST_AttrDocTable.inc", "clang/include", "-gen-clang-attr-doc-table"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_AST_AttrNodeTraverse.inc", "clang/include", "-gen-clang-attr-node-traverse"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_AST_AttrTextNodeDump.inc", "clang/include", "-gen-clang-attr-text-node-dump"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_AST_AttrImpl.inc", "clang/include", "-gen-clang-attr-impl"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_Sema_AttrParsedAttrImpl.inc", "clang/include", "-gen-clang-attr-parsed-attr-impl"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_Sema_AttrTemplateInstantiate.inc", "clang/include", "-gen-clang-attr-template-instantiate"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_Parse_AttrParserStringSwitches.inc", "clang/include", "-gen-clang-attr-parser-string-switches"},
        {clangTableGenExe, "clang/include/clang/Basic/Attr.td", "clang_include_clang_Parse_AttrSubMatchRulesParserStringSwitches.inc", "clang/include", "-gen-clang-attr-subject-match-rules-parser-string-switches"},
        {clangTableGenExe, "clang/include/clang/Basic/TypeNodes.td", "clang_include_clang_AST_TypeNodes.inc", "clang/include", "-gen-clang-type-nodes"},
        {clangTableGenExe, "clang/include/clang/Basic/DeclNodes.td", "clang_include_clang_AST_DeclNodes.inc", "clang/include", "-gen-clang-decl-nodes"},
        {clangTableGenExe, "clang/include/clang/Basic/CommentNodes.td", "clang_include_clang_AST_CommentNodes.inc", "clang/include", "-gen-clang-comment-nodes"},
        {clangTableGenExe, "clang/include/clang/Basic/riscv_vector.td", "clang_include_clang_Basic_riscv_vector_builtin_sema.inc", "clang/include", "-gen-riscv-vector-builtin-sema"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_neon.td", "clang_include_clang_Basic_arm_neon.inc", "clang/include/clang/Basic", "-gen-arm-neon-sema"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_neon.td", "clang_include_clang_Basic_arm_neon.h", "clang/include/clang/Basic", "-gen-arm-neon"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_fp16.td", "clang_include_clang_Basic_arm_fp16.inc", "clang/include/clang/Basic", "-gen-arm-neon-sema"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_mve.td", "clang_include_clang_Basic_arm_mve_builtins.inc", "clang/include/clang/Basic", "-gen-arm-mve-builtin-def"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_mve.td", "clang_include_clang_Basic_arm_mve_builtin_sema.inc", "clang/include/clang/Basic", "-gen-arm-mve-builtin-sema"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_mve.td", "clang_include_clang_Basic_arm_mve_builtin_aliases.inc", "clang/include/clang/Basic", "-gen-arm-mve-builtin-aliases"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_mve.td", "clang_include_clang_Basic_arm_mve_builtin_cg.inc", "clang/include/clang/Basic", "-gen-arm-mve-builtin-codegen"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_cde.td", "clang_include_clang_Basic_arm_cde_builtins.inc", "clang/include/clang/Basic", "-gen-arm-cde-builtin-def"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_cde.td", "clang_include_clang_Basic_arm_cde_builtin_sema.inc", "clang/include/clang/Basic", "-gen-arm-cde-builtin-sema"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_cde.td", "clang_include_clang_Basic_arm_cde_builtin_aliases.inc", "clang/include/clang/Basic", "-gen-arm-cde-builtin-aliases"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_cde.td", "clang_include_clang_Basic_arm_cde_builtin_cg.inc", "clang/include/clang/Basic", "-gen-arm-cde-builtin-codegen"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_sve.td", "clang_include_clang_Basic_arm_sve_builtins.inc", "clang/include/clang/Basic", "-gen-arm-sve-builtins"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_sve.td", "clang_include_clang_Basic_arm_sve_builtin_cg.inc", "clang/include/clang/Basic", "-gen-arm-sve-builtin-codegen"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_sve.td", "clang_include_clang_Basic_arm_sve_sema_rangechecks.inc", "clang/include/clang/Basic", "-gen-arm-sve-sema-rangechecks"},
        {clangTableGenExe, "clang/include/clang/Basic/riscv_vector.td", "clang_include_clang_Basic_riscv_vector_builtins.inc", "clang/include/clang/Basic", "-gen-riscv-vector-builtins"},
        {clangTableGenExe, "clang/include/clang/Basic/riscv_vector.td", "clang_include_clang_Basic_riscv_vector_builtin_cg.inc", "clang/include/clang/Basic", "-gen-riscv-vector-builtin-codegen"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_sve.td", "clang_include_clang_Basic_arm_sve_typeflags.inc", "clang/include/clang/Basic", "-gen-arm-sve-typeflags"},
        {clangTableGenExe, "clang/include/clang/AST/PropertiesBase.td", "clang_include_clang_AST_AbstractBasicWriter.inc", "clang/include", "-gen-clang-basic-writer"},
        {clangTableGenExe, "clang/include/clang/AST/PropertiesBase.td", "clang_include_clang_AST_AbstractBasicReader.inc", "clang/include", "-gen-clang-basic-reader"},
        {clangTableGenExe, "clang/include/clang/AST/TypeProperties.td", "clang_include_clang_AST_AbstractTypeWriter.inc", "clang/include", "-gen-clang-type-writer"},
        {clangTableGenExe, "clang/include/clang/AST/TypeProperties.td", "clang_include_clang_AST_AbstractTypeReader.inc", "clang/include", "-gen-clang-type-reader"},
        {clangTableGenExe, "clang/include/clang/AST/CommentHTMLNamedCharacterReferences.td", "clang_include_clang_AST_CommentHTMLNamedCharacterReferences.inc", "clang/include", "-gen-clang-comment-html-named-character-references"},
        {clangTableGenExe, "clang/include/clang/AST/CommentCommands.td", "clang_include_clang_AST_CommentCommandInfo.inc", "clang/include", "-gen-clang-comment-command-info"},
        {clangTableGenExe, "clang/include/clang/AST/CommentHTMLTags.td", "clang_include_clang_AST_CommentHTMLTags.inc", "clang/include", "-gen-clang-comment-html-tags"},
        {clangTableGenExe, "clang/include/clang/AST/CommentHTMLTags.td", "clang_include_clang_AST_CommentHTMLTagsProperties.inc", "clang/include", "-gen-clang-comment-html-tags-properties"},
        {clangTableGenExe, "clang/include/clang/AST/CommentCommands.td", "clang_include_clang_AST_CommentCommandList.inc", "clang/include", "-gen-clang-comment-command-list"},
        {clangTableGenExe, "clang/include/clang/AST/StmtDataCollectors.td", "clang_include_clang_AST_StmtDataCollectors.inc", "clang/include", "-gen-clang-data-collectors"},
        {clangTableGenExe, "clang/include/clang/StaticAnalyzer/Checkers/Checkers.td", "clang_include_clang_StaticAnalyzer_Checkers_Checkers.inc", "clang/include/clang/StaticAnalyzer/Checkers", "-gen-clang-sa-checkers"},
        {clangTableGenExe, "clang/lib/Sema/OpenCLBuiltins.td", "clang_lib_Sema_OpenCLBuiltins.inc", "clang/include", "-gen-clang-opencl-builtins"},
        {clangTableGenExe, "clang/lib/AST/Interp/Opcodes.td", "clang_lib_AST_Interp_Opcodes.inc", "clang/include", "-gen-clang-opcodes"},
    };

    {
        prb_TempMemory   temp = prb_beginTempMemory(arena);
        RunTableGenSpec* tableGenSpecs = prb_arenaAllocArray(arena, RunTableGenSpec, prb_arrayCount(tableGenArgs));
        prb_Job*         jobs = 0;
        for (i32 ind = 0; ind < prb_arrayCount(tableGenArgs); ind++) {
            TableGenArgs args = tableGenArgs[ind];
            tableGenSpecs[ind] = (RunTableGenSpec) {args.exe, prb_STR(args.in), prb_STR(args.out), prb_STR(args.include), prb_STR(args.args)};
            intptr_t memsize = 20 * prb_MEGABYTE;
            if (prb_strStartsWith(prb_STR(args.out), prb_STR("X86Gen"))) {
                memsize *= 4;
            }
            prb_Job job = prb_createJob(runTableGen, tableGenSpecs + ind, arena, memsize);
            arrput(jobs, job);
        }
        prb_assert(prb_launchJobs(jobs, arrlen(jobs), prb_Background_Yes));
        prb_assert(prb_waitForJobs(jobs, arrlen(jobs)));
        prb_endTempMemory(temp);
    }

    // NOTE(khvorov) Compile the actual compiler
    CompileStaticLibResult deps[] = {
        compileStaticLib(arena, prb_STR("clang_lib_FrontendTool")),
        compileStaticLib(arena, prb_STR("clang_lib_ExtractAPI")),
        compileStaticLib(arena, prb_STR("clang_lib_Index")),
        compileStaticLib(arena, prb_STR("clang_lib_CodeGen")),
        compileStaticLib(arena, prb_STR("llvm_lib_LTO")),
        compileStaticLib(arena, prb_STR("llvm_lib_Passes")),
        compileStaticLib(arena, prb_STR("clang_lib_Frontend")),
        compileStaticLib(arena, prb_STR("clang_lib_Rewrite")),
        compileStaticLib(arena, prb_STR("clang_lib_Parse")),
        compileStaticLib(arena, prb_STR("clang_lib_Serialization")),
        compileStaticLib(arena, prb_STR("clang_lib_Sema")),
        compileStaticLib(arena, prb_STR("clang_lib_Analysis")),
        compileStaticLib(arena, prb_STR("clang_lib_ASTMatchers")),
        clangSupportLibFile,
        compileStaticLib(arena, prb_STR("clang_lib_AST")),
        compileStaticLib(arena, prb_STR("llvm_lib_Frontend")),
        compileStaticLib(arena, prb_STR("clang_lib_Edit")),
        compileStaticLib(arena, prb_STR("clang_lib_Lex")),
        compileStaticLib(arena, prb_STR("llvm_lib_Target")),
        compileStaticLib(arena, prb_STR("llvm_lib_CodeGen")),
        compileStaticLib(arena, prb_STR("llvm_lib_Transforms")),
        compileStaticLib(arena, prb_STR("llvm_lib_Linker")),
        compileStaticLib(arena, prb_STR("llvm_lib_Analysis")),
        compileStaticLib(arena, prb_STR("clang_lib_Driver")),
        compileStaticLib(arena, prb_STR("llvm_lib_WindowsDriver")),
        compileStaticLib(arena, prb_STR("clang_lib_Basic")),
        compileStaticLib(arena, prb_STR("llvm_lib_TargetParser")),
        compileStaticLib(arena, prb_STR("llvm_lib_Option")),
        compileStaticLib(arena, prb_STR("llvm_lib_ProfileData")),
        compileStaticLib(arena, prb_STR("llvm_lib_DebugInfo")),
        compileStaticLib(arena, prb_STR("llvm_lib_Object")),
        compileStaticLib(arena, prb_STR("llvm_lib_MC")),
        compileStaticLib(arena, prb_STR("llvm_lib_BinaryFormat")),
        compileStaticLib(arena, prb_STR("llvm_lib_TextAPI")),
        compileStaticLib(arena, prb_STR("llvm_lib_Bitcode")),
        compileStaticLib(arena, prb_STR("llvm_lib_IRReader")),
        compileStaticLib(arena, prb_STR("llvm_lib_IR")),
        compileStaticLib(arena, prb_STR("llvm_lib_Remarks")),
        compileStaticLib(arena, prb_STR("llvm_lib_Bitstream")),
        compileStaticLib(arena, prb_STR("llvm_lib_AsmParser")),
        supportLibFile,
        compileStaticLib(arena, prb_STR("llvm_lib_Demangle")),
    };

    compileExe(arena, prb_STR("clang_tools_driver"), deps, prb_arrayCount(deps), prb_STR("clang"));

    prb_writeToStdout(prb_fmt(arena, "total: %.2fms\n", prb_getMsFrom(scriptStart)));
}
