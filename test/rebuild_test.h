#pragma once

#include "async_runtime.h"

class _ChildWidget : public StatelessWidget
{
public:
    _ChildWidget(const int count) : count(count) {}
    const int count;

    ref<Widget> build(ref<BuildContext> context) override
    {
        Logger::of(context)->writeLine("_ChildWidget::build on " + std::to_string(count));
        return LeafWidget::factory();
    }
};

class RebuildTest : public StatefulWidget
{
public:
    RebuildTest(option<Key> key = nullptr) : StatefulWidget(key) {}
    ref<State<> > createState() override;
};

class _RebuildTestState : public State<RebuildTest>
{
    using super = State<RebuildTest>;
    int _count;
    lateref<Timer> _timer;

    void initState() override
    {
        super::initState();
        auto self = Object::cast<>(this);
        _count = 0;
        _timer = Timer::periodic(this, Duration(1000), [self] {
            if (self->mounted)
                self->setState([self] {
                    self->_count++;
                    if (self->_count > 5)
                    {
                        self->_timer->cancel();
                        Logger::of(self->context)->writeLine("RebuildTest timer cancel");
                    }
                });
        });
    }

    void dispose() override
    {
        _timer->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return Object::create<_ChildWidget>(_count);
    }
};

inline ref<State<> > RebuildTest::createState()
{
    return Object::create<_RebuildTestState>();
}
