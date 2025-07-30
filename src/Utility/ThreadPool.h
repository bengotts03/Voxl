//
// Created by Ben Gotts on 29/07/2025.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>

class ThreadPool {
public:
    ThreadPool(size_t numberOfThreads);
    ~ThreadPool();

    void Enqueue(std::function<void()> taskToThread);
private:
    std::vector<std::thread> _threads;
    std::queue<std::function<void()>> _tasksQueue;
    std::mutex _queueMutex;
    std::condition_variable _threadCondition;
    bool _stop;
};



#endif //THREADPOOL_H
