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

ref<State<>> Child::createState()
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
        auto self = self();
        _count = 0;
        _timer = Timer::periodic(
            this, Duration(1000), [this, self] {
                if (!self->mounted)
                    return;
                if (self->_count > 5)
                {
                    self->_timer->cancel();
                    LogInfo("Timer cancel");
                    Process::of(self->context)->exit();
                    return;
                }
                self->setState([self] { self->_count++; });
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

ref<State<>> MyWidget::createState()
{
    return Object::create<_MyWidgetState>();
}

int main()
{
    return runApp(Object::create<MyWidget>());
}