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

    void initState() override
    {
        super::initState();
        std::cout << this->widget << " initState" << std::endl;
    }

    void didWidgetUpdated(ref<LifecycleNotifyWidget> oldWidget) override
    {
        super::didWidgetUpdated(oldWidget);
        std::cout << this->widget << " didWidgetUpdated" << std::endl;
    }

    void dispose() override
    {
        std::cout << this->widget << " dispose" << std::endl;
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

ref<State<>> LifecycleNotifyWidget::createState()
{
    return Object::create<_LifecycleNotifyWidgetState>();
}
