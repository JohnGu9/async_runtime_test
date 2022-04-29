#include "async_runtime.h"

struct Child : StatefulWidget
{
    Child(ref<String> name) : name(name) {}
    finalref<String> name;
    ref<State<>> createState() override;
};

struct _ChildState : State<Child>
{
    using super = State<Child>;

    void dispose() override
    {
        LogInfo("dispose " << this->widget->name);
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
    lateref<Map<ref<String>, lateref<Widget>>> _children;
    lateref<Timer> _timer;

    void initState() override
    {
        super::initState();
        _children = {
            {"child0", Object::create<Child>("child0")},
            {"child1", Object::create<Child>("child1")},
            {"child2", Object::create<Child>("child2")},
        };
        _count = _children->size();
        _timer = Timer::periodic(Duration(1000), [this](ref<Timer>) //
                                 {                                  //
                                     if (_count > 10)
                                     {
                                         RootWidget::of(context)->exit();
                                         return;
                                     }
                                     setState([this] //
                                              {      //
                                                  static finalref<String> name = "child";
                                                  ++_count;
                                                  auto iter = _children->find(name + (_count - 3));
                                                  if (iter != _children->end())
                                                      _children->erase(iter);
                                                  else
                                                      LogInfo("Can't find: " << (name + (_count - 3)));
                                                  _children[name + _count] = Object::create<Child>(name + _count);
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
    return 0;
}