#ifndef __PYTHON_CTYPES_SPEED_TEST_MANAGER__
#define __PYTHON_CTYPES_SPEED_TEST_MANAGER__

#include <vector>
#include <chrono>
#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Worker.h"
#include "Logger.h"

class Manager {
public:
    enum Status {
        IDLE,
        PREPARED,
        STARTED
    };

    Manager();
    ~Manager();

    int prepare(int workers = 1);
    void unprepare();

    int start();
    void stop();
    
    void wait();

    const Status status() const;

private:
    void doWait();
    void workerEndCallback(void* opaque, Worker* worker);

    std::vector<std::unique_ptr<Worker>> workers_;
    Status status_ = IDLE;;

    std::chrono::steady_clock::time_point startTime_;
    std::chrono::steady_clock::time_point endTime_;

    std::atomic<int> finishCount_;

    std::mutex endMutex_;
    std::condition_variable endCond_;

    Logger logger_;
};

#endif