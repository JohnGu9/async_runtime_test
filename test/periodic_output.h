#pragma once

#include "async_runtime.h"

class PeriodicOutput : public StatefulWidget
{
public:
    PeriodicOutput(ref<Widget> child, Logger::Handler handler, option<Key> key = nullptr)
        : child(child), handler(handler), StatefulWidget(key) {}

    ref<Widget> child;
    Logger::Handler handler;
    ref<State<>> createState() override;
};

class _PeriodicOutputState : public State<PeriodicOutput>
{
    using super = State<PeriodicOutput>;
    lateref<Timer> _timer;

    void initState() override
    {
        super::initState();
        auto self = self();
        debug_print("initState");
        _timer = Timer::periodic(
            this, Duration::fromMilliseconds(1000), [self] {
                if (self->mounted)
                    self->getWidget()->handler->writeLine("Timer callback");
            });
    }

    void dispose() override
    {
        _timer->cancel();
        debug_print("dispose");
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return this->getWidget()->child;
    }
};

inline ref<State<>> PeriodicOutput::createState()
{
    return Object::create<_PeriodicOutputState>();
}