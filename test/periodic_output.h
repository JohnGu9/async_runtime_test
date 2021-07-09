#pragma once

#include "async_runtime.h"

class PeriodicOutput : public StatefulWidget
{
public:
    PeriodicOutput(ref<Widget> child, Logger::Handler handler, option<Key> key = nullptr)
        : StatefulWidget(key), child(child), handler(handler) {}

    finalref<Widget> child;
    finalref<LoggerHandler> handler;
    ref<State<>> createState() override;
};

class _PeriodicOutputState : public State<PeriodicOutput>
{
    using super = State<PeriodicOutput>;
    lateref<Timer> _timer;

    void initState() override
    {
        super::initState();
        // by default logger macro use the logger from context
        // but here, widget explicitly require a logger
        // use the widget handle rather than use logger from context
        widget->handler->writeLine("initState");
        _timer = Timer::periodic(self(), Duration::fromMilliseconds(1000),
                                 [this]
                                 {
                                     widget->handler->writeLine("Timer callback");
                                 });
    }

    void dispose() override
    {
        _timer->cancel();
        widget->handler->writeLine("dispose");
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        widget->handler->writeLine("build");
        return this->widget->child;
    }
};

inline ref<State<>> PeriodicOutput::createState()
{
    return Object::create<_PeriodicOutputState>();
}