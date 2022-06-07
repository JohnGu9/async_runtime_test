#pragma once
#include "async_runtime.h"

class LifecycleNotifyWidget : public StatefulWidget
{
public:
    LifecycleNotifyWidget(ref<Key> key) : StatefulWidget(key) {}
    ref<State<>> createState() override;
};

class _LifecycleNotifyWidgetState : public State<LifecycleNotifyWidget>
{
    using super = State<LifecycleNotifyWidget>;

public:
    void initState() override
    {
        super::initState();
        std::cout << this->widget << " " << self() << " initState" << std::endl;
    }

    void didWidgetUpdated(ref<LifecycleNotifyWidget> oldWidget) override
    {
        super::didWidgetUpdated(oldWidget);
        std::cout << this->widget << " " << self() << " didWidgetUpdated" << std::endl;
    }

    void dispose() override
    {
        std::cout << this->widget << " " << self() << " dispose" << std::endl;
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        std::cout << this->widget << " " << self() << " build" << std::endl;
        return LeafWidget::factory();
    }
};

ref<State<>> LifecycleNotifyWidget::createState()
{
    return Object::create<_LifecycleNotifyWidgetState>();
}
