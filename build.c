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

int
main() {
    prb_Arena  arena_ = prb_createArenaFromVmem(1 * prb_GIGABYTE);
    prb_Arena  permArena_ = prb_createArenaFromArena(&arena_, 100 * prb_MEGABYTE);
    prb_Arena* permArena = &permArena_;
    prb_Arena* tempArena = &arena_;

    prb_Str rootdir = prb_getParentDir(permArena, prb_STR(__FILE__));
    prb_Str llvmRootdir = prb_pathJoin(permArena, rootdir, prb_STR("llvm-project"));
    prb_Str llvmIncludeDir = prb_pathJoin(permArena, llvmRootdir, prb_STR("llvm/include"));
    prb_Str clangogdir = prb_pathJoin(permArena, llvmRootdir, prb_STR("clang"));
    prb_Str clangIncludeDir = prb_pathJoin(permArena, clangogdir, prb_STR("include"));
    prb_Str clangdcdir = prb_pathJoin(permArena, rootdir, prb_STR("clang_src"));
    prb_Str builddir = prb_pathJoin(permArena, rootdir, prb_STR("build"));

    prb_clearDir(tempArena, builddir);
    prb_clearDir(tempArena, clangdcdir);

    prb_Str*       clangRelevantFiles = 0;
    NewpathOgpath* newpaths = 0;
    arrput(clangRelevantFiles, prb_STR("tools/driver/driver.cpp"));

    // NOTE(khvorov) Pull the relevant files
    for (i32 clangSrcFileIndex = 0; clangSrcFileIndex < arrlen(clangRelevantFiles); clangSrcFileIndex++) {
        prb_TempMemory temp = prb_beginTempMemory(tempArena);
        prb_Str clangSrcFile = clangRelevantFiles[clangSrcFileIndex];

        prb_Str ogpath = {};
        if (prb_strEndsWith(clangSrcFile, prb_STR(".cpp"))) {
            ogpath = prb_pathJoin(permArena, clangogdir, clangSrcFile);
        } else if (prb_strStartsWith(clangSrcFile, prb_STR("clang"))) {
            ogpath = prb_pathJoin(permArena, clangIncludeDir, clangSrcFile);
        } else {
            if (!prb_strStartsWith(clangSrcFile, prb_STR("llvm"))) {
                prb_writelnToStdout(tempArena, clangSrcFile);
                prb_assert(!"unrecognized");
            }
            ogpath = prb_pathJoin(permArena, llvmIncludeDir, clangSrcFile);
        }
        prb_writelnToStdout(tempArena, ogpath);

        prb_Str newpath = prb_pathJoin(permArena, clangdcdir, clangSrcFile);
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
            };
            i32 newpathIndex = shgeti(newpaths, newpath.ptr);
            if (newpathIndex == -1) {
                shput(newpaths, newpath.ptr, ogpath);
                prb_TempMemory temp = prb_beginTempMemory(tempArena);

                // NOTE(khvorov) Copy file
                prb_ReadEntireFileResult clangSrcFileReadRes = prb_readEntireFile(tempArena, ogpath);
                prb_assert(clangSrcFileReadRes.success);
                prb_assert(prb_writeEntireFile(tempArena, newpath, clangSrcFileReadRes.content.data, clangSrcFileReadRes.content.len));

                // NOTE(khvorov) Scan for includes
                prb_Str        clangSrcFileStr = prb_strFromBytes(clangSrcFileReadRes.content);
                prb_StrScanner scanner = prb_createStrScanner(clangSrcFileStr);
                while (prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_STR("#include \"")}, prb_StrScannerSide_AfterMatch)) {
                    prb_assert(prb_strScannerMove(&scanner, (prb_StrFindSpec) {.pattern = prb_STR("\"")}, prb_StrScannerSide_AfterMatch));
                    prb_Str includedFile = scanner.betweenLastMatches;
                    if (notIn(includedFile, generatedFiles, prb_arrayCount(generatedFiles))) {
                        // TODO(khvorov) If doesn't start with clang/llvm then it's next to the parent?
                        arrput(clangRelevantFiles, prb_fmt(permArena, "%.*s", prb_LIT(includedFile)));
                    }
                }

                prb_endTempMemory(temp);
            } else {
                NewpathOgpath prevPath = newpaths[newpathIndex];
                prb_assert(prb_streq(prevPath.value, ogpath));
            }
        }

        prb_endTempMemory(temp);
    }

    // // NOTE(khvorov) Compile the relevant files
    // for (i32 clangSrcFileIndex = 0; clangSrcFileIndex < prb_arrayCount(clangSrcFilesPulled); clangSrcFileIndex++) {
    //     prb_Str path = clangSrcFilesPulled[clangSrcFileIndex];
    //     if (prb_strEndsWith(path, prb_STR(".cpp"))) {
    //         prb_Str out = prb_replaceExt(arena, path, prb_STR("obj"));
    //         prb_Str cmd = prb_fmt(arena, "clang %.*s -o %.*s", prb_LIT(path), prb_LIT(out));
    //         prb_writelnToStdout(arena, cmd);
    //         prb_Process proc = prb_createProcess(cmd, (prb_ProcessSpec) {});
    //         prb_assert(prb_launchProcesses(arena, &proc, 1, prb_Background_No));
    //     }
    // }
}
