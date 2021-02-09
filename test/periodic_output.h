#pragma once

#include "async_runtime.h"

class PeriodicOutput : public StatefulWidget
{
public:
    PeriodicOutput(Object::Ref<Widget> child, Logger::Handler handler, Object::Ref<Key> key = nullptr)
        : child(child), handler(handler), StatefulWidget(key) {}

    Object::Ref<Widget> child;
    Logger::Handler handler;
    Object::Ref<State<>> createState() override;
};

class _PeriodicOutputState : public State<PeriodicOutput>
{
    using super = State<PeriodicOutput>;
    Object::Ref<Timer> _timer;

    void initState() override
    {
        super::initState();
        auto self = Object::cast<>(this);
        debug_print("initState");
        _timer = Timer::periodic(
            this, Duration(1000), [self] {
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

    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        return this->getWidget()->child;
    }
};

inline Object::Ref<State<>> PeriodicOutput::createState()
{
    return Object::create<_PeriodicOutputState>();
}