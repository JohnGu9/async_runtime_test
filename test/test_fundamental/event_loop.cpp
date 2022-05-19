#include "async_runtime/fundamental/event_loop.h"
#include <iostream>
#include <mutex>
#include <thread>

void task();
int main()
{
    EventLoop::run(task);
    return 0;
}

void task()
{
    std::cout << "EventLoop master started " << std::this_thread::get_id() << std::endl;

    // Cross thread operation, you need to sync threads by yourself
    std::mutex mutex;
    std::condition_variable cv;
    std::unique_lock<std::mutex> lock(mutex);
    bool done = false;

    std::cout << "EventLoop master waiting worker task to complete " << std::this_thread::get_id() << std::endl;
    auto workerLoop = EventLoop::createEventLoopOnNewThread([&done, &cv] { //
        std::cout << "EventLoop worker started " << std::this_thread::get_id() << std::endl;
        std::cout << "EventLoop worker task done " << std::this_thread::get_id() << std::endl;
        done = true;
        cv.notify_all();
    });
    cv.wait(lock, [&done]
            { return done; });
    lock.unlock();
    std::cout << "EventLoop master notices that worker task was completed " << std::this_thread::get_id() << std::endl;

    // if you want to post task to other event loop, please use [EventLoop::callSoonThreadSafe]
    // [EventLoop::callSoonThreadSafe] is async that it will immediately return
    // Task will exec as soon as possible but not now
    workerLoop->callSoonThreadSafe([&mutex] { //
        std::unique_lock<std::mutex> lk(mutex);
        std::cout << "EventLoop worker task call from [callSoonThreadSafe] " << std::this_thread::get_id() << std::endl;
    });

    {
        std::unique_lock<std::mutex> lk(mutex);
        std::cout << "EventLoop master closing worker loop " << std::this_thread::get_id() << std::endl;
    }

    // [EventLoop::createEventLoopOnNewThread] will build a never stop event loop until you close it from other thread
    // Thread-eventloop will be closed when [Event::close] was called and all the eventloop handles are cleared
    workerLoop->close(); // If any actively handles in event loop, [EventLoop::close] will block forever
    std::cout << "EventLoop master closed worker loop " << std::this_thread::get_id() << std::endl;
}