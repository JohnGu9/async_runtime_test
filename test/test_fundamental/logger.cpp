#define ASYNC_RUNTIME_DISABLE_CONSOLE
#define ASYNC_RUNTIME_TIMESTAMP_FORMAT "%c %Z" // custom time format
#include "async_runtime.h"

class LoggerTest : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _LoggerTestState : public State<LoggerTest>
{
    using super = State<LoggerTest>;
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
                {
                    LogInfo("Timer::periodic callback with counter {}! [printf style]", _count);
                    LogInfo("Timer::periodic callback with counter " << _count << "! [istream style]");
                }
            }); // async runtime's timer is async that not block the thread
    }

    void dispose() override
    {
        _timer->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext> context) override
    {
        return LeafWidget::factory();
    }
};

inline ref<State<StatefulWidget>> LoggerTest::createState()
{
    return Object::create<_LoggerTestState>();
}

int main()
{
    return runApp(Object::create<LoggerTest>());
}