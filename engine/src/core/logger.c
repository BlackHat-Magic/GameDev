#include "logger.h"

// TEMPORARY
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

b8 initialize_logging () {
    // TODO: create log file
    return (TRUE);
}

void shutdown_logging () {
    // TODO: cleanup logging; write queued entries
}

_API void log_output (log_level level, const char* message, ...) {
    const char* log_strings[6] = {
        "[FATAL]:\t",
        "[ERROR]:\t",
        "[WARN]:\t",
        "[INFO]:\t",
        "[DEBUG]:\t",
        "[TRACE]:\t"
    };

    b8 is_error = level < 2;

    // avoid dynamic memory allocation
    char out_message[65536];
    memset (out_message, 0, sizeof (out_message));

    // format original message
    __builtin_va_list arg_ptr;
    va_start (arg_ptr, message);
    vsnprintf (out_message, 65536, message, arg_ptr);
    va_end (arg_ptr);

    // prepend log level
    char out_message2[65536];
    sprintf (out_message2, "%s%s\n", log_strings[level], out_message);

    // is it an error?
    // TODO: Platform-specific output
    if (is_error) {
        fprintf (stderr, "%s", out_message2);
    } else {
        printf ("%s", out_message2);
    }
}