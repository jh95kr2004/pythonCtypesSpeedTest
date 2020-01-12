#ifndef __PYTHON_CTYPES_SPEED_TEST_WORKER__
#define __PYTHON_CTYPES_SPEED_TEST_WORKER__

#include <thread>
#include <functional>

#include "Logger.h"

class Worker {
public:
    Worker(int id, Logger& logger);
    ~Worker();

    typedef std::function<void(void* opaque, Worker* worker)> WorkerEndCallbackFunction;

    void setEndCallback(WorkerEndCallbackFunction function, void* opaque);

    int start();
    void stop();
    void reset();

private:
    void work();

    int id_;

    std::thread workingThread_;

    WorkerEndCallbackFunction endCallback_;
    void* endCallbackOpaque_;

    bool stop_ = true;

    Logger& logger_;
};

#endif