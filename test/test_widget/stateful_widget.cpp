#include "async_runtime.h"

class Child : public StatelessWidget
{
    int count;
    ref<Widget> build(ref<BuildContext>) override
    {
        debug_print("Child count is " << count);
        return LeafWidget::factory();
    }

public:
    Child(int count_) : count(count_) {}
};

class MyWidget : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _MyWidgetState : public State<MyWidget>
{
    using super = State<MyWidget>;
    int _count;
    lateref<Timer> _timer;

    void initState() override
    {
        super::initState();
        auto self = self();
        _count = 0;
        _timer = Timer::periodic(
            this, Duration(1000), [self] {
                if (!self->mounted)
                    return;
                if (self->_count > 5)
                {
                    self->_timer->cancel();
                    debug_print("Timer cancel");
                    Process::of(self->context)->exit();
                    return;
                }
                self->setState([self] { self->_count++; });
            });
    }

    void dispose() override
    {
        _timer->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return Object::create<Child>(_count);
    }
};

ref<State<>> MyWidget::createState()
{
    return Object::create<_MyWidgetState>();
}

int main()
{
    return runApp(Object::create<MyWidget>());
}