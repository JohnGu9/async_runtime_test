#include "async_runtime/fundamental/async.h"
#include <iostream>

void task();
int main()
{
    EventLoop::run(task);
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

    Future<>::wait<>(Future<int>::delay(1000, 0), Future<char>::delay(2000, 'c'))
        ->then<ref<Future<>::Package<int, char>>>(
            [](ref<Future<>::Package<int, char>> package) //
            {                                             //
                std::cout << "Future::wait " << package->value << " " << package->next->value << std::endl;
                return Future<>::race<>(Future<int>::delay(1000, 10), Future<char>::delay(2000, 'a'));
            })
        ->then([](ref<Future<>::Package<int, char>> package) //
               {                                             //
                   std::cout << "Future::race" << std::endl;
                   if (package->future->completed())
                   {
                       std::cout << "Future[0] completed with value " << package->value << std::endl;
                   }
                   else
                   {
                       std::cout << "Future[0] not yet completed" << std::endl;
                   }

                   if (package->next->future->completed())
                   {
                       std::cout << "Future[1] completed with value" << package->next->value << std::endl;
                   }
                   else
                   {
                       std::cout << "Future[1] not yet completed" << std::endl;
                   }

               })
        ->then([completer]()                            //
               {                                        //
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
               });
}
