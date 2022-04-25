#include <iostream>
#include "async_runtime/fundamental/timer.h"

void task();
int main()
{
    EventLoop::ensure();
    EventLoop::run(task);
}

void task()
{
    auto timer0 = Timer::periodic(1000, [](const ref<Timer> &)
                                  { std::cout << "timer0 " << std::endl; });
    timer0->start();
    Timer::delay(10000, [timer0](const ref<Timer> &)
                 { 
                timer0->cancel();
                std::cout << "timer1 " << std::endl; })
        ->start();
}