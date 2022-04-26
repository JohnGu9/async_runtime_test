#include "async_runtime/fundamental/async.h"
#include <iostream>

void task();
int main()
{
    EventLoop::run(task);
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
    std::cout << "Future Testing" << std::endl;
    auto handle = EventLoop::Handle::create();
    auto completer = Object::create<Completer<int>>();
    completer->then<int>([handle](const int &)
                         { 
                             std::cout << "completer callback" << std::endl;
                             handle->dispose();
                             return 0; });
    delay<int>(5000, [completer]
               { 
                   std::cout << "delay future callback" << std::endl;
                   completer->resolve(0);
                   return 0; })
        ->timeout(3000, []
                  { 
                      std::cout << "future timeout callback" << std::endl;
                      return 0; });
}
