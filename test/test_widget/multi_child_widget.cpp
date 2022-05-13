#include "async_runtime.h"

class Child : public StatefulWidget
{
public:
    Child(int id) : id(id) {}
    ref<State<>> createState() override;
    const int id;
};

class _ChildState : public State<Child>
{
    using super = State<Child>;

    void dispose() override
    {
        LogInfo("_ChildState::dispose id[{}]", widget->id);
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        LogInfo("_ChildState::build id[{}]", widget->id);
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

    lateref<List<ref<Widget>>> _children;
    lateref<Timer> _timer;
    int _count;

    void initState() override
    {
        super::initState();
        _children = {
            Object::create<Child>(0),
            Object::create<Child>(1)};
        _count = 1;
        _timer = Timer::periodic(Duration::fromSeconds(1), [this](ref<Timer>) //
                                 {                                            //
                                     if (++_count > 5)
                                     {
                                         _timer->cancel();
                                         setState([this]
                                                  { _children->pop_back(); });
                                         _timer = Timer::periodic(Duration::fromSeconds(1), [this](ref<Timer>) //
                                                                  {                                            //
                                                                      if (_children->empty())
                                                                          RootWidget::of(context)->exit();
                                                                      else
                                                                          setState([this]
                                                                                   { _children->pop_back(); });
                                                                  });
                                     }
                                     else
                                     {
                                         setState([this]
                                                  { _children->emplace_back(Object::create<Child>(_count)); });
                                     }
                                 });
        _timer->start();
    }

    void dispose() override
    {
        _timer->cancel();
        _children->clear();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext> context) override
    {
        return Object::create<MultiChildWidget>(/* children */ this->_children);
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