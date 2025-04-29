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
    const char* log_strings[7] = {
        "[FATAL]:   ",
        "[ERROR]:   ",
        "[WARN]:    ",
        "[INFO]:    ",
        "[DEBUG]:   ",
        "[TRACE]:   ",
        "[UNKNOWN]: "
    };

    if (level < LOG_LEVEL_FATAL || level > LOG_LEVEL_TRACE) {
        fprintf (stderr, "[UNKNOWN]: Invalid log level %d for message: %s\n", level, message);
        return;
    }

    b8 is_error = level <= LOG_LEVEL_ERROR;

    // avoid dynamic memory allocation
    char va_formatted_message[LOG_MESSAGE_MAX_LENGTH];
    memset (va_formatted_message, 0, sizeof (va_formatted_message));

    va_list arg_ptr;
    va_start (arg_ptr, message);
    vsnprintf (va_formatted_message, LOG_MESSAGE_MAX_LENGTH, message, arg_ptr);
    va_end (arg_ptr);

    FILE* output_stream = is_error ? stderr : stdout;
    fprintf (output_stream, "%s%s\n", log_strings[level], va_formatted_message);
    if (is_error) {
        fflush (output_stream);
    }
}