#include "logger.h"

// TEMPORARY
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define LOG_MESSAGE_MAX_LENGTH 4096

b8 initialize_logging () {
    // TODO: create log file
    return (TRUE);
}

void shutdown_logging () {
    // TODO: cleanup logging; write queued entries
}

void log_output (log_level level, const char* message, ...) {
    const char* log_strings[6] = {
        "[FATAL]:\t",
        "[ERROR]:\t",
        "[WARN]:\t",
        "[INFO]:\t",
        "[DEBUG]:\t",
        "[TRACE]:\t"
    };

    b8 is_error = level < 2;

    if (level < LOG_LEVEL_FATAL || level > LOG_LEVEL_TRACE) {
        fprintf (stderr, "[UNKNOWN]:\t")
    }

    // format original message
    __builtin_va_list arg_ptr;
    va_start (arg_ptr, message);
    vsnprintf (out_message, 65536, message, arg_ptr);
    va_end (arg_ptr);

    // prepend log level
    char formatted_message[65536];
    snprintf (formatted_message, sizeof (formatted_message), "%s%s\n", log_strings[level], out_message);

    // is it an error?
    // TODO: Platform-specific output
    if (is_error) {
        fprintf (stderr, "%s", formatted_message);
    } else {
        printf ("%s", formatted_message);
    }
}