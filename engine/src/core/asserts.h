#pragma once

#include "defines.h"

// comment out to disable assertions
#define ASSERTIONS_ENABLED

#ifdef ASSERTIONS_ENABLED
    #if _MSC_VER
        #include <intrin.h>
        #define debugBreak () __debugbreak ()
    #else
        #define debugBreak () __builtin_trap ()
    #endif

    // define assertions
    _API void report_assertion_failure (const char* expression, const char* message, const char* file, i32 line);

    #define _ASSERT (expr) {                                            \
        if (expr) {                                                     \
            // ...
        } else {                                                        \
            report_assertion_failure (#expr, "", __FILE__, __LINE__);   \
            debugBreak ();                                              \
        }                                                               \
    }

    #define _ASSERT_MSG (expr, message) {                                   \
        if (expr) {                                                         \
            // ...
        } else {                                                            \
            report_assertion_failure (#expr, message, __FILE__, __LINE__);  \
            debugBreak ()                                                   \
        }                                                                   \
    }

    #ifdef _DEBUG
        #define _ASSERT_DEBUG (expr) {                                      \
            if (expr) {                                                     \
                // ...
            } else {                                                        \
                report_assertion_failure (#expr, "", __FILE__, __LINE__);   \
                debugBreak ();                                              \
            }                                                               \
        }
    #else
        #define _ASSERT_DEBUG (expr)
    #endif
#else
    #define _ASSERT (expr)
    #define _ASSERT_MSG (expr, message)
    #define _ASSERT_DEBUG (expr)
#endif