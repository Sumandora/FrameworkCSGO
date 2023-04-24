#ifndef UTILS_PLATFORM_COMPILERSUPPORT
#define UTILS_PLATFORM_COMPILERSUPPORT

#ifdef __clang__
#define NO_OPTIMIZATION optnone
#else
#define NO_OPTIMIZATION optimize("O0")
#endif

#ifdef __clang__
// Clang writes its own name and its version there
#define COMPILER_NAME __VERSION__
#else
// GCC doesn't write its own name there
#define COMPILER_NAME "GNU " __VERSION__
#endif

#endif