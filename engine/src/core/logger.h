#pragma once

#include "defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

// disable debug and trace for release
#if RELEASE == 1
    #define LOG_DEBUG_ENABLED 0
    #define LOG_TRACE_ENABLED 0
#endif

typedef enum log_level {
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE
} log_level;

b8 initialize_logging ();
void shutdown_logging ();

API void log_output (log_level level, const char* message, ...);

#define _FATAL (message, ...) log_output (LOG_LEVEL_FATAL, message, ##__VA_ARGS__);

// logs error-level message
#ifndef _ERROR
    #define _ERROR (message, ...) log_output (LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

// logs warning-level message
#if LOG_WARN_ENABLED == 1
    #define _WARN (message, ...) log_output (LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#else
    #define _WARN (message, ...)
#endif

// logs info-level message
#if LOG_INFO_ENABLED == 1
    #define _INFO (message, ...) log_output (LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
    #define _INFO (message, ...)
#endif

// logs debug-level message
#if LOG_DEBUG_ENABLED == 1
    #define _DEBUG (message, ...) log_output (LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
    #define _DEBUG (message, ...)
#endif

// logs trace-level message
#if LOG_TRACE_ENABLED == 1
    #define _TRACE (message, ...) log_output (LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
    #define _TRACE (message, ...)
#endif