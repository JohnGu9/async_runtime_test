#include "async_runtime/fundamental/async.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

void task();
int main()
{
    EventLoop::run(task);
    return 0;
}

static void printTime();
static FutureOr<int> testFutureToEventLoop();
static FutureOr<int> testFutureWait();
static FutureOr<int> testFutureRace();

void task()
{
    // handle tell the event loop not to close before it was disposed
    auto handle = EventLoop::Handle::create();

    // future is not handle, the callback will be missing call when event loop close before future complete
    auto completer = Object::create<Completer<int>>();
    completer->then<int>([handle] { //
        std::cout << "completer callback" << std::endl;
        // dispose handle
        handle->dispose();
        // now event loop has no alive handle that loop will be closed as soon as possible
        return 0;
    });

    std::cout << "Current thread: " << std::this_thread::get_id() << std::endl;
    printTime();
    Future<int>::value(0)
        ->then<int>(testFutureToEventLoop)
        ->then(printTime)
        ->then<int>(testFutureWait)
        ->then(printTime)
        ->then<int>(testFutureRace)
        ->then(printTime)
        ->then([completer] { //
            auto future = Future<int>::delay(5000, 0)
                              ->then([]
                                     { std::cout << "delay future callback" << std::endl; })
                              ->then(printTime)
                              ->then([completer]
                                     { completer->complete(0); });

            future->timeout(3000, [] {                       // timeout api would not cancel the previous future, but just give you a new future
                      std::cout << "must call" << std::endl; // wait shorted than origin future will get the value from the function
                      return 1;                              // ensure a return value
                  })
                ->then([](const int &value /* 1 */)
                       { std::cout << "future timeout [3000] callback with value " << value << std::endl; })
                ->then(printTime);

            future->timeout(6000, [] {                        //
                      std::cout << "never call" << std::endl; // wait longer than origin future will get the value from origin future
                      return 2;                               // here will never call
                  })
                ->then([](const int &value /* 0 */)
                       { std::cout << "future timeout [6000] callback with value " << value << std::endl; });
        });
}

static std::time_t now;
static void printTime()
{
    now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::ctime(&now);
}

static FutureOr<int> testFutureToEventLoop()
{
    auto worker = EventLoop::createEventLoopOnNewThread([] {});
    return Future<int>::toEventLoop([] { //
               std::cout << "Current thread: " << std::this_thread::get_id() << std::endl;
               std::cout << "Doing hard job" << std::endl;
               std::this_thread::sleep_for(std::chrono::seconds(3));
               std::cout << "Hard Job done" << std::endl;
               return 0;
           },
                                    worker)
        ->then([worker]
               { worker->close(); });
}

static FutureOr<int> testFutureWait()
{
    return Future<>::wait<>(Future<int>::delay(1000, 0), Future<char>::delay(2000, 'c'))
        ->then<int>(
            [](ref<Future<>::Package<int, char>> package) { //
                std::cout << "Future::wait " << package->getValue<0>() << " " << package->getValue<1>() << std::endl;
                return 0;
            });
}

static FutureOr<int> testFutureRace()
{
    return Future<>::race<>(Future<int>::delay(1000, 10), Future<char>::delay(2000, 'a'))
        ->then<int>(
            [](ref<Future<>::Package<int, char>> package) { //
                std::cout << "Future::race" << std::endl;
                if (package->completed<0>())
                    std::cout << "Future[0] completed with value " << package->getValue<0>() << std::endl;
                else // [[never]]
                    std::cout << "Future[0] not yet completed" << std::endl;

                if (package->completed<1>()) // [[never]]
                    std::cout << "Future[1] completed with value" << package->getValue<1>() << std::endl;
                else
                    std::cout << "Future[1] not yet completed" << std::endl;

                return 0;
            });
}
