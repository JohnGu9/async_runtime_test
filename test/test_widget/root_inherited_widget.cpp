#include "async_runtime.h"

struct MyWidget : StatefulWidget
{
    ref<State<>> createState() override;
};

struct _MyWidgetState : State<MyWidget>
{
    using super = ::State<MyWidget>;
    lateref<Timer> _timer;

    void _requestExit()
    {
        if (this->mounted)
        {
            Logger::of(context)->writeLine("MyWidget request to exit");
            RootInheritedWidget::of(context)->exit();
        }
    }

    void initState() override
    {
        super::initState();
        auto self = self();
        _timer = Timer::delay(
            this, Duration(2000), [self] { self->_requestExit(); });
    }

    void dispose() override
    {
        _timer->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

ref<State<>> MyWidget::createState() { return Object::create<_MyWidgetState>(); }

int main()
{
    runApp(Object::create<MyWidget>());
    return EXIT_SUCCESS;
}