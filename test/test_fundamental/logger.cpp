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
        _timer = Timer::periodic(self(), Duration::fromMilliseconds(1000), [this]
                                 {
                                     if (mounted)
                                     {
                                         if (++_count > 5)
                                             Process::of(context)->exit();
                                         else
                                         {
                                             LogInfo("Timer::periodic callback with counter {}! [printf style]", _count);
                                             LogInfo("Timer::periodic callback with counter " << _count << "! [istream style]");
                                         }
                                     }
                                 });
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

inline ref<State<>> LoggerTest::createState()
{
    return Object::create<_LoggerTestState>();
}

class LoggerChangeWidget : public StatefulWidget
{
    ref<State<>> createState() override;

public:
    LoggerChangeWidget(ref<Widget> child, option<Key> key = nullptr) : StatefulWidget(key), child(child) {}
    finalref<Widget> child;
};

class _LoggerChangeWidgetState : public State<LoggerChangeWidget>
{

    class NewLoggerHandler : public LoggerHandler
    {
        weakref<LoggerHandler> parent;
        ref<Future<bool>> write(ref<String> str) override
        {
            auto loggerFromContext = parent.assertNotNull();
            loggerFromContext->write("NewLoggerHandler catch new log: ");
            return loggerFromContext->writeLine(str);
        }

        ref<Future<bool>> writeLine(ref<String> str) override
        {
            auto loggerFromContext = parent.assertNotNull();
            loggerFromContext->write("NewLoggerHandler catch new log: ");
            return loggerFromContext->writeLine(str);
        }

        void dispose() override {}

    public:
        NewLoggerHandler(ref<BuildContext> context) : parent(Logger::of(context)) {}
    };

    using super = State<LoggerChangeWidget>;
    lateref<Timer> _timer;
    bool _newLoggerHandler = false;

    void initState() override
    {
        super::initState();
        _newLoggerHandler = false;
        _timer = Timer::delay(self(), Duration::fromSeconds(2), [this]
                              { setState([this]
                                         { _newLoggerHandler = true; /* change the logger handle 2 second after this state init */ }); });
    }

    void dispose() override
    {
        _timer->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext> context) override
    {
        return Object::create<Logger>(widget->child,
                                      _newLoggerHandler ? Object::create<NewLoggerHandler>(context) : Logger::of(context));
    }
};

inline ref<State<>> LoggerChangeWidget::createState()
{
    return Object::create<_LoggerChangeWidgetState>();
};

int main()
{
    return runApp(/* root */ Object::create<LoggerChangeWidget>(
        /* child */ Object::create<LoggerTest>()));
}