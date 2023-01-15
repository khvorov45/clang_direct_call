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

    prb_Str outpathObj = {};
    {
        prb_Str outnameObj = prb_fmt(arena, "prog%d.obj", counter);
        outpathObj = prb_pathJoin(arena, globalTestDir, outnameObj);
        prb_assert(prb_removePathIfExists(arena, outpathObj));

        prb_Str cmdObj = prb_fmt(
            arena,
            "%.*s -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -clear-ast-before-backend -main-file-name %.*s "
            "-mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math "
            "-mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -mllvm -treat-scalable-fixed-error-as-warning "
            "-debugger-tuning=gdb -fcoverage-compilation-dir=%.*s "
            "-I %.*s -internal-isystem /usr/local/include "
            "-internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include "
            "-internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include "
            "-internal-externc-isystem /usr/include -fdebug-compilation-dir=%.*s -ferror-limit 19 "
            "-fgnuc-version=4.2.1 -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 "
            "-o %.*s -x c %.*s",
            prb_LIT(globalMyClangExe),
            prb_LIT(programFilepath),
            prb_LIT(globalTestDir),
            prb_LIT(globalMyClangHeaders),
            prb_LIT(globalTestDir),
            prb_LIT(outpathObj),
            prb_LIT(programFilepath)
        );
        execCmd(arena, cmdObj);
    }

    {
        prb_Str outnameExe = prb_fmt(arena, "prog%d.exe", counter);
        prb_Str outpathExe = prb_pathJoin(arena, globalTestDir, outnameExe);
        prb_assert(prb_removePathIfExists(arena, outpathExe));

        prb_Str cmdExe = prb_fmt(arena, "clang %.*s -o %.*s", prb_LIT(outpathObj), prb_LIT(outpathExe));
        execCmd(arena, cmdExe);
        execCmd(arena, outpathExe);
    }

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
