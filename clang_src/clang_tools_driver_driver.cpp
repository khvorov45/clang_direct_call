#include <assert.h>
#include <stdint.h>

// clang-format off
#define mdc_STR(x) (mdc_Str) { x, mdc_strlen(x) }

#ifndef mdc_assert
#define mdc_assert(condition) assert(condition)
#endif
// clang-format on

typedef struct mdc_Str {
    const char* ptr;
    intptr_t    len;
} mdc_Str;

bool
mdc_memeq(const void* ptr1, const void* ptr2, intptr_t bytes) {
    mdc_assert(bytes >= 0);
    bool result = true;
    for (intptr_t index = 0; index < bytes; index++) {
        if (((uint8_t*)ptr1)[index] != ((uint8_t*)ptr2)[index]) {
            result = false;
            break;
        }
    }
    return result;
}

intptr_t
mdc_strlen(const char* str) {
    intptr_t result = 0;
    if (str) {
        while (str[result] != '\0') {
            result += 1;
        }
    }
    return result;
}

bool
mdc_strStartsWith(mdc_Str str, mdc_Str pattern) {
    bool result = false;
    if (pattern.len <= str.len) {
        result = mdc_memeq(str.ptr, pattern.ptr, pattern.len);
    }
    return result;
}

bool
mdc_streq(mdc_Str str1, mdc_Str str2) {
    bool result = false;
    if (str1.len == str2.len) {
        result = mdc_memeq(str1.ptr, str2.ptr, str1.len);
    }
    return result;
}

extern int cc1_main(int argc, char** argv);

extern "C" int
clang_main(int argc, char** argv) {
    // NOTE(khvorov) Only support generating objs via the cc1 tool
    int result = 0;
    {
        mdc_assert(argc >= 2);
        mdc_Str firstArg = mdc_STR(argv[1]);
        mdc_assert(mdc_streq(firstArg, mdc_STR("-cc1")));
        result = cc1_main(argc, argv);
    }

    return result;
}
