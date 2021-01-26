#include "async_runtime.h"

class Child : public StatelessWidget
{
    int count;
    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        debug_print("Child count is " << count);
        return LeafWidget::factory();
    }

public:
    Child(int count_) : count(count_) {}
};

class MyWidget : public StatefulWidget
{
    Object::Ref<State<>> createState() override;
};

class _MyWidgetState : public State<MyWidget>
{
    using super = State<MyWidget>;
    int _count;
    Object::Ref<Timer> _timer;

    void initState() override
    {
        super::initState();
        auto self = Object::cast<>(this);
        _count = 0;
        _timer = Timer::periodic(
            this, [self] {
                if (!self->mounted)
                    return;
                if (self->_count > 5)
                {
                    self->_timer->cancel();
                    debug_print("Timer cancel");
                    return;
                }
                self->setState([self] { self->_count++; });
            },
            Duration(1000));
    }

    void dispose() override
    {
        _timer->dispose();
        super::dispose();
    }

    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        return Object::create<Child>(_count);
    }
};

Object::Ref<State<>> MyWidget::createState()
{
    return Object::create<_MyWidgetState>();
}

int main()
{
    runApp(Object::create<MyWidget>());
    return EXIT_SUCCESS;
}