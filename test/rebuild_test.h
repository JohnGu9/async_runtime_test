#pragma once

#include "async_runtime.h"

class RebuildTest : public StatefulWidget
{
public:
    RebuildTest(Object::Ref<Key> key = nullptr) : StatefulWidget(key) {}
    Object::Ref<State> createState() override;
};

class _ChildWidget : public StatelessWidget
{
public:
    _ChildWidget(const int &count_) : count(count_) {}
    const int count;

    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        debug_print("Child build on count: " << count);
        return LeafWidget::factory();
    }
};

class _RebuildTestState : public State<RebuildTest>
{
    using super = State<RebuildTest>;
    int _count;
    Object::Ref<Timer> _timer;

    void initState() override
    {
        super::initState();
        _count = 0;

        auto self = Object::self(this);
        _timer = Timer::periodic(
            this,
            [self] {
                if (self->getMounted())
                    self->setState([self] {
                        self->_count++;
                        if (self->_count > 5)
                        {
                            self->_timer->cancel();
                            debug_print("RebuildTest timer cancel");
                        }
                    });
            },
            Duration(1000));
    }

    void dispose() override
    {
        _timer->dispose();
        super::dispose();
    }

    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        return Object::create<_ChildWidget>(_count);
    }
};

inline Object::Ref<StatefulWidget::State> RebuildTest::createState()
{
    return Object::create<_RebuildTestState>();
}
