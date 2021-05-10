#define ASYNC_RUNTIME_DISABLE_CONSOLE
#include "async_runtime.h"

class MainActivity : public StatefulWidget
{
    ref<State<>> createState() override;
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
            this, Duration::fromMilliseconds(1000), [this, self] {
                if (++self->_count > 5)
                    Process::of(self->context)->exit();
                else if (self->mounted)
                    LogInfo("Timer::periodic callback"); // will be canceled so that original _timer only be called twice
            });                                          // async runtime's timer is async that not block the thread

        // you can hold no timer reference that you do not want to cancel it.
        // but be careful for the state life cycle.
        // check the [mounted] flag making sure state is alive.
        // if state is not alive, you can't access the context.
        Timer::delay(this, Duration::fromMilliseconds(2000), [this, self] {
            if (self->mounted)
            {
                LogInfo("Timer::delay callback");

                self->_timer->cancel(); // canceled so that original _timer only be called twice
                self->_count = 0;
                self->_timer = Timer::periodic(
                    this, Duration(1000), [this, self] {
                        if (++self->_count > 4)
                            Process::of(self->context)->exit();
                        else if (self->mounted)
                            LogInfo("Timer::periodic new callback"); // new _timer will be called four times
                    });
            }
        });
    }

    void didWidgetUpdated(ref<MainActivity> oldWidget) override
    {
        super::didWidgetUpdated(oldWidget);
        LogInfo("_MainActivityState::didWidgetUpdated");
    }

    void didDependenceChanged() override
    {
        Scheduler::Handler handler = Scheduler::of(context);
        LogInfo("_MainActivityState::didDependenceChanged");
        LogInfo("Current scheduler handler is " << handler->runtimeType());
        super::didDependenceChanged();
    }

    void dispose() override
    {
        LogInfo("_MainActivityState::dispose");
        _timer->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext> context) override
    {
        LogInfo("_MainActivityState::build");
        return LeafWidget::factory();
    }
};

inline ref<State<StatefulWidget>> MainActivity::createState()
{
    return Object::create<_MainActivityState>();
}

int main()
{
    return runApp(Object::create<MainActivity>());
}