#define ASYNC_RUNTIME_DISABLE_CONSOLE
#include "async_runtime.h"

class MyWidget : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _MyWidgetState : public State<MyWidget>
{
    using super = State<MyWidget>;

    static ref<Widget> onChildBuild(ref<BuildContext> context)
    {
        LogInfo("onChildBuild");
        return LeafWidget::factory();
    };

    List<ref<Widget>> children;

    void initState() override
    {
        super::initState();
        this->children = {
            Object::create<Builder>(onChildBuild),
            Object::create<Builder>(onChildBuild)};
    }

    void dispose() override
    {
        this->children = nullptr;
        super::dispose();
    }

    void addChild()
    {
        auto self = self();
        this->setState([self] { self->children->emplace_back(Object::create<Builder>(onChildBuild)); });
    }

    ref<Widget> build(ref<BuildContext> context) override
    {
        return Object::create<MultiChildWidget>(this->children);
    }
};

inline ref<State<>> MyWidget::createState()
{
    return Object::create<_MyWidgetState>();
}

int main()
{
    return runApp(Object::create<MyWidget>());
}