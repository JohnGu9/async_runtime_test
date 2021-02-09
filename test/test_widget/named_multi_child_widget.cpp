#include "async_runtime.h"

struct Child : StatefulWidget
{
    Child(String name) : name(name) {}
    String name;
    Object::Ref<State<>> createState() override;
};

struct _ChildState : State<Child>
{
    using super = State<Child>;

    void initState() override
    {
        super::initState();
        debug_print("initState " << this->getWidget()->name);
    }

    void dispose() override
    {
        debug_print("dispose " << this->getWidget()->name);
        super::dispose();
    }

    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

inline Object::Ref<State<>> Child::createState()
{
    return Object::create<_ChildState>();
}

class MyWidget : public StatefulWidget
{
    Object::Ref<State<>> createState() override;
};

class _MyWidgetState : public State<MyWidget>
{
    using super = State<MyWidget>;

    int _count;
    Map<String, Object::Ref<Widget>> _children;
    Object::Ref<Timer> _timer;

    void initState() override
    {
        super::initState();
        auto self = Object::cast<>(this);
        _children = {
            {"child0", Object::create<Child>("child0")},
            {"child1", Object::create<Child>("child1")},
            {"child2", Object::create<Child>("child2")},
        };
        _count = _children->size();
        _timer = Timer::periodic(this, Duration(1000), [self] {
            if (self->_count > 10)
            {
                RootInheritedWidget::of(self->context)->requestExit();
                return;
            }
            self->setState([self] {
                static const String name = "child";
                ++self->_count;
                auto iter = self->_children->find(name + (self->_count - 3));
                if (iter != self->_children->end())
                    self->_children->erase(iter);
                else
                    debug_print("Can't find: " << (name + (self->_count - 3)));
                self->_children[name + self->_count] = Object::create<Child>(name + self->_count);
            });
        });
    }

    void dispose() override
    {
        _timer->cancel();
        super::dispose();
    }

    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        return NamedMultiChildWidget::fromChildren(this->_children);
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