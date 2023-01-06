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
        prb_Str        outpath = prb_pathJoin(tempArena, clangdcdir, prb_STR("llvm_include_llvm_Config_abi-breaking.h"));
        prb_Str        content = prb_STR("");
        prb_assert(prb_writeEntireFile(tempArena, outpath, content.ptr, content.len));
        prb_endTempMemory(temp);
    }

    // TODO(khvorov) Compile just the table gen
    for (i32 newCppfileIndex = 0; newCppfileIndex < arrlen(newClangCppFiles); newCppfileIndex++) {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str        path = newClangCppFiles[newCppfileIndex];
        prb_assert(prb_strEndsWith(path, prb_STR(".cpp")));
        prb_Str name = prb_getLastEntryInPath(path);
        if (prb_strStartsWith(name, prb_STR("clang_utils"))) {
            prb_Str out = prb_replaceExt(tempArena, path, prb_STR("obj"));
            prb_Str cmd = prb_fmt(tempArena, "clang -Wfatal-errors -std=c++17 -c %.*s -o %.*s", prb_LIT(path), prb_LIT(out));
            prb_writelnToStdout(tempArena, cmd);
            prb_Process proc = prb_createProcess(cmd, (prb_ProcessSpec) {});
            prb_assert(prb_launchProcesses(tempArena, &proc, 1, prb_Background_No));
        }
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
