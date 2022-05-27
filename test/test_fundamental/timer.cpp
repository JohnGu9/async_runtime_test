#include "async_runtime/fundamental/timer.h"
#include <ctime>
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
    auto start = time(nullptr);

    // Timer::periodic
    auto timer0 = Timer::periodic(1000, []
                                  { std::cout << "timer0 " << std::endl; });
    timer0->start();

    // Timer::delay
    Timer::delay(10000, [start, timer0] { // after 10s, cancel the periodic timer
        auto now = time(nullptr);
        assert(now - start == 10); // delay 10s
        timer0->cancel();
        std::cout << "timer1 " << std::endl;
    })
        ->start();
}
