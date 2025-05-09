#pragma once

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long long    i64;

typedef float   f32;
typedef double  f64;

// bools
typedef int     b32;
typedef char    b8;

#define TRUE 1
#define FALSE 0

#if defined(__clang__) || defined(__gcc__) || defined(__GNUC__)
    #define STATIC_ASSERT _Static_assert
#elif defined(_MSC_VER)
    #define STATIC_ASSERT static_assert
#else
    #define STATIC_ASSERT static_assert
#endif

STATIC_ASSERT (sizeof (u8) == 1, "Expected u8 to be 1 byte.");
STATIC_ASSERT (sizeof (u16) == 2, "Expected u16 to be 2 bytes.");
STATIC_ASSERT (sizeof (u32) == 4, "Expected u32 to be 4 bytes.");
STATIC_ASSERT (sizeof (u64) == 8, "Expected u64 to be 8 bytes.");

STATIC_ASSERT (sizeof (i8) == 1, "Expected i8 to be 1 byte.");
STATIC_ASSERT (sizeof (i16) == 2, "Expected i16 to be 2 bytes.");
STATIC_ASSERT (sizeof (i32) == 4, "Expected i32 to be 4 bytes.");
STATIC_ASSERT (sizeof (i64) == 8, "Expected i64 to be 8 bytes.");

STATIC_ASSERT (sizeof (f32) == 4, "Expected xx to be 4 bytes.");
STATIC_ASSERT (sizeof (f64) == 8, "Expected xx to be 8 bytes.");

// detect platform
// windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define PLATFORM_WINDOWS 1
    #ifndef _WIN64
        #error "Only 64-bit Windows supported."
    #endif
#elif defined(__linux__) || defined(__gnu_linux__)
    #define PLATFORM_LINUX 1
    #if defined(__ANDROID__)
        #define PLATFORM_ANDROID 1
    #endif
#elif defined(__unix__) // Generic Unix (might overlap with Linux/Apple)
    #define PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
    #define PLATFORM_POSIX 1
#elif __APPLE__
    #define PLATFORM_APPLE 1
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
        #define PLATFORM_IOS 1
        #define PLATFORM_IOS_SIMULATOR 1
    #elif TARGET_OS_IPHONE
        #define PLATFORM_IOS 1
    #elif TARGET_OS_MAC
        #define PLATFORM_MACOS 1 // Define specific MacOS platform
    #else
        #error "Unknown Apple Platform."
    #endif
#else
    #error "Unknown platform."
#endif