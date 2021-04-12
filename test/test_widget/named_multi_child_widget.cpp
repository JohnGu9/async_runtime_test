#include "async_runtime.h"

struct Child : StatefulWidget
{
    Child(ref<String> name) : name(name) {}
    ref<String> name;
    ref<State<>> createState() override;
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

    ref<Widget> build(ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

inline ref<State<>> Child::createState()
{
    return Object::create<_ChildState>();
}

class MyWidget : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _MyWidgetState : public State<MyWidget>
{
    using super = State<MyWidget>;

    size_t _count;
    Map<ref<String>, lateref<Widget>> _children;
    lateref<Timer> _timer;

    void initState() override
    {
        super::initState();
        auto self = self();
        _children = {
            {"child0", Object::create<Child>("child0")},
            {"child1", Object::create<Child>("child1")},
            {"child2", Object::create<Child>("child2")},
        };
        _count = _children->size();
        _timer = Timer::periodic(this, Duration(1000), [self] {
            if (self->_count > 10)
            {
                Process::of(self->context)->exit();
                return;
            }
            self->setState([self] {
                static finalref<String> name = "child";
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

    ref<Widget> build(ref<BuildContext>) override
    {
        return NamedMultiChildWidget::fromChildren(this->_children);
    }
};

inline ref<State<>> MyWidget::createState()
{
    return Object::create<_MyWidgetState>();
}

int main()
{
    runApp(Object::create<MyWidget>());
    return EXIT_SUCCESS;
}