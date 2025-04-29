#pragma once

#include "defines.h"
#include "engine_export.h"

// comment out to disable assertions
#define ASSERTIONS_ENABLED

#ifdef ASSERTIONS_ENABLED
    #if _MSC_VER
        #include <intrin.h>
        #define debugBreak() __debugbreak()
    #else
        #if defined(__clang__)
            #define debugBreak() __builtin_debugtrap()
        #elif defined(__GNUC__)
            #define debugBreak() __builtin_trap()
        #else
            // Fallback - might cause issues or just crash
            #define debugBreak() (*(volatile int*)0 = 0)
        #endif
    #endif

    // define assertions
    ENGINE_EXPORT void report_assertion_failure (const char* expression, const char* message, const char* file, i32 line);

    #define _ASSERT(expr) {                                            \
        if (!(expr)) {                                                        \
            report_assertion_failure (#expr, "", __FILE__, __LINE__);   \
            debugBreak ();                                              \
        }                                                               \
    }

    #define _ASSERT_MSG(expr, message) {                                   \
        if (!(expr)) {                                                            \
            report_assertion_failure(#expr, message, __FILE__, __LINE__);  \
            debugBreak()                                                   \
        }                                                                   \
    }

    #ifdef DEBUG
        #define _ASSERT_DEBUG(expr) {                                      \
            if (!(expr)) {                                                        \
                report_assertion_failure (#expr, "", __FILE__, __LINE__);   \
                debugBreak ();                                              \
            }                                                               \
        }
    #else
        #define _ASSERT_DEBUG(expr) ((void)0)
    #endif
#else
    #define _ASSERT(expr) ((void)0)
    #define _ASSERT_MSG(expr, message) ((void)0)
    #define _ASSERT_DEBUG(expr) ((void)0)
#endif