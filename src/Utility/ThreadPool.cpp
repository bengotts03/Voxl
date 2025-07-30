//
// Created by Ben Gotts on 29/07/2025.
//

#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numberOfThreads) {
    for (int i = 0; i < numberOfThreads; ++i) {
        _threads.emplace_back([this] {
            std::function<void()> task;
            while (true){
                std::unique_lock<std::mutex> threadLock(_queueMutex);

                _threadCondition.wait(threadLock, [this]{
                    return !_tasksQueue.empty() || _stop;
                });

                if (_stop && _tasksQueue.empty()) {
                    return;
                }

                task = _tasksQueue.front();
                _tasksQueue.pop();
            }

            task();
        });
    }
}

ThreadPool::~ThreadPool() {
    std::unique_lock<std::mutex> threadLock(_queueMutex);
    _stop = true;

    _threadCondition.notify_all();

    for (auto& t : _threads)
        t.join();
}

void ThreadPool::Enqueue(std::function<void()> taskToThread) {
    std::unique_lock<std::mutex> threadLock(_queueMutex);
    _tasksQueue.push(taskToThread);

    _threadCondition.notify_one();
}
