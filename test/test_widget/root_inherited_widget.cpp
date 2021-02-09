#include "async_runtime.h"

struct MyWidget : StatefulWidget
{
    Object::Ref<State<>> createState() override;
};

struct _MyWidgetState : State<MyWidget>
{
    using super = ::State<MyWidget>;
    Object::Ref<Timer> _timer;

    void _requestExit()
    {
        if (this->mounted)
        {
            Logger::of(context)->writeLine("MyWidget request to exit");
            RootInheritedWidget::of(context)->requestExit();
        }
    }

    void initState() override
    {
        super::initState();
        auto self = Object::cast<>(this);
        _timer = Timer::delay(
            this, Duration(2000), [self] { self->_requestExit(); });
    }

    void dispose() override
    {
        _timer->cancel();
        super::dispose();
    }

    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

Object::Ref<State<>> MyWidget::createState() { return Object::create<_MyWidgetState>(); }

int main()
{
    runApp(Object::create<MyWidget>());
    return EXIT_SUCCESS;
}