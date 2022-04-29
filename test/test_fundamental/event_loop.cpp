#include "async_runtime/fundamental/event_loop.h"
#include <iostream>

void task();
int main()
{
    EventLoop::run(task);
    return 0;
}

void task()
{
    std::cout << "EventLoop master started " << std::endl;
    bool done = false;
    std::mutex mutex;
    std::condition_variable cv;
    std::unique_lock<std::mutex> lock(mutex);
    auto workerLoop = EventLoop::createEventLoopOnNewThread([&done, &cv] //
                                                            {            //
                                                                std::cout << "EventLoop worker started" << std::endl;
                                                                done = true;
                                                                std::cout << "EventLoop worker job done" << std::endl;
                                                                cv.notify_all();
                                                            });
    cv.wait(lock, [&done]
            { return done; });
    std::cout << "EventLoop master close worker loop" << std::endl;
    workerLoop->close();
}