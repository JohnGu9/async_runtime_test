#include "async_runtime.h"
#include <iostream>

class Child : public StatefulWidget
{
    ref<State<>> createState() override;

public:
    Child(int count_) : count(count_) {}
    const int count;
};

class _ChildState : public State<Child>
{
    using super = State<Child>;

    void didWidgetUpdated(ref<Child> oldWidget) override
    {
        super::didWidgetUpdated(oldWidget);
        std::cout << "Child count change from [" << oldWidget->count << "] to [" << widget->count << "]" << std::endl;
    }

    ref<Widget> build(ref<BuildContext> context) override
    {
        std::cout << "Child count is " << widget->count << std::endl;
        return LeafWidget::factory();
    }
};

inline ref<State<>> Child::createState()
{
    return Object::create<_ChildState>();
}

class MyWidget : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _MyWidgetState : public State<MyWidget>
{
    using super = State<MyWidget>;
    int _count;
    lateref<Timer> _timer;

    void initState() override
    {
        super::initState();
        _count = 0;
        _timer = Timer::periodic(Duration(1000), [this](ref<Timer>)
                                 {
                if (!mounted)
                    return;
                if (_count > 5)
                {
                    _timer->cancel();
                    std::cout << "Timer cancel" << std::endl;

                    RootWidget::of(context)->exit();
                    return;
                } else
                    setState([this] { _count++; }); });
        _timer->start();
    }

    void dispose() override
    {
        _timer->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return Object::create<Child>(_count);
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