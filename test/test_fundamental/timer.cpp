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
        _timer = Timer::periodic(self(), Duration::fromMilliseconds(1000), [this] {
            if (++_count > 5)
                Process::of(context)->exit();
            else if (mounted)
                LogInfo("Timer::periodic callback"); // will be canceled so that original _timer only be called twice
        });                                          // async runtime's timer is async that not block the thread

        // you can hold no timer reference that you do not want to cancel it.
        // but be careful for the state life cycle.
        // check the [mounted] flag making sure state is alive.
        // if state is not alive, you can't access the context.
        Timer::delay(self(), Duration::fromMilliseconds(2000), [this] {
            if (mounted)
            {
                LogInfo("Timer::delay callback");

                _timer->cancel(); // canceled so that original _timer only be called twice
                _count = 0;
                _timer = Timer::periodic(
                    self(), Duration(1000), [this] {
                        if (++_count > 4)
                            Process::of(context)->exit();
                        else if (mounted)
                            LogInfo("Timer::periodic new callback"); // new _timer will be called four times
                    });
            }
        });
    }

    void didWidgetUpdated(ref<MainActivity> oldWidget) override
    {
        super::didWidgetUpdated(oldWidget);
        LogInfo(__FUNCTION__);
    }

    void didDependenceChanged() override
    {
        Scheduler::Handler handler = Scheduler::of(context);
        LogInfo(__FUNCTION__);
        LogInfo("Current scheduler handler is " << handler->runtimeType());
        super::didDependenceChanged();
    }

    void dispose() override
    {
        LogInfo(__FUNCTION__);
        _timer->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext> context) override
    {
        LogInfo(__FUNCTION__);
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