#include <cstdint>

#include "Worker.h"

Worker::Worker(int id, Logger& logger)
: id_(id), logger_(logger) {
    logger_.log(__FILE__, __LINE__, "[W%d] Created", id_);
}

Worker::~Worker() {
    if(stop_ == false) {
        stop();
    }

    if(workingThread_.joinable()) {
        workingThread_.join();
    }

    logger_.log(__FILE__, __LINE__, "[W%d] Destroyed", id_);
}

void Worker::setEndCallback(WorkerEndCallbackFunction function, void* opaque) {
    endCallback_ = function;
    endCallbackOpaque_ = opaque;
}

int Worker::start() {
    if(stop_ == false) {
        return -1;
    }

    stop_ = false;

    if(workingThread_.joinable()) {
        workingThread_.join();
    }

    logger_.log(__FILE__, __LINE__, "[W%d] Starts work", id_);

    workingThread_ = std::thread([this]{
        work();
        endCallback_(endCallbackOpaque_, this);
    });

    return 0;
}

void Worker::stop() {
    logger_.log(__FILE__, __LINE__, "[W%d] Stop", id_);

    stop_ = true;
}

void Worker::reset() {
    stop_ = true;

    if(workingThread_.joinable()) {
        workingThread_.join();
    }
}

void Worker::work() {
    for(int t = 0; t < 100; t++) {
        uint8_t* A = new uint8_t[2160 * 3840 * 3];
        uint8_t* B = new uint8_t[2160 * 3840 * 3];

        for(int i = 0; i < 2160 && stop_ == false; i++) {
            for(int j = 0; j < 3840 && stop_ == false; j++) {
                for(int k = 0; k < 3 && stop_ == false; k++) {
                    int index = i * 3840 * 3 + j * 3 + k;
                    A[index] = A[index] + B[index];
                }
            }
        }

        delete[] A;
        delete[] B;
    }

    logger_.log(__FILE__, __LINE__, "[W%d] Finished work", id_);
}