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

    lateref<List<ref<Widget>>> _children;
    lateref<Timer> _timer;
    int _count;

    void initState() override
    {
        super::initState();
        _children = {
            Object::create<Builder>(onChildBuild),
            Object::create<Builder>(onChildBuild)};
        _count = 0;
        _timer = Timer::periodic(self(), Duration::fromSeconds(1), [this] {
            if (++_count > 5)
            {
                _timer->cancel();
                Process::of(context)->exit();
            }
            else
            {
                addChild();
            }
        });
    }

    void dispose() override
    {
        _children->clear();
        super::dispose();
    }

    void addChild()
    {
        setState([this] { _children->emplace_back(Object::create<Builder>(onChildBuild)); });
    }

    ref<Widget> build(ref<BuildContext> context) override
    {
        return Object::create<MultiChildWidget>(this->_children);
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