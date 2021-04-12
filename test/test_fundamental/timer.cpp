#include "async_runtime.h"

class MainActivity : public StatefulWidget
{
    virtual ref<State<StatefulWidget>> createState() override;
};

class _MainActivityState : public State<MainActivity>
{
    using super = State<MainActivity>;
    lateref<Timer> _timer;
    size_t _count = 0;

    void initState() override
    {
        super::initState();
        auto self = self();
        _timer = Timer::periodic(
            this, Duration(1000), [self] {
                if (self->mounted)
                    Logger::of(self->context)->writeLine("Timer callback");
                if (++self->_count > 5)
                    Process::of(self->context)->exit();
            });
    }

    void didWidgetUpdated(ref<StatefulWidget> oldWidget) override
    {
        Logger::of(context)->writeLine("_MainActivityState::didWidgetUpdated");
        super::didWidgetUpdated(oldWidget);
    }

    void didDependenceChanged() override
    {
        Scheduler::Handler handler = Scheduler::of(context);
        Logger::of(context)->writeLine("_MainActivityState::didDependenceChanged");
        Logger::of(context)->writeLine("Current scheduler handler is " + handler->runtimeType());
        super::didDependenceChanged();
    }

    void dispose() override
    {
        Logger::of(context)->writeLine("_MainActivityState::dispose");
        _timer->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext> context) override
    {
        Logger::of(context)->writeLine("_MainActivityState::build");
        return LeafWidget::factory();
    }
};

inline ref<State<StatefulWidget>> MainActivity::createState()
{
    return Object::create<_MainActivityState>();
}

int main()
{
    runApp(Object::create<MainActivity>());
    return EXIT_SUCCESS;
}