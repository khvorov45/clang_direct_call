#include "cbuild.h"

#define function static
#define global_variable static

typedef int32_t  i32;
typedef uint64_t u64;

global_variable prb_Str globalRootDir;
global_variable prb_Str globalTestDir;
global_variable prb_Str globalMyClangExe;
global_variable prb_Str globalMyClangHeaders;

function void
execCmd(prb_Arena* arena, prb_Str cmd) {
    prb_TempMemory temp = prb_beginTempMemory(arena);
    prb_writelnToStdout(arena, cmd);
    prb_Process proc = prb_createProcess(cmd, (prb_ProcessSpec) {});
    prb_assert(prb_launchProcesses(arena, &proc, 1, prb_Background_No));
    prb_endTempMemory(temp);
}

function void
runTestForProgram(prb_Arena* arena, i32 counter, prb_Str program) {
    prb_TempMemory temp = prb_beginTempMemory(arena);

    prb_Str programFilepath = {};
    {
        prb_Str name = prb_fmt(arena, "prog%d.c", counter);
        programFilepath = prb_pathJoin(arena, globalTestDir, name);
        prb_assert(prb_writeEntireFile(arena, programFilepath, program.ptr, program.len));
    }

    prb_Str outname = prb_fmt(arena, "prog%d.exe", counter);
    prb_Str outpath = prb_pathJoin(arena, globalTestDir, outname);
    prb_Str cmd = prb_fmt(arena, "%.*s -I%.*s %.*s -o %.*s", prb_LIT(globalMyClangExe), prb_LIT(globalMyClangHeaders), prb_LIT(programFilepath), prb_LIT(outpath));
    execCmd(arena, cmd);
    execCmd(arena, outpath);

    prb_endTempMemory(temp);
}

int
main() {
    prb_Arena  arena_ = prb_createArenaFromVmem(1 * prb_GIGABYTE);
    prb_Arena* arena = &arena_;

    globalRootDir = prb_getParentDir(arena, prb_STR(__FILE__));
    globalTestDir = prb_pathJoin(arena, globalRootDir, prb_STR("tests"));
    globalMyClangExe = prb_pathJoin(arena, globalRootDir, prb_STR("build/clang.exe"));
    globalMyClangHeaders = prb_pathJoin(arena, globalRootDir, prb_STR("llvm-project/clang/lib/Headers"));

    prb_assert(prb_clearDir(arena, globalTestDir));

    // NOTE(khvorov) Just a sanity check for now, not a thorough test suite

    i32 testPostfixCounter = 0;
    runTestForProgram(arena, testPostfixCounter++, prb_STR("#include \"../cbuild.h\"\nint main() {prb_writeToStdout(prb_STR(\"compiled and ran\\n\"));return 0;}"));

    return 0;
}
