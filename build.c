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
            "-DCLANG_DEFAULT_UNWINDLIB=\"\" -DCLANG_DEFAULT_CXX_STDLIB=\"\""
        );
        prb_Str flags = prb_STR("-std=c++17");
        if (prb_strEndsWith(path, prb_STR(".c"))) {
            flags = prb_STR("");
        }
        prb_Str cmd = prb_fmt(arena, "clang -g %.*s %.*s -Werror -Wfatal-errors -c %.*s -o %.*s", prb_LIT(defines), prb_LIT(flags), prb_LIT(path), prb_LIT(out));
        prb_writelnToStdout(arena, cmd);
        prb_Process proc = prb_createProcess(cmd, (prb_ProcessSpec) {});
        prb_assert(prb_launchProcesses(arena, &proc, 1, prb_Background_Yes));
        arrput(objProcs, proc);
    }
    prb_assert(prb_waitForProcesses(objProcs, arrlen(objProcs)));

    prb_Str objList = prb_endStr(&objListBuilder);
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
    prb_assert(arrlen(allEntries) > 0);
    for (i32 ind = 0; ind < arrlen(allEntries); ind++) {
        prb_Str entry = allEntries[ind];
        if (isSrcFile(entry)) {
            arrput(*storage, entry);
        }
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

    if (skip == Skip_No) {
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

    if (skip == Skip_No) {
        prb_assert(prb_removePathIfExists(arena, outfile));
        prb_TempMemory temp = prb_beginTempMemory(arena);
        prb_Str        objs = compileObjsThatStartWith(arena, builddir, allFilesInSrc, startsWith);
        prb_Str        libCmd = prb_fmt(arena, "clang -o %.*s %.*s %.*s -lstdc++ -lm", prb_LIT(outfile), prb_LIT(objs), prb_LIT(deps));
        execCmd(arena, libCmd);
        prb_endTempMemory(temp);
    }

    return outfile;
}

function void
runTableGen(prb_Arena* arena, prb_Str tableGenExe, prb_Str llvmRootDir, prb_Str clangdcdir, prb_Str in, prb_Str out, prb_Str includes, prb_Str args) {
    prb_TempMemory temp = prb_beginTempMemory(arena);

    prb_Str includePaths = {};
    {
        prb_GrowingStr includePathsBuilder = prb_beginStr(arena);
        prb_StrScanner scanner = prb_createStrScanner(includes);
        while (prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_STR(" "), .alwaysMatchEnd = true}, prb_StrScannerSide_AfterMatch)) {
            prb_addStrSegment(&includePathsBuilder, "-I%.*s/%.*s", prb_LIT(llvmRootDir), prb_LIT(scanner.betweenLastMatches));
        }
        includePaths = prb_endStr(&includePathsBuilder);
    }

    prb_Str inpath = prb_pathJoin(arena, llvmRootDir, in);
    prb_Str outpath = prb_pathJoin(arena, clangdcdir, out);

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

    prb_Arena      flatoutArena = prb_createArenaFromArena(arena, 10 * prb_MEGABYTE);
    prb_GrowingStr flatoutBuilder = prb_beginStr(&flatoutArena);
    prb_StrScanner scanner = prb_createStrScanner(outstr);
    while (prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_STR("#include \"")}, prb_StrScannerSide_AfterMatch)) {
        prb_addStrSegment(&flatoutBuilder, "%.*s", prb_LIT(scanner.betweenLastMatches));
        prb_assert(prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_STR("\"")}, prb_StrScannerSide_AfterMatch));
        prb_Str includedFile = scanner.betweenLastMatches;
        prb_assert(prb_strStartsWith(includedFile, prb_STR("clang")));
        prb_Str pathFromRoot = prb_pathJoin(arena, prb_STR("clang/include"), includedFile);
        prb_Str flatpath = replaceSeps(arena, pathFromRoot);
        prb_addStrSegment(&flatoutBuilder, "#include \"%.*s\"", prb_LIT(flatpath));
    }
    prb_addStrSegment(&flatoutBuilder, "%.*s", prb_LIT(scanner.afterMatch));

    prb_Str flatout = prb_endStr(&flatoutBuilder);
    prb_assert(prb_writeEntireFile(arena, outpath, flatout.ptr, flatout.len));
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

int
main() {
    prb_Arena  arena_ = prb_createArenaFromVmem(1 * prb_GIGABYTE);
    prb_Arena  permArena_ = prb_createArenaFromArena(&arena_, 100 * prb_MEGABYTE);
    prb_Arena* permArena = &permArena_;
    prb_Arena* tempArena = &arena_;

    prb_Str rootdir = prb_getParentDir(permArena, prb_STR(__FILE__));
    prb_Str llvmRootDir = prb_pathJoin(permArena, rootdir, prb_STR("llvm-project"));
    prb_Str clangdcdir = prb_pathJoin(permArena, rootdir, prb_STR("clang_src"));
    prb_Str builddir = prb_pathJoin(permArena, rootdir, prb_STR("build"));

    prb_createDirIfNotExists(tempArena, builddir);
    prb_clearDir(tempArena, clangdcdir);

    prb_Str* clangRelevantFiles = 0;
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("clang/tools/driver/driver.cpp")));

    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Support")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/TableGen")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/lib/Option")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/utils/TableGen")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("llvm/utils/TableGen/GlobalISel")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("clang/lib/Support")));
    addAllSrcFiles(permArena, &clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, prb_STR("clang/lib/Driver")));
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
                while (prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_STR("#include \"")}, prb_StrScannerSide_AfterMatch)) {
                    prb_addStrSegment(&newContentBuilder, "%.*s", prb_LIT(scanner.betweenLastMatches));

                    prb_assert(prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_STR("\"")}, prb_StrScannerSide_AfterMatch));
                    prb_Str includedFile = scanner.betweenLastMatches;

                    prb_Str relevantPath = includedFile;
                    if (prb_strStartsWith(includedFile, prb_STR("clang"))) {
                        relevantPath = prb_pathJoin(tempArena, prb_STR("clang/include"), includedFile);
                    } else if (prb_strStartsWith(includedFile, prb_STR("llvm"))) {
                        relevantPath = prb_pathJoin(tempArena, prb_STR("llvm/include"), includedFile);
                    } else if (prb_streq(includedFile, prb_STR("Targets.h"))) {
                        relevantPath = prb_STR("clang/lib/Basic/Targets.h");
                    } else if (!prb_streq(relevantPath, prb_STR("x.h"))) {
                        prb_StrFindResult includeFindRes = prb_strFind(ogpathDir, (prb_StrFindSpec) {.pattern = prb_STR("/llvm-project/")});
                        prb_assert(includeFindRes.found);
                        relevantPath = prb_pathJoin(tempArena, includeFindRes.afterMatch, includedFile);
                    }

                    prb_Str includedFileFlattened = replaceSeps(tempArena, relevantPath);
                    prb_addStrSegment(&newContentBuilder, "#include \"%.*s\"", prb_LIT(includedFileFlattened));

                    if (notIn(includedFile, generatedFiles, prb_arrayCount(generatedFiles))) {
                        arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootDir, relevantPath));
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
    runTableGen(
        tempArena,
        llvmTableGenExe,
        llvmRootDir,
        clangdcdir,
        prb_STR("clang/include/clang/Driver/Options.td"),
        prb_STR("clang_include_clang_Driver_Options.inc"),
        prb_STR("llvm/include"),
        prb_STR("-gen-opt-parser-defs")
    );

    runTableGen(
        tempArena,
        llvmTableGenExe,
        llvmRootDir,
        clangdcdir,
        prb_STR("llvm/include/llvm/Frontend/OpenMP/OMP.td"),
        prb_STR("llvm_include_llvm_Frontend_OpenMP_OMP.inc"),
        prb_STR("llvm/include"),
        prb_STR("--gen-directive-impl")
    );

    runTableGen(
        tempArena,
        clangTableGenExe,
        llvmRootDir,
        clangdcdir,
        prb_STR("clang/include/clang/Basic/Diagnostic.td"),
        prb_STR("clang_include_clang_Basic_DiagnosticCommonKinds.inc"),
        prb_STR("clang/include/clang/Basic"),
        prb_STR("-gen-clang-diags-defs -clang-component=Common")
    );

    runTableGen(
        tempArena,
        clangTableGenExe,
        llvmRootDir,
        clangdcdir,
        prb_STR("clang/include/clang/Basic/Diagnostic.td"),
        prb_STR("clang_include_clang_Basic_DiagnosticDriverKinds.inc"),
        prb_STR("clang/include/clang/Basic"),
        prb_STR("-gen-clang-diags-defs -clang-component=Driver")
    );

    runTableGen(
        tempArena,
        clangTableGenExe,
        llvmRootDir,
        clangdcdir,
        prb_STR("clang/include/clang/Basic/StmtNodes.td"),
        prb_STR("clang_include_clang_AST_StmtNodes.inc"),
        prb_STR("clang/include"),
        prb_STR("-gen-clang-stmt-nodes")
    );

    runTableGen(
        tempArena,
        clangTableGenExe,
        llvmRootDir,
        clangdcdir,
        prb_STR("clang/include/clang/Basic/Attr.td"),
        prb_STR("clang_include_clang_Basic_AttrList.inc"),
        prb_STR("clang/include"),
        prb_STR("-gen-clang-attr-list")
    );

    runTableGen(
        tempArena,
        clangTableGenExe,
        llvmRootDir,
        clangdcdir,
        prb_STR("clang/include/clang/Basic/TypeNodes.td"),
        prb_STR("clang_include_clang_AST_TypeNodes.inc"),
        prb_STR("clang/include"),
        prb_STR("-gen-clang-type-nodes")
    );

    runTableGen(
        tempArena,
        clangTableGenExe,
        llvmRootDir,
        clangdcdir,
        prb_STR("clang/include/clang/Basic/DeclNodes.td"),
        prb_STR("clang_include_clang_AST_DeclNodes.inc"),
        prb_STR("clang/include"),
        prb_STR("-gen-clang-decl-nodes")
    );

    runTableGen(
        tempArena,
        clangTableGenExe,
        llvmRootDir,
        clangdcdir,
        prb_STR("clang/include/clang/AST/CommentCommands.td"),
        prb_STR("clang_include_clang_AST_CommentCommandList.inc"),
        prb_STR("clang/include"),
        prb_STR("-gen-clang-comment-command-list")
    );

    runTableGen(
        tempArena,
        clangTableGenExe,
        llvmRootDir,
        clangdcdir,
        prb_STR("clang/include/clang/StaticAnalyzer/Checkers/Checkers.td"),
        prb_STR("clang_include_clang_StaticAnalyzer_Checkers_Checkers.inc"),
        prb_STR("clang/include/clang/StaticAnalyzer/Checkers"),
        prb_STR("-gen-clang-sa-checkers")
    );

    runTableGen(
        tempArena,
        clangTableGenExe,
        llvmRootDir,
        clangdcdir,
        prb_STR("clang/include/clang/Basic/arm_neon.td"),
        prb_STR("clang_include_clang_Basic_arm_neon.inc"),
        prb_STR("clang/include/clang/Basic"),
        prb_STR("-gen-arm-neon-sema")
    );

    runTableGen(
        tempArena,
        clangTableGenExe,
        llvmRootDir,
        clangdcdir,
        prb_STR("clang/include/clang/Basic/arm_neon.td"),
        prb_STR("clang_include_clang_Basic_arm_neon.h"),
        prb_STR("clang/include/clang/Basic"),
        prb_STR("-gen-arm-neon")
    );

    // TODO(khvorov) Compile the actual compiler
    prb_Str optionLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("llvm_lib_Option"));
    prb_Str clangDriverLibFile = compileStaticLib(Skip_Yes, permArena, builddir, allFilesInSrc, prb_STR("clang_lib_Driver"));
    prb_Str clangBasicLibFile = compileStaticLib(Skip_No, permArena, builddir, allFilesInSrc, prb_STR("clang_lib_Basic"));

    // prb_Str compilerDeps = prb_fmt(
    //     permArena,
    //     "%.*s %.*s %.*s %.*s",
    //     prb_LIT(optionLibFile),
    //     prb_LIT(clangDriverLibFile),
    //     prb_LIT(clangBasicLibFile),
    //     prb_LIT(supportLibFile)
    // );
    // compileExe(Skip_No, permArena, builddir, allFilesInSrc, prb_STR("clang_tools_driver"), compilerDeps);
}
