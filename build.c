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

int
main() {
    prb_Arena  arena_ = prb_createArenaFromVmem(1 * prb_GIGABYTE);
    prb_Arena  permArena_ = prb_createArenaFromArena(&arena_, 100 * prb_MEGABYTE);
    prb_Arena* permArena = &permArena_;
    prb_Arena* tempArena = &arena_;

    prb_Str rootdir = prb_getParentDir(permArena, prb_STR(__FILE__));
    prb_Str llvmRootdir = prb_pathJoin(permArena, rootdir, prb_STR("llvm-project"));
    prb_Str clangdcdir = prb_pathJoin(permArena, rootdir, prb_STR("clang_src"));
    prb_Str builddir = prb_pathJoin(permArena, rootdir, prb_STR("build"));

    prb_clearDir(tempArena, builddir);
    prb_clearDir(tempArena, clangdcdir);

    prb_Str* clangRelevantFiles = 0;
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("clang/utils/TableGen/TableGen.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("clang/tools/driver/driver.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/CommandLine.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/Error.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/raw_ostream.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/SmallVector.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/ErrorHandling.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/Debug.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/circular_raw_ostream.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/ManagedStatic.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/StringRef.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/Hashing.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/APFloat.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/APInt.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/Signals.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/FormatVariadic.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/NativeFormatting.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/Path.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/Twine.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/Process.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/CrashRecoveryContext.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/ThreadLocal.cpp")));
    arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/lib/Support/Threading.cpp")));

    NewpathOgpath* newpaths = 0;
    prb_Str*       newClangCppFiles = 0;

    // NOTE(khvorov) Pull the relevant files
    for (i32 clangSrcFileIndex = 0; clangSrcFileIndex < arrlen(clangRelevantFiles); clangSrcFileIndex++) {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        ogpath = clangRelevantFiles[clangSrcFileIndex];
        prb_writelnToStdout(tempArena, ogpath);

        bool isCpp = prb_strEndsWith(ogpath, prb_STR(".cpp"));

        prb_Str newpath = {};
        {
            prb_StrFindResult res = prb_strFind(ogpath, (prb_StrFindSpec) {.pattern = prb_STR("/llvm-project/")});
            prb_assert(res.found);
            prb_Str newname = replaceSeps(tempArena, res.afterMatch);
            newpath = prb_pathJoin(permArena, clangdcdir, newname);
            if (isCpp) {
                arrput(newClangCppFiles, newpath);
            }
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
            };
            i32 newpathIndex = shgeti(newpaths, newpath.ptr);
            if (newpathIndex == -1) {
                shput(newpaths, newpath.ptr, ogpath);
                prb_TempMemory temp = prb_beginTempMemory(tempArena);
                prb_Str        ogpathDir = prb_getParentDir(tempArena, ogpath);

                // NOTE(khvorov) Scan for includes
                prb_ReadEntireFileResult clangSrcFileReadRes = prb_readEntireFile(tempArena, ogpath);
                prb_assert(clangSrcFileReadRes.success);
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
                    } else {
                        prb_StrFindResult includeFindRes = prb_strFind(ogpathDir, (prb_StrFindSpec) {.pattern = prb_STR("/llvm-project/")});
                        prb_assert(includeFindRes.found);
                        relevantPath = prb_pathJoin(tempArena, includeFindRes.afterMatch, includedFile);
                    }

                    prb_Str includedFileFlattened = replaceSeps(tempArena, relevantPath);
                    prb_addStrSegment(&newContentBuilder, "#include \"%.*s\"", prb_LIT(includedFileFlattened));

                    if (notIn(includedFile, generatedFiles, prb_arrayCount(generatedFiles))) {
                        arrput(clangRelevantFiles, prb_pathJoin(permArena, llvmRootdir, relevantPath));
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

    // TODO(khvorov) Compile just the table gen
    prb_Str tableGenSrc[] = {
        prb_pathJoin(permArena, clangdcdir, prb_STR("clang_utils_TableGen_TableGen.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_CommandLine.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_Error.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_raw_ostream.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_SmallVector.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_ErrorHandling.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_Debug.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_circular_raw_ostream.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_ManagedStatic.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_StringRef.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_Hashing.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_APFloat.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_APInt.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_Signals.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_FormatVariadic.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_NativeFormatting.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_Path.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_Twine.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_Process.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_CrashRecoveryContext.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_ThreadLocal.cpp")),
        prb_pathJoin(permArena, clangdcdir, prb_STR("llvm_lib_Support_Threading.cpp")),
    };
    prb_Str tableGenDir = prb_pathJoin(permArena, builddir, prb_STR("tablegen"));
    prb_clearDir(tempArena, tableGenDir);
    prb_Str* tableGenObjs = 0;
    for (i32 srcIndex = 0; srcIndex < prb_arrayCount(tableGenSrc); srcIndex++) {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        path = tableGenSrc[srcIndex];
        prb_assert(prb_strEndsWith(path, prb_STR(".cpp")));
        prb_Str filename = prb_getLastEntryInPath(path);
        prb_Str outname = prb_replaceExt(tempArena, filename, prb_STR("obj"));
        prb_Str out = prb_pathJoin(permArena, tableGenDir, outname);
        arrput(tableGenObjs, out);
        prb_Str defines = prb_STR(
            "-DLLVM_ON_UNIX -DPACKAGE_NAME=\"LLVM\" -DPACKAGE_VERSION=\"420.69\" "
            "-DHAVE_FCNTL_H=1 -DHAVE_UNISTD_H=1 -DLLVM_ENABLE_THREADS=1 -DHAVE_SYSEXITS_H=1 "
            "-DHAVE_SYS_STAT_H=1 -DLLVM_WINDOWS_PREFER_FORWARD_SLASH=1 -DHAVE_SYS_MMAN_H=1 "
            "-DHAVE_FUTIMENS=1 -DLLVM_ENABLE_CRASH_DUMPS=1 -DHAVE_GETRUSAGE=1 -DHAVE_SYS_RESOURCE_H=1 "
            "-DHAVE_GETPAGESIZE=1 -DHAVE_MALLINFO2=1 -DHAVE_PTHREAD_H"
        );
        prb_Str cmd = prb_fmt(tempArena, "clang %.*s -Werror -Wfatal-errors -std=c++17 -c %.*s -o %.*s", prb_LIT(defines), prb_LIT(path), prb_LIT(out));
        prb_writelnToStdout(tempArena, cmd);
        prb_Process proc = prb_createProcess(cmd, (prb_ProcessSpec) {});
        prb_assert(prb_launchProcesses(tempArena, &proc, 1, prb_Background_No));
        prb_endTempMemory(temp);
    }

    // TODO(khvorov) Link table gen
    prb_Str tableGenExe = prb_pathJoin(permArena, tableGenDir, prb_STR("tablegen.exe"));
    {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_GrowingStr linkCmdBuilder = prb_beginStr(tempArena);
        prb_addStrSegment(&linkCmdBuilder, "clang");
        for (i32 objIndex = 0; objIndex < arrlen(tableGenObjs); objIndex++) {
            prb_Str obj = tableGenObjs[objIndex];
            prb_addStrSegment(&linkCmdBuilder, " %.*s", prb_LIT(obj));
        }
        prb_addStrSegment(&linkCmdBuilder, " -o %.*s -lstdc++ -lm", prb_LIT(tableGenExe));
        prb_Str cmd = prb_endStr(&linkCmdBuilder);
        prb_writelnToStdout(tempArena, cmd);
        prb_Process proc = prb_createProcess(cmd, (prb_ProcessSpec) {});
        prb_assert(prb_launchProcesses(tempArena, &proc, 1, prb_Background_No));
        prb_endTempMemory(temp);
    }

    // TODO(khvorov) Generate the files we need table gen for
    {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        outpath = prb_pathJoin(tempArena, clangdcdir, prb_STR("clang_include_clang_Basic_DiagnosticCommonKinds.inc"));
        prb_Str        content = prb_STR("");
        prb_assert(prb_writeEntireFile(tempArena, outpath, content.ptr, content.len));
        prb_endTempMemory(temp);
    }

    {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        outpath = prb_pathJoin(tempArena, clangdcdir, prb_STR("clang_include_clang_Basic_DiagnosticDriverKinds.inc"));
        prb_Str        content = prb_STR("");
        prb_assert(prb_writeEntireFile(tempArena, outpath, content.ptr, content.len));
        prb_endTempMemory(temp);
    }

    {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        outpath = prb_pathJoin(tempArena, clangdcdir, prb_STR("clang_include_clang_Driver_Options.inc"));
        prb_Str        content = prb_STR("");
        prb_assert(prb_writeEntireFile(tempArena, outpath, content.ptr, content.len));
        prb_endTempMemory(temp);
    }

    // TODO(khvorov) Compile the actual compiler
    for (i32 newCppfileIndex = 0; newCppfileIndex < arrlen(newClangCppFiles); newCppfileIndex++) {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        path = newClangCppFiles[newCppfileIndex];
        prb_assert(prb_strEndsWith(path, prb_STR(".cpp")));
        prb_Str name = prb_getLastEntryInPath(path);
        if (!prb_strStartsWith(name, prb_STR("clang_utils"))) {
            prb_Str out = prb_replaceExt(tempArena, path, prb_STR("obj"));
            prb_Str cmd = prb_fmt(tempArena, "clang -Wfatal-errors -std=c++17 -c %.*s -o %.*s", prb_LIT(path), prb_LIT(out));
            prb_writelnToStdout(tempArena, cmd);
            prb_Process proc = prb_createProcess(cmd, (prb_ProcessSpec) {});
            prb_assert(prb_launchProcesses(tempArena, &proc, 1, prb_Background_No));
        }
        prb_endTempMemory(temp);
    }
}
