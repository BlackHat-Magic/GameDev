#include <core/logger.h>

int main (int argc, char* argv[]) {
    _FATAL ("A test message: %f", 3.14f);
    _ERROR ("A test message: %f", 3.14f);
    _WARN ("A test message: %f", 3.14f);
    _INFO ("A test message: %f", 3.14f);
    _DEBUG ("A test message: %f", 3.14f);
    _TRACE ("A test message: %f", 3.14f);
    return (0);
}