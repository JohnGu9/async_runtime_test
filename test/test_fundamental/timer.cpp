#include "async_runtime/fundamental/timer.h"
#include <iostream>

void task();
int main()
{
    EventLoop::run(task);
    return 0;
}

void task()
{
    // timer has event loop handle that event loop would not close before all timers be canceled
    auto timer0 = Timer::periodic(1000, [](const ref<Timer> &)
                                  { std::cout << "timer0 " << std::endl; });
    timer0->start();
    Timer::delay(10000, [timer0](const ref<Timer> &)
                 { 
                timer0->cancel();
                std::cout << "timer1 " << std::endl; })
        ->start();
}