#pragma once
#include "async_runtime.h"

class Child : public StatefulWidget
{
public:
    Child(ref<Key> key) : StatefulWidget(key) {}
    ref<State<>> createState() override;
};

class _ChildState : public State<Child>
{
    using super = State<Child>;

    void initState() override
    {
        super::initState();
        std::cout << "Child mounted" << std::endl;
    }

    void dispose() override
    {
        std::cout << "Child unmount" << std::endl;
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

ref<State<>> Child::createState()
{
    return Object::create<_ChildState>();
}
