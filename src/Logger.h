#ifndef _PYTHON_CTYPES_SPEED_TEST_LOGGER_H_
#define _PYTHON_CTYPES_SPEED_TEST_LOGGER_H_

#include <string>
#include <mutex>

class Logger {
public:
    void log(const char* file, const int line, const char* format, ...);

private:
    static std::string getTimestamp();
    static const std::string trimPath(std::string path);

    std::mutex mutex_;
};

#endif