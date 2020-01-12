#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>

#include <cstdarg>
#include <unistd.h>

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "Logger.h"

std::string Logger::getTimestamp() {
    struct timeval tv;
    time_t cur;

    gettimeofday(&tv, nullptr);
    cur = tv.tv_sec;

    char time_stamp_head[32] = {0,};
    strftime(time_stamp_head, sizeof(time_stamp_head), "%H:%M:%S", localtime(&cur));

    char time_stamp[128];
    snprintf(time_stamp, sizeof(time_stamp), "%s,%03d", time_stamp_head, tv.tv_usec / 1000);

    return time_stamp;
}

void Logger::log(const char* file, const int line, const char* format, ...) {
    std::unique_lock<std::mutex> lck(mutex_);

    std::string timestamp = getTimestamp();
    char prefix[256], buffer[1024];
    va_list lpStart;

    sprintf(prefix, "[\x1b[36m%s\x1b[0m] {%s:%d} ", timestamp.c_str(), trimPath(file).c_str(), line);

    va_start(lpStart, format);
    vsnprintf(buffer, sizeof(buffer), format, lpStart);
    va_end(lpStart);

    std::cout << prefix << buffer << std::endl;
}

const std::string Logger::trimPath(std::string path) {
    std::size_t first = path.rfind('/');
    if(first == std::string::npos || first == 0) {
        return path;
    }

    std::size_t second = path.rfind('/', first - 1);
    if(second == std::string::npos) {
        return path.substr(first);
    }

    std::size_t third = path.rfind('/', second - 1);
    if(third == std::string::npos) {
        return path.substr(second);
    }

    return path.substr(third);
}