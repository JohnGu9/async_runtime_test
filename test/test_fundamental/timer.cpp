#include "async_runtime.h"

class MainActivity : public StatefulWidget
{
    virtual Object::Ref<State<StatefulWidget>> createState() override;
};

class _MainActivityState : public State<MainActivity>
{
    using super = State<MainActivity>;
    Object::Ref<Timer> _timer;

    void initState() override
    {
        super::initState();
        Object::Ref<_MainActivityState> self = Object::cast<>(this);
        this->_timer = Timer::periodic(
            this,
            [self] {
                if (self->mounted)
                    Logger::of(self->context)->writeLine("Timer callback");
            },
            Duration(1000));
    }

    void didWidgetUpdated(Object::Ref<StatefulWidget> oldWidget) override
    {
        Logger::of(this->context)->writeLine("_MainActivityState::didWidgetUpdated");
        super::didWidgetUpdated(oldWidget);
    }

    void didDependenceChanged() override
    {
        Scheduler::Handler handler = Scheduler::of(context);
        Logger::of(this->context)->writeLine("_MainActivityState::didDependenceChanged");
        Logger::of(this->context)->writeLine("Current scheduler handler is " + handler->runtimeType());
        super::didDependenceChanged();
    }

    void dispose() override
    {
        Logger::of(context)->writeLine("_MainActivityState::dispose");
        this->_timer->dispose();
        super::dispose();
    }

    Object::Ref<Widget> build(Object::Ref<BuildContext> context) override
    {
        Logger::of(context)->writeLine("_MainActivityState::build");
        return LeafWidget::factory();
    }
};

inline Object::Ref<State<StatefulWidget>> MainActivity::createState()
{
    return Object::create<_MainActivityState>();
}

int main()
{
    Object::Ref<RootElement> root = Object::create<RootElement>(Object::create<MainActivity>());
    root->attach();
    root->getStdoutHandler()->writeLine("Testing timer").get();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    root->detach();
    return EXIT_SUCCESS;
}