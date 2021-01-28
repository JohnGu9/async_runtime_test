#include "async_runtime.h"

class MyWidget : public StatefulWidget
{
    Object::Ref<State<>> createState() override;
};

class _MyWidgetState : public State<MyWidget>
{
    using super = State<MyWidget>;

    static Object::Ref<Widget> onChildBuild(Object::Ref<BuildContext> context)
    {
        debug_print("onChildBuild");
        return LeafWidget::factory();
    };

    List<Object::Ref<Widget>> children;

    void initState() override
    {
        super::initState();
        this->children = {
            Object::create<Builder>(onChildBuild),
            Object::create<Builder>(onChildBuild)};
    }

    void dispose() override
    {
        this->children.clear();
        super::dispose();
    }

    void addChild()
    {
        auto self = Object::cast<>(this);
        this->setState([self] { self->children.push_back(Object::create<Builder>(onChildBuild)); });
    }

    Object::Ref<Widget> build(Object::Ref<BuildContext> context) override
    {
        return Object::create<MultiChildWidget>(this->children);
    }
};

inline Object::Ref<State<>> MyWidget::createState()
{
    return Object::create<_MyWidgetState>();
}

int main()
{
    runApp(Object::create<MyWidget>());
    return EXIT_SUCCESS;
}