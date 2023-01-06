#include "async_runtime/fundamental/async_thread_pool.h"
#include "async_runtime/fundamental/async.h"

static void task();
int main()
{
    EventLoop::run(task);
    return 0;
}

static void task()
{
    auto pool = AsyncThreadPool::single();
    Future<int>::delay(Duration(1000), 0)
        ->then<int>([pool] {  //
            pool->launch([] { //
                std::cout << "launch0" << std::endl;
            });
            return pool->withContext<int>([] { //
                std::cout << "withContext0" << std::endl;
                return 0;
            });
        })
        ->then<int>([] { //
            return Future<int>::delay(Duration(1000), 0);
        })
        ->then<int>([pool] {  //
            pool->launch([] { //
                std::cout << "launch1" << std::endl;
            });
            return pool->withContext<int>([] { //
                std::cout << "withContext1" << std::endl;
                return 0;
            });
        })
        ->then<int>([pool] { //
            pool->dispose();
            return 0;
        });
}
