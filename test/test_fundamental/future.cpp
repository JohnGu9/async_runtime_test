#include "async_runtime/fundamental/async.h"
#include <iostream>

void task();
int main()
{
    EventLoop::run(task);
    return 0;
}

int onTimeout()
{
    std::cout << "Future timeout" << std::endl;
    return 0;
}

int timeoutCallback(const int &)
{
    std::cout << "Future::timeout callback" << std::endl;
    return 0;
}

void task()
{
    // handle tell the event loop not to close before it was disposed
    auto handle = EventLoop::Handle::create();

    // future is not handle, the callback will be missing call when event loop close before future complete
    auto completer = Object::create<Completer<int>>();
    completer->then<int>([handle](const int &) //
                         {                     //
                             std::cout << "completer callback" << std::endl;
                             // dispose handle
                             handle->dispose();
                             // now event loop has no alive handle that loop will be closed as soon as possible
                             return 0;
                         });
    Future<int>::delay(5000, [completer] //
                       {                 //
                           std::cout << "delay future callback" << std::endl;
                           completer->complete(0);
                           return 0;
                       })
        ->timeout(3000, [] //
                  {        //
                      std::cout << "future timeout callback" << std::endl;
                      return 0;
                  });
}
