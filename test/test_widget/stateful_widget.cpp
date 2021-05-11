#define ASYNC_RUNTIME_DISABLE_CONSOLE
#include "async_runtime.h"

class Child : public StatefulWidget
{
    ref<State<>> createState() override;

public:
    Child(int count_) : count(count_) {}
    int count;
};

class _ChildState : public State<Child>
{
    using super = State<Child>;

    void didWidgetUpdated(ref<Child> oldWidget) override
    {
        super::didWidgetUpdated(oldWidget);
        LogInfo("Child count change from [{}] to [{}]", oldWidget->count, widget->count);
    }

    ref<Widget> build(ref<BuildContext> context) override
    {
        LogInfo("Child count is " << widget->count);
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
        _timer = Timer::periodic(
            self(), Duration(1000), [this] {
                if (!mounted)
                    return;
                if (_count > 5)
                {
                    _timer->cancel();
                    LogInfo("Timer cancel");
                    Process::of(context)->exit();
                    return;
                }
                setState([this] { _count++; });
            });
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
    return runApp(Object::create<MyWidget>());
}