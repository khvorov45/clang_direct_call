#include "cbuild.h"

#define function static

typedef int32_t i32;

typedef struct NewpathOgpath {
    char*   key;
    prb_Str value;
} NewpathOgpath;

function bool
notIn(prb_Str val, prb_Str* arr, i32 arrc) {
    bool result = true;
    for (i32 ind = 0; ind < arrc; ind++) {
        if (prb_streq(val, arr[ind])) {
            result = false;
            break;
        }
    }
    return result;
}

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
compileObjs(prb_Arena* arena, prb_Str outdir, prb_Str* srcFiles, i32 srcFileCount) {
    prb_Arena      objListArena = prb_createArenaFromArena(arena, 1 * prb_MEGABYTE);
    prb_GrowingStr objListBuilder = prb_beginStr(&objListArena);

    prb_Process* objProcs = 0;
    for (i32 srcIndex = 0; srcIndex < srcFileCount; srcIndex++) {
        prb_Str path = srcFiles[srcIndex];
        prb_assert(isSrcFile(path));
        prb_Str filename = prb_getLastEntryInPath(path);
        prb_Str outname = prb_replaceExt(arena, filename, prb_STR("obj"));
        prb_Str out = prb_pathJoin(arena, outdir, outname);
        prb_addStrSegment(&objListBuilder, " %.*s", prb_LIT(out));
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
            "-DLLVM_VERSION_MINOR=420 -DLLVM_VERSION_PATCH=1337"
        );
        prb_Str flags = prb_STR("-std=c++17");
        if (prb_strEndsWith(path, prb_STR(".c"))) {
            flags = prb_STR("");
        }
        prb_Str cmd = prb_fmt(arena, "clang -g %.*s %.*s -Werror -Wfatal-errors -c %.*s -o %.*s", prb_LIT(defines), prb_LIT(flags), prb_LIT(path), prb_LIT(out));
        prb_writelnToStdout(arena, cmd);
        prb_Process proc = prb_createProcess(cmd, (prb_ProcessSpec) {});
        arrput(objProcs, proc);
    }
    prb_Str objList = prb_endStr(&objListBuilder);

    i32 totalObjCount = arrlen(objProcs);
    prb_CoreCountResult chunkSize = prb_getCoreCount(arena);
    prb_assert(chunkSize.success);
    i32 chunkCount = (totalObjCount / chunkSize.cores) + 1;
    i32 procsDoneCount = 0;
    for (i32 chunkIndex = 0; chunkIndex < chunkCount; chunkIndex++) {
        prb_Process* thisProcSet = objProcs + procsDoneCount;
        i32 procsLeft = totalObjCount - procsDoneCount;
        i32 thisChunkSize = prb_min(procsLeft, chunkSize.cores);
        prb_assert(prb_launchProcesses(arena, thisProcSet, thisChunkSize, prb_Background_Yes));
        prb_assert(prb_waitForProcesses(thisProcSet, thisChunkSize));
        procsDoneCount += thisChunkSize;
    }
    prb_assert(procsDoneCount == totalObjCount);

    return objList;
}

function void
execCmd(prb_Arena* arena, prb_Str cmd) {
    prb_writelnToStdout(arena, cmd);
    prb_Process proc = prb_createProcess(cmd, (prb_ProcessSpec) {});
    prb_assert(prb_launchProcesses(arena, &proc, 1, prb_Background_No));
}

function void
addAllSrcFiles(prb_Arena* arena, prb_Str** storage, prb_Str dir) {
    prb_Str* allEntries = prb_getAllDirEntries(arena, dir, prb_Recursive_No);
    bool     atleastone = false;
    for (i32 ind = 0; ind < arrlen(allEntries); ind++) {
        prb_Str entry = allEntries[ind];
        if (isSrcFile(entry)) {
            arrput(*storage, entry);
            atleastone = true;
        }
    }

    if (!atleastone) {
        prb_writeToStdout(prb_fmt(arena, "0 src files: %.*s\n", prb_LIT(dir)));
        prb_assert(!"no files");
    }
}

function prb_Str
compileObjsThatStartWith(prb_Arena* arena, prb_Str builddir, prb_Str* allFilesInSrc, prb_Str startsWith) {
    prb_Str outdir = prb_pathJoin(arena, builddir, startsWith);
    prb_assert(prb_clearDir(arena, outdir));

    prb_Str* files = 0;
    for (i32 srcIndex = 0; srcIndex < arrlen(allFilesInSrc); srcIndex++) {
        prb_Str path = allFilesInSrc[srcIndex];
        prb_Str name = prb_getLastEntryInPath(path);
        if (isSrcFile(name) && prb_strStartsWith(name, startsWith)) {
            arrput(files, path);
        }
    }

    prb_assert(arrlen(files) > 0);

    prb_Str objs = compileObjs(arena, outdir, files, arrlen(files));
    return objs;
}

typedef enum Skip {
    Skip_No,
    Skip_Yes,
} Skip;

function prb_Str
compileStaticLib(Skip skip, prb_Arena* arena, prb_Str builddir, prb_Str* allFilesInSrc, prb_Str startsWith) {
    prb_Str outfile = prb_pathJoin(arena, builddir, prb_fmt(arena, "%.*s.lib", prb_LIT(startsWith)));

    if (skip == Skip_No || !prb_isFile(arena, outfile)) {
        prb_assert(prb_removePathIfExists(arena, outfile));
        prb_TempMemory temp = prb_beginTempMemory(arena);
        prb_Str        objs = compileObjsThatStartWith(arena, builddir, allFilesInSrc, startsWith);
        prb_Str        libCmd = prb_fmt(arena, "ar rcs %.*s %.*s", prb_LIT(outfile), prb_LIT(objs));
        execCmd(arena, libCmd);
        prb_endTempMemory(temp);
    }

    return outfile;
}

function prb_Str
compileExe(Skip skip, prb_Arena* arena, prb_Str builddir, prb_Str* allFilesInSrc, prb_Str startsWith, prb_Str deps) {
    prb_Str outfile = prb_pathJoin(arena, builddir, prb_fmt(arena, "%.*s.exe", prb_LIT(startsWith)));

    if (skip == Skip_No || !prb_isFile(arena, outfile)) {
        prb_assert(prb_removePathIfExists(arena, outfile));
        prb_TempMemory temp = prb_beginTempMemory(arena);
        prb_Str        objs = compileObjsThatStartWith(arena, builddir, allFilesInSrc, startsWith);
        prb_Str        libCmd = prb_fmt(arena, "clang -o %.*s %.*s %.*s -lstdc++ -lm", prb_LIT(outfile), prb_LIT(objs), prb_LIT(deps));
        execCmd(arena, libCmd);
        prb_endTempMemory(temp);
    }

    return outfile;
}

typedef struct RunTableGenSpec {
    prb_Str tableGenExe;
    prb_Str llvmRootDir;
    prb_Str clangdcdir;
    prb_Str in;
    prb_Str out;
    prb_Str includes;
    prb_Str args;
} RunTableGenSpec;

function void
runTableGen(prb_Arena* arena, void* data) {
    RunTableGenSpec* spec = (RunTableGenSpec*)data;

    prb_Str tableGenExe = spec->tableGenExe;
    prb_Str llvmRootDir = spec->llvmRootDir;
    prb_Str clangdcdir = spec->clangdcdir;
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
            prb_addStrSegment(&includePathsBuilder, " -I%.*s/%.*s", prb_LIT(llvmRootDir), prb_LIT(scanner.betweenLastMatches));
        }
        includePaths = prb_endStr(&includePathsBuilder);
    }

    prb_Str inpath = prb_pathJoin(arena, llvmRootDir, in);
    prb_Str outpath = prb_pathJoin(arena, clangdcdir, out);

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
writeTargetDef(prb_Arena* arena, prb_Str llvmRootDir, prb_Str clangdcdir, prb_Str in, prb_Str out) {
    prb_TempMemory temp = prb_beginTempMemory(arena);

    prb_Str                  inpath = prb_pathJoin(arena, llvmRootDir, in);
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
    prb_Str outpath = prb_pathJoin(arena, clangdcdir, out);
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
    prb_Arena  permArena_ = prb_createArenaFromArena(&arena_, 100 * prb_MEGABYTE);
    prb_Arena* permArena = &permArena_;
    prb_Arena* tempArena = &arena_;

    prb_Str rootdir = prb_getParentDir(permArena, prb_STR(__FILE__));
    prb_Str llvmRootDir = prb_pathJoin(permArena, rootdir, prb_STR("llvm-project"));
    prb_Str clangdcdir = prb_pathJoin(permArena, rootdir, prb_STR("clang_src"));
    prb_Str builddir = prb_pathJoin(permArena, rootdir, prb_STR("build"));

    prb_createDirIfNotExists(tempArena, builddir);
    prb_createDirIfNotExists(tempArena, clangdcdir);

    prb_Str* clangRelevantFiles = 0;
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("clang/tools/driver/driver.cpp")));

    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Support")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/TableGen")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Option")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/MC")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/MC/MCParser")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/TargetParser")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/ProfileData")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Demangle")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/BinaryFormat")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Object")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/TextAPI")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Remarks")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/IR")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/IRReader")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/AsmParser")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/DebugInfo/DWARF")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Bitstream/Reader")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Bitcode/Reader")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/WindowsDriver")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Target")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Target/X86")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Target/X86/TargetInfo")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Target/X86/MCTargetDesc")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Analysis")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/CodeGen")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/CodeGen/LiveDebugValues")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/CodeGen/SelectionDAG")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/CodeGen/GlobalISel")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Transforms/Utils")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Transforms/ObjCARC")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/utils/TableGen")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/utils/TableGen/GlobalISel")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("clang/lib/Support")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("clang/lib/Driver")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("clang/lib/Driver/ToolChains")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("clang/lib/Driver/ToolChains/Arch")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("clang/lib/Basic")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("clang/utils/TableGen")));

    NewpathOgpath* newpaths = 0;

    // NOTE(khvorov) Pull the relevant files
    for (i32 clangSrcFileIndex = 0; clangSrcFileIndex < arrlen(clangRelevantFiles); clangSrcFileIndex++) {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        ogpath = clangRelevantFiles[clangSrcFileIndex];

        prb_Str newpath = {};
        {
            prb_StrFindResult res = prb_strFind(ogpath, (prb_StrFindSpec) {.pattern = prb_STR("/llvm-project/")});
            prb_assert(res.found);
            prb_Str newname = replaceSeps(tempArena, res.afterMatch);
            newpath = prb_pathJoin(permArena, clangdcdir, newname);
        }

        {
            prb_Str generatedFiles[] = {
                prb_STR("clang/Config/config.h"),
                prb_STR("clang/Driver/Options.inc"),
                prb_STR("llvm/Config/llvm-config.h"),
                prb_STR("llvm/Config/config.h"),
                prb_STR("llvm/Config/Targets.def"),
                prb_STR("llvm/Config/AsmPrinters.def"),
                prb_STR("llvm/Config/AsmParsers.def"),
                prb_STR("llvm/Config/Disassemblers.def"),
                prb_STR("llvm/Config/TargetMCAs.def"),
                prb_STR("llvm/Config/abi-breaking.h"),
                prb_STR("clang/Basic/DiagnosticDriverKinds.inc"),
                prb_STR("clang/StaticAnalyzer/Checkers/Checkers.inc"),
                prb_STR("clang/Basic/DiagnosticCommonKinds.inc"),
                prb_STR("clang/AST/StmtNodes.inc"),
                prb_STR("clang/AST/DeclNodes.inc"),
                prb_STR("clang/AST/TypeNodes.inc"),
                prb_STR("clang/Basic/AttrList.inc"),
                prb_STR("llvm/Frontend/OpenMP/OMP.inc"),
                prb_STR("clang/include/clang/AST/CommentCommandList.inc"),
                prb_STR("clang/AST/CommentCommandList.inc"),
                prb_STR("clang/Basic/Version.inc"),
                prb_STR("clang/Basic/DiagnosticFrontendKinds.inc"),
                prb_STR("clang/Basic/DiagnosticSerializationKinds.inc"),
                prb_STR("clang/Basic/DiagnosticLexKinds.inc"),
                prb_STR("clang/Basic/DiagnosticParseKinds.inc"),
                prb_STR("clang/Basic/DiagnosticASTKinds.inc"),
                prb_STR("clang/Basic/DiagnosticCommentKinds.inc"),
                prb_STR("clang/Basic/DiagnosticCrossTUKinds.inc"),
                prb_STR("clang/Basic/DiagnosticSemaKinds.inc"),
                prb_STR("clang/Basic/DiagnosticAnalysisKinds.inc"),
                prb_STR("clang/Basic/DiagnosticRefactoringKinds.inc"),
                prb_STR("clang/Basic/DiagnosticGroups.inc"),
                prb_STR("clang/Basic/AttrHasAttributeImpl.inc"),
                prb_STR("clang/Basic/AttrSubMatchRulesList.inc"),
                prb_STR("clang/Sema/AttrParsedAttrKinds.inc"),
                prb_STR("clang/Sema/AttrParsedAttrList.inc"),
                prb_STR("clang/Sema/AttrSpellingListIndex.inc"),
                prb_STR("VCSVersion.inc"),
                prb_STR("clang/Basic/arm_sve_typeflags.inc"),
                prb_STR("clang/Basic/arm_neon.inc"),
                prb_STR("clang/Basic/arm_fp16.inc"),
                prb_STR("clang/Basic/arm_mve_builtins.inc"),
                prb_STR("clang/Basic/arm_cde_builtins.inc"),
                prb_STR("clang/Basic/arm_sve_builtins.inc"),
                prb_STR("clang/Basic/riscv_vector_builtins.inc"),
                prb_STR("llvm/Frontend/OpenMP/OMP.h.inc"),
                prb_STR("llvm/IR/Attributes.inc"),
                prb_STR("llvm/Support/VCSRevision.h"),
                prb_STR("llvm/IR/IntrinsicsAArch64.h"),
                prb_STR("llvm/IR/IntrinsicsARM.h"),
                prb_STR("llvm/IR/IntrinsicsX86.h"),
                prb_STR("llvm/IR/IntrinsicsAMDGPU.h"),
                prb_STR("llvm/IR/IntrinsicsBPF.h"),
                prb_STR("llvm/IR/IntrinsicsDirectX.h"),
                prb_STR("llvm/IR/IntrinsicsHexagon.h"),
                prb_STR("llvm/IR/IntrinsicsMips.h"),
                prb_STR("llvm/IR/IntrinsicsNVPTX.h"),
                prb_STR("llvm/IR/IntrinsicsPowerPC.h"),
                prb_STR("llvm/IR/IntrinsicsR600.h"),
                prb_STR("llvm/IR/IntrinsicsRISCV.h"),
                prb_STR("llvm/IR/IntrinsicsS390.h"),
                prb_STR("llvm/IR/IntrinsicsVE.h"),
                prb_STR("llvm/IR/IntrinsicsWebAssembly.h"),
                prb_STR("llvm/IR/IntrinsicsXCore.h"),
                prb_STR("llvm/IR/IntrinsicImpl.inc"),
                prb_STR("llvm/IR/IntrinsicEnums.inc"),
                prb_STR("X86GenRegisterBank.inc"),
                prb_STR("X86GenDAGISel.inc"),
                prb_STR("X86GenCallingConv.inc"),
                prb_STR("X86GenGlobalISel.inc"),
                prb_STR("X86GenEVEX2VEXTables.inc"),
                prb_STR("X86GenInstrInfo.inc"),
                prb_STR("X86GenRegisterInfo.inc"),
                prb_STR("X86GenFastISel.inc"),
                prb_STR("X86GenSubtargetInfo.inc"),
                prb_STR("X86GenMnemonicTables.inc"),
            };

            i32 newpathIndex = shgeti(newpaths, newpath.ptr);
            if (newpathIndex == -1) {
                shput(newpaths, newpath.ptr, ogpath);
                prb_TempMemory temp = prb_beginTempMemory(tempArena);
                prb_Str        ogpathDir = prb_getParentDir(tempArena, ogpath);

                // NOTE(khvorov) Scan for includes
                prb_ReadEntireFileResult clangSrcFileReadRes = prb_readEntireFile(tempArena, ogpath);
                if (!clangSrcFileReadRes.success) {
                    prb_writelnToStdout(tempArena, ogpath);
                    prb_assert(!"couldn't read the file");
                }
                prb_Str        clangSrcFileStr = prb_strFromBytes(clangSrcFileReadRes.content);
                prb_StrScanner scanner = prb_createStrScanner(clangSrcFileStr);
                prb_Arena      newContentArena = prb_createArenaFromArena(tempArena, clangSrcFileStr.len * 2);
                prb_GrowingStr newContentBuilder = prb_beginStr(&newContentArena);
                while (prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_STR("#include ")}, prb_StrScannerSide_AfterMatch)) {
                    prb_addStrSegment(&newContentBuilder, "%.*s", prb_LIT(scanner.betweenLastMatches));
                    prb_assert(scanner.afterMatch.len > 0);
                    char quoteCh1 = scanner.afterMatch.ptr[0];
                    if (quoteCh1 == '"' || quoteCh1 == '<') {
                        char quoteCh2 = '"';
                        if (quoteCh1 == '<') {
                            quoteCh2 = '>';
                        }

                        prb_assert(prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_fmt(tempArena, "%c", quoteCh1)}, prb_StrScannerSide_AfterMatch));
                        prb_assert(prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_fmt(tempArena, "%c", quoteCh2)}, prb_StrScannerSide_AfterMatch));

                        prb_Str includedFile = scanner.betweenLastMatches;
                        bool    isX86Inc = prb_strStartsWith(includedFile, prb_STR("X86Gen")) && prb_strEndsWith(includedFile, prb_STR(".inc"));

                        bool ignore = prb_strStartsWith(includedFile, prb_STR("google"))
                            || prb_strStartsWith(includedFile, prb_STR("tensorflow"))
                            || prb_streq(includedFile, prb_STR("InlinerSizeModel.h"))
                            || prb_streq(includedFile, prb_STR("RegallocEvictModel.h"));

                        prb_Str relevantPath = includedFile;
                        bool    pathNotModified = false;
                        if (prb_strStartsWith(includedFile, prb_STR("clang"))) {
                            relevantPath = prb_pathJoin(tempArena, prb_STR("clang/include"), includedFile);
                        } else if (prb_strStartsWith(includedFile, prb_STR("llvm"))) {
                            relevantPath = prb_pathJoin(tempArena, prb_STR("llvm/include"), includedFile);
                        } else if (prb_strStartsWith(includedFile, prb_STR("ToolChains/"))) {
                            relevantPath = prb_pathJoin(tempArena, prb_STR("clang/lib/Driver"), includedFile);
                        } else if (prb_streq(includedFile, prb_STR("Targets.h"))) {
                            relevantPath = prb_STR("clang/lib/Basic/Targets.h");
                        } else if (prb_strStartsWith(includedFile, prb_STR("MCTargetDesc/X86"))) {
                            relevantPath = prb_pathJoin(tempArena, prb_STR("llvm/lib/Target/X86"), includedFile);
                        } else if (prb_strEndsWith(includedFile, prb_STR("X86TargetInfo.h"))) {
                            relevantPath = prb_STR("llvm/lib/Target/X86/TargetInfo/X86TargetInfo.h");
                        } else if (prb_streq(includedFile, prb_STR("X86InstrInfo.h"))) {
                            relevantPath = prb_STR("llvm/lib/Target/X86/X86InstrInfo.h");
                        } else if (quoteCh1 == '"' && !prb_streq(relevantPath, prb_STR("x.h")) && !isX86Inc && !ignore) {
                            prb_StrFindResult includeFindRes = prb_strFind(ogpathDir, (prb_StrFindSpec) {.pattern = prb_STR("/llvm-project/")});
                            prb_assert(includeFindRes.found);
                            relevantPath = prb_pathJoin(tempArena, includeFindRes.afterMatch, includedFile);
                        } else {
                            pathNotModified = true;
                        }

                        prb_Str includedFileFlattened = replaceSeps(tempArena, relevantPath);
                        if (pathNotModified) {
                            prb_addStrSegment(&newContentBuilder, "#include %c%.*s%c", quoteCh1, prb_LIT(includedFile), quoteCh2);
                        } else {
                            prb_addStrSegment(&newContentBuilder, "#include \"%.*s\"", prb_LIT(includedFileFlattened));
                            if (notIn(includedFile, generatedFiles, prb_arrayCount(generatedFiles))) {
                                arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, relevantPath));
                            }
                        }

                    } else {
                        prb_addStrSegment(&newContentBuilder, "%.*s", prb_LIT(scanner.match));
                    }
                }
                prb_addStrSegment(&newContentBuilder, "%.*s", prb_LIT(scanner.afterMatch));
                prb_Str newFileContent = prb_endStr(&newContentBuilder);

                // NOTE(khvorov) Write out new content to the new location
                prb_assert(prb_writeEntireFile(tempArena, newpath, newFileContent.ptr, newFileContent.len));

                prb_endTempMemory(temp);
            } else {
                NewpathOgpath prevPath = newpaths[newpathIndex];
                prb_assert(prb_streq(prevPath.value, ogpath));
            }
        }

        prb_endTempMemory(temp);
    }

    // TODO(khvorov) Generate the files we don't need table gen for
    {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        outpath = prb_pathJoin(tempArena, clangdcdir, prb_STR("clang_include_clang_Config_config.h"));
        prb_Str        content = prb_STR("");
        prb_assert(prb_writeEntireFile(tempArena, outpath, content.ptr, content.len));
        prb_endTempMemory(temp);
    }

    {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        outpath = prb_pathJoin(tempArena, clangdcdir, prb_STR("llvm_include_llvm_Config_llvm-config.h"));
        prb_Str        content = prb_STR("");
        prb_assert(prb_writeEntireFile(tempArena, outpath, content.ptr, content.len));
        prb_endTempMemory(temp);
    }

    {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        outpath = prb_pathJoin(tempArena, clangdcdir, prb_STR("llvm_include_llvm_Config_config.h"));
        prb_Str        content = prb_STR("");
        prb_assert(prb_writeEntireFile(tempArena, outpath, content.ptr, content.len));
        prb_endTempMemory(temp);
    }

    {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        outpath = prb_pathJoin(tempArena, clangdcdir, prb_STR("llvm_include_llvm_Config_abi-breaking.h"));
        prb_Str        content = prb_STR("");
        prb_assert(prb_writeEntireFile(tempArena, outpath, content.ptr, content.len));
        prb_endTempMemory(temp);
    }

    {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        outpath = prb_pathJoin(tempArena, clangdcdir, prb_STR("clang_include_clang_Basic_Version.inc"));

        prb_Str content = prb_STR(
            "#define CLANG_VERSION 69.0.0\n"
            "#define CLANG_VERSION_STRING \"69.0.0\"\n"
            "#define CLANG_VERSION_MAJOR 69\n"
            "#define CLANG_VERSION_MAJOR_STRING \"69\"\n"
            "#define CLANG_VERSION_MINOR 0\n"
            "#define CLANG_VERSION_PATCHLEVEL 0\n"
        );
        prb_assert(prb_writeEntireFile(tempArena, outpath, content.ptr, content.len));
        prb_endTempMemory(temp);
    }

    {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        outpath = prb_pathJoin(tempArena, clangdcdir, prb_STR("clang_lib_Basic_VCSVersion.inc"));

        prb_Str content = prb_STR(
            "#define LLVM_REVISION \"\"\n"
            "#define LLVM_REPOSITORY \"\"\n"
            "#define CLANG_REVISION \"\"\n"
            "#define CLANG_REPOSITORY \"\"\n"
        );
        prb_assert(prb_writeEntireFile(tempArena, outpath, content.ptr, content.len));
        prb_endTempMemory(temp);
    }

    {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        outpath = prb_pathJoin(tempArena, clangdcdir, prb_STR("llvm_include_llvm_Support_VCSRevision.h"));

        prb_Str content = prb_STR(
            "#define LLVM_REVISION \"\"\n"
            "#define LLVM_REPOSITORY \"https://github.com/llvm/llvm-project\"\n"
        );
        prb_assert(prb_writeEntireFile(tempArena, outpath, content.ptr, content.len));
        prb_endTempMemory(temp);
    }

    writeTargetDef(
        tempArena,
        llvmRootDir,
        clangdcdir,
        prb_STR("llvm/include/llvm/Config/Targets.def.in"),
        prb_STR("llvm_include_llvm_Config_Targets.def")
    );

    writeTargetDef(
        tempArena,
        llvmRootDir,
        clangdcdir,
        prb_STR("llvm/include/llvm/Config/AsmPrinters.def.in"),
        prb_STR("llvm_include_llvm_Config_AsmPrinters.def")
    );

    writeTargetDef(
        tempArena,
        llvmRootDir,
        clangdcdir,
        prb_STR("llvm/include/llvm/Config/AsmParsers.def.in"),
        prb_STR("llvm_include_llvm_Config_AsmParsers.def")
    );

    writeTargetDef(
        tempArena,
        llvmRootDir,
        clangdcdir,
        prb_STR("llvm/include/llvm/Config/Disassemblers.def.in"),
        prb_STR("llvm_include_llvm_Config_Disassemblers.def")
    );

    writeTargetDef(
        tempArena,
        llvmRootDir,
        clangdcdir,
        prb_STR("llvm/include/llvm/Config/TargetMCAs.def.in"),
        prb_STR("llvm_include_llvm_Config_TargetMCAs.def")
    );

    prb_Str* allFilesInSrc = prb_getAllDirEntries(permArena, clangdcdir, prb_Recursive_No);

    // NOTE(khvorov) Static libs we need for tablegen
    prb_Str supportLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_Support"));
    prb_Str clangSupportLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("clang_lib_Support"));
    prb_Str tableGenLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_TableGen"));

    prb_Str depsOfTablegen = prb_fmt(permArena, "%.*s %.*s %.*s", prb_LIT(tableGenLibFile), prb_LIT(clangSupportLibFile), prb_LIT(supportLibFile));

    // NOTE(khvorov) Compile just the table gen
    prb_Str llvmTableGenExe = compileExe(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_utils_TableGen"), depsOfTablegen);
    prb_Str clangTableGenExe = compileExe(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("clang_utils_TableGen"), depsOfTablegen);

    // TODO(khvorov) Generate the files we need table gen for
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
        {clangTableGenExe, "clang/include/clang/Basic/TypeNodes.td", "clang_include_clang_AST_TypeNodes.inc", "clang/include", "-gen-clang-type-nodes"},
        {clangTableGenExe, "clang/include/clang/Basic/DeclNodes.td", "clang_include_clang_AST_DeclNodes.inc", "clang/include", "-gen-clang-decl-nodes"},
        {clangTableGenExe, "clang/include/clang/AST/CommentCommands.td", "clang_include_clang_AST_CommentCommandList.inc", "clang/include", "-gen-clang-comment-command-list"},
        {clangTableGenExe, "clang/include/clang/StaticAnalyzer/Checkers/Checkers.td", "clang_include_clang_StaticAnalyzer_Checkers_Checkers.inc", "clang/include/clang/StaticAnalyzer/Checkers", "-gen-clang-sa-checkers"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_neon.td", "clang_include_clang_Basic_arm_neon.inc", "clang/include/clang/Basic", "-gen-arm-neon-sema"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_neon.td", "clang_include_clang_Basic_arm_neon.h", "clang/include/clang/Basic", "-gen-arm-neon"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_fp16.td", "clang_include_clang_Basic_arm_fp16.inc", "clang/include/clang/Basic", "-gen-arm-neon-sema"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_mve.td", "clang_include_clang_Basic_arm_mve_builtins.inc", "clang/include/clang/Basic", "-gen-arm-mve-builtin-def"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_cde.td", "clang_include_clang_Basic_arm_cde_builtins.inc", "clang/include/clang/Basic", "-gen-arm-cde-builtin-def"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_sve.td", "clang_include_clang_Basic_arm_sve_builtins.inc", "clang/include/clang/Basic", "-gen-arm-sve-builtins"},
        {clangTableGenExe, "clang/include/clang/Basic/riscv_vector.td", "clang_include_clang_Basic_riscv_vector_builtins.inc", "clang/include/clang/Basic", "-gen-riscv-vector-builtins"},
        {clangTableGenExe, "clang/include/clang/Basic/arm_sve.td", "clang_include_clang_Basic_arm_sve_typeflags.inc", "clang/include/clang/Basic", "-gen-arm-sve-typeflags"},
    };

    if (true) {
        prb_TempMemory   temp = prb_beginTempMemory(tempArena);
        RunTableGenSpec* tableGenSpecs = prb_arenaAllocArray(tempArena, RunTableGenSpec, prb_arrayCount(tableGenArgs));
        prb_Job*         jobs = 0;
        for (i32 ind = 0; ind < prb_arrayCount(tableGenArgs); ind++) {
            TableGenArgs args = tableGenArgs[ind];
            tableGenSpecs[ind] = (RunTableGenSpec) {args.exe, llvmRootDir, clangdcdir, prb_STR(args.in), prb_STR(args.out), prb_STR(args.include), prb_STR(args.args)};
            intptr_t memsize = 20 * prb_MEGABYTE;
            if (prb_strStartsWith(prb_STR(args.out), prb_STR("X86Gen"))) {
                memsize *= 4;
            }
            prb_Job job = prb_createJob(runTableGen, tableGenSpecs + ind, tempArena, memsize);
            arrput(jobs, job);
        }
        prb_assert(prb_launchJobs(jobs, arrlen(jobs), prb_Background_Yes));
        prb_assert(prb_waitForJobs(jobs, arrlen(jobs)));
        prb_endTempMemory(temp);
    }

    // TODO(khvorov) Compile the actual compiler
    prb_Str optionLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_Option"));
    prb_Str targetParserLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_TargetParser"));
    prb_Str mcLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_MC"));
    prb_Str profileDataLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_ProfileData"));
    prb_Str demangleLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_Demangle"));
    prb_Str debugInfoLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_DebugInfo"));
    prb_Str objectLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_Object"));
    prb_Str textAPILibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_TextAPI"));
    prb_Str binaryFormatLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_BinaryFormat"));
    prb_Str irLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_IR"));
    prb_Str remarksLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_Remarks"));
    prb_Str bitstreamLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_Bitstream"));
    prb_Str bitcodeLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_Bitcode"));
    prb_Str irreaderLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_IRReader"));
    prb_Str asmParserLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_AsmParser"));
    prb_Str windowsDriverLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_WindowsDriver"));
    prb_Str targetLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_Target"));
    prb_Str analysisLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_Analysis"));
    prb_Str codeGenLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_CodeGen"));
    prb_Str transformsLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_Transforms"));
    prb_Str clangDriverLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("clang_lib_Driver"));
    prb_Str clangBasicLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("clang_lib_Basic"));

    {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);

        prb_Str srcFiles[] = {
            prb_pathJoin(tempArena, clangdcdir, prb_STR("clang_tools_driver_driver.cpp")),
        };

        prb_Str outdir = prb_pathJoin(tempArena, builddir, prb_STR("clang"));
        prb_assert(prb_clearDir(tempArena, outdir));
        prb_Str objs = compileObjs(tempArena, outdir, srcFiles, prb_arrayCount(srcFiles));

        prb_Str deps[] = {
            targetLibFile,
            codeGenLibFile,
            transformsLibFile,
            analysisLibFile,
            clangDriverLibFile,
            windowsDriverLibFile,
            clangBasicLibFile,
            targetParserLibFile,
            optionLibFile,
            profileDataLibFile,
            debugInfoLibFile,
            objectLibFile,
            mcLibFile,
            binaryFormatLibFile,
            textAPILibFile,
            bitcodeLibFile,
            irreaderLibFile,
            irLibFile,
            remarksLibFile,
            bitstreamLibFile,
            asmParserLibFile,
            supportLibFile,
            demangleLibFile,
        };

        prb_Str depsStr = prb_stringsJoin(tempArena, deps, prb_arrayCount(deps), prb_STR(" "));
        prb_Str out = prb_pathJoin(tempArena, builddir, prb_STR("clang.exe"));
        prb_Str linkCmd = prb_fmt(permArena, "clang -o %.*s %.*s %.*s -lstdc++ -lm", prb_LIT(out), prb_LIT(objs), prb_LIT(depsStr));
        execCmd(tempArena, linkCmd);

        prb_endTempMemory(temp);
    }

    prb_writeToStdout(prb_fmt(tempArena, "total: %.2fms\n", prb_getMsFrom(scriptStart)));
}
