#include "async_runtime.h"

class MyWidget : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _MyWidgetState : public State<MyWidget>
{
    using super = State<MyWidget>;
    lateref<Timer> _timer;

    void initState() override
    {
        super::initState();
        _timer = Timer::periodic(Duration(1000), [this](ref<Timer>)
                                 {
                if (!mounted) // [[unlikely]]
                    return;
                Logger::of(context)->writeLine("Logger::of(context)"); // root widget wrap a default logger context that output message to console
                RootWidget::of(context)->cout->writeLine("RootWidget::of(context)->cout"); // root widget's cout is thread safe
                LogInfo("LogInfo stream like " << 123 << ' ' << "abc"); // implicitly use context's logger and format message
                LogInfo("LogInfo format like {} {}", 123 ,"abc");
                std::cout << "std::cout" << std::endl; // it's also ok to use std::cout to print in master event loop
                return; });
        _timer->start();
        Timer::delay(Duration(3000), [this](ref<Timer>)
                     { RootWidget::of(context)->exit(); })
            ->start();
    }

    void dispose() override
    {
        _timer->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

inline ref<State<>> MyWidget::createState()
{
    return Object::create<_MyWidgetState>();
}

int main()
{
    runApp(Object::create<MyWidget>());
    return 0;
}