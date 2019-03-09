#pragma once

/**********************************************************************
 * locking-queue.hh
 *
 * A trivially simple example of a thread-safe queue based on mutex
 * locking.
 *
 *      * Allegedly thread- and exception-safe.
 *
 *      * Queue has a maximum size to imitate ZeroMQ's message queues.
 *
 **********************************************************************/


#include <queue>
#include <mutex>
#include <condition_variable>


template <typename T>
class LockingQueue {

    public:

        LockingQueue(size_t qMax):
            myQMax(qMax),
            myQ(),
            myMutex(),
            myNotEmpty(),
            myNotFull()
        {};

        ~LockingQueue() {};

        void push(T& task)
        {
            std::unique_lock<std::mutex> lock(myMutex);
            myNotFull.wait(lock, [this]{ return myQ.size() < myQMax; });

            myQ.push(std::move(task));
            myNotEmpty.notify_one();
        };

        void pop(T& task)
        {
            std::unique_lock<std::mutex> lock(myMutex);
            myNotEmpty.wait(lock, [this]{ return !myQ.empty(); });

            task = std::move(myQ.front());
            myQ.pop(); // does not throw
            myNotFull.notify_one();
        };

    private:

        size_t                  myQMax;
        std::queue<T>           myQ;
        std::mutex              myMutex;
        std::condition_variable myNotEmpty;
        std::condition_variable myNotFull;

        // Copying me makes baby Jesus cry
        LockingQueue(LockingQueue& ) = delete;
        LockingQueue(LockingQueue&&) = delete;
        LockingQueue&  operator=(LockingQueue& ) = delete;
        LockingQueue&& operator=(LockingQueue&&) = delete;
};
