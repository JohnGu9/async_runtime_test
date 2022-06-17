#include "async_runtime/fundamental/async.h"
#include <assert.h>
#include <chrono>
#include <ctime>
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
static FutureOr<int> testFutureTimeout();

void task()
{
    Future<int>::value(0)
        ->then(printTime)
        ->then<int>(testFutureToEventLoop)
        ->then(printTime)
        ->then<int>(testFutureWait)
        ->then(printTime)
        ->then<int>(testFutureRace)
        ->then(printTime)
        ->then<int>(testFutureTimeout)
        ->then(printTime);
}

static void printTime()
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
#ifdef _WIN32
    char buffer[64];
    ctime_s(buffer, 64, &now);
    std::cout << buffer;
#else
    std::cout << std::ctime(&now);
#endif
}

static FutureOr<int> testFutureToEventLoop()
{
    const auto mainThreadId = std::this_thread::get_id();
    std::cout << "Current thread: " << mainThreadId << std::endl;

    // future is non handle, hold a handle tell system that still some tasks is working
    // if not holding a handle here, there is no more handle in event loop and event loop will close itself
    auto handle = EventLoop::Handle::create();
    auto worker = EventLoop::createEventLoopOnNewThread([] {});
    return Future<int>::toEventLoop([mainThreadId] { // post a task to another event loop
               const auto workerThreadId = std::this_thread::get_id();
               assert(mainThreadId != workerThreadId);
               std::cout << "Current thread: " << workerThreadId << std::endl;
               std::cout << "Doing hard job..." << std::endl;
               std::this_thread::sleep_for(std::chrono::seconds(3));
               std::cout << "Hard Job done" << std::endl;
               return 0;
           },
                                    worker)
        ->then([handle, worker] { //
            handle->dispose();    // dispose handle that event loop this task is completed
            worker->close();      // don't forget to close event loop that come from [EventLoop::createEventLoopOnNewThread]
        });
}

static FutureOr<int> testFutureWait()
{
    return Future<>::wait<>(Future<int>::delay(1000, 0), Future<char>::delay(2000, 'c'))
        ->then<int>([](ref<Future<>::Package<int, char>> package) { //
            assert(package->completed<0>());
            assert(package->completed<1>());
            std::cout << "Future::wait " << package->getValue<0>() << " " << package->getValue<1>() << std::endl;
            return 0;
        });
}

static FutureOr<int> testFutureRace()
{
    return Future<>::race<>(Future<int>::delay(1000, 10), Future<char>::delay(2000, 'a'))
        ->then<int>([](ref<Future<>::Package<int, char>> package) { //
            std::cout << "Future::race" << std::endl;
            assert(package->completed<0>());
            std::cout << "Future[0] completed with value " << package->getValue<0>() << std::endl;
            assert(!package->completed<1>());
            std::cout << "Future[1] not yet completed" << std::endl;
            return 0;
        });
}

static FutureOr<int> testFutureTimeout()
{
    auto start = time(nullptr);
    auto future = Future<int>::delay(5000, 0)
                      ->then([start] { //
                          auto now = time(nullptr);
                          assert(now - start == 5); // delay 5s
                          std::cout << "delay future callback" << std::endl;
                      })
                      ->then(printTime);

    future->timeout(3000, [] {                       // timeout api would not cancel the previous future, but just give you a new future
              std::cout << "must call" << std::endl; // wait shorted than origin future will get the value from the function
              return 1;                              // ensure a return value
          })
        ->then([start](const int &value /* 1 */) { //
            auto now = time(nullptr);
            assert(now - start == 3); // delay 3s
            assert(value == 1);
            std::cout << "future timeout [3000] callback with value " << value << std::endl;
        })
        ->then(printTime);

    future->timeout(6000, [] {                        //
              std::cout << "never call" << std::endl; // wait longer than origin future will get the value from origin future
              return 2;                               // here will never call
          })
        ->then([start](const int &value /* 0 */) { //
            auto now = time(nullptr);
            assert(now - start == 5); // delay 5s
            assert(value == 0);
            std::cout << "future timeout [6000] callback with value " << value << std::endl;
        });

    return future;
}
