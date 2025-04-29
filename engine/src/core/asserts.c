#include "asserts.h"
#include "logger.h"

void report_assertion_failure (const char* expression, const char* message, const char* file, i32 line) {
    log_output (LOG_LEVEL_FATAL, "Assertion Failure: %s, message: %s, in file: %s, line: %d\n", expression, message, file, line);
}