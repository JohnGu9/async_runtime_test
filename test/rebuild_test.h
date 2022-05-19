#pragma once

#include "async_runtime.h"

class _ChildWidget : public StatelessWidget
{
public:
    _ChildWidget(const int count) : count(count) {}
    const int count;

    ref<Widget> build(ref<BuildContext> context) override
    {
        LogInfo("_ChildWidget::build on {}", count);
        return LeafWidget::factory();
    }
};

class RebuildTest : public StatefulWidget
{
public:
    RebuildTest(option<Key> key = nullptr) : StatefulWidget(key) {}
    ref<State<>> createState() override;
};

class _RebuildTestState : public State<RebuildTest>
{
    using super = State<RebuildTest>;
    int _count;
    lateref<Timer> _timer;

    void initState() override
    {
        super::initState();
        _count = 0;
        _timer = Timer::periodic(Duration::fromMilliseconds(1000), [this](ref<Timer>) { //
            setState([this] {                                                           //
                _count++;
                if (_count > 5)
                {
                    _timer->cancel();
                    LogInfo("RebuildTest timer cancel");
                }
            });
        });
        _timer->start();
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

inline ref<State<>> RebuildTest::createState()
{
    return Object::create<_RebuildTestState>();
}
