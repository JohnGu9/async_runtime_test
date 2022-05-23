#include "async_runtime/fundamental/async.h"
#include <iostream>

void task();
int main()
{
    EventLoop::run(task);
    return 0;
}

static int counter = 0;
void task()
{
    // stream is not handle, the callback will be missing call when event loop close before stream closed
    auto controller = Object::create<StreamController<ref<String>>>();
    controller->sink("Hello");
    controller->sink("World");
    std::cout << "Sink data before listen" << std::endl;
    auto subscription = controller->listen([](const ref<String> &value)
                                           { std::cout << "Read: " << value << std::endl; });

    // but timer has handle
    // event loop close after timer cancel
    Timer::periodic(1000, [controller](ref<Timer> timer) { //
        if (++counter > 5)
        {
            timer->cancel();
            controller->close();
        }
        else
            controller->sink(String::formatFromString("{}", counter));

    })
        ->start();
}
