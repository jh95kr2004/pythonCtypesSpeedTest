#include <iostream>

#include "Manager.h"

Manager::Manager() {
    logger_.log(__FILE__, __LINE__, "[M] Created");
}

Manager::~Manager() {
    if(status_ == STARTED) {
        stop();
    }

    if(status_ == PREPARED) {
        unprepare();
    }

    logger_.log(__FILE__, __LINE__, "[M] Destroyed");
}

int Manager::prepare(int workers) {
    if(status_ != IDLE) {
        logger_.log(__FILE__, __LINE__, "[M] Invalid status");
        return -1;
    }

    logger_.log(__FILE__, __LINE__, "[M] Creates %d workers", workers);

    for(int i = 0; i < workers; i++) {
        Worker* worker = new Worker(i, logger_);

        worker->setEndCallback([this](void* opaque, Worker* worker) {
            workerEndCallback(opaque, worker);
        }, nullptr);

        workers_.emplace_back(worker);
    }

    status_ = PREPARED;

    logger_.log(__FILE__, __LINE__, "[M] %d workers are created", workers);

    return 0;
}

void Manager::unprepare() {
    if(status_ != PREPARED) {
        logger_.log(__FILE__, __LINE__, "[M] Invalid status");
        return;
    }

    workers_.clear();

    status_ = IDLE;

    logger_.log(__FILE__, __LINE__, "[M] Unprepared");
}

int Manager::start() {
    if(status_ != PREPARED) {
        logger_.log(__FILE__, __LINE__, "[M] Invalid status");
        return -1;
    }

    logger_.log(__FILE__, __LINE__, "[M] Starts all workers");

    startTime_ = std::chrono::steady_clock::now();

    finishCount_ = 0;

    for(auto it = workers_.begin(); it != workers_.end(); it++) {
        if(it->get()->start()) {

            while(it != workers_.begin()) {
                it--;
                it->get()->stop();
            }

            return -1;
        }
    }

    status_ = STARTED;

    logger_.log(__FILE__, __LINE__, "[M] Started");

    return 0;
}

void Manager::stop() {
    if(status_ != STARTED) {
        logger_.log(__FILE__, __LINE__, "[M] Invalid status");
        return;
    }

    logger_.log(__FILE__, __LINE__, "[M] Stops all workers");

    for(auto it = workers_.begin(); it != workers_.end(); it++) {
        it->get()->stop();
    }

    doWait();

    logger_.log(__FILE__, __LINE__, "[M] Stopped");
}

void Manager::wait() {
    if(status_ != STARTED) {
        logger_.log(__FILE__, __LINE__, "[M] Invalid status");
        return;
    }

    logger_.log(__FILE__, __LINE__, "[M] Wait all workers finishing job");

    doWait();
}

const Manager::Status Manager::status() const {
    return status_;
}

void Manager::doWait() {
    std::unique_lock<std::mutex> lck(endMutex_);
    endCond_.wait(lck, [this]{
        return status_ == PREPARED;
    });

    for(auto it = workers_.begin(); it != workers_.end(); it++) {
        it->get()->reset();
    }
}

void Manager::workerEndCallback(void* opaque, Worker* worker) {
    finishCount_ = finishCount_ + 1;

    if(finishCount_.load() == workers_.size()) {
        endTime_ = std::chrono::steady_clock::now();

        std::unique_lock<std::mutex> lck(endMutex_);
        status_ = PREPARED;
        endCond_.notify_one();

        std::chrono::duration<double, std::milli> elapsed = endTime_ - startTime_;

        logger_.log(__FILE__, __LINE__, "[M] All workers are finished (%.2f ms)", elapsed.count());
    }
}