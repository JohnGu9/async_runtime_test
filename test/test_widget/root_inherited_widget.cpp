#include "async_runtime.h"

struct MyWidget : StatefulWidget
{
    Object::Ref<State<StatefulWidget>> createState() override;
};

struct _MyWidgetState : State<MyWidget>
{
    using super = ::State<MyWidget>;
    Object::Ref<Timer> _timer;

    void _requestExit()
    {
        if (this->getMounted())
        {
            Logger::of(this->getContext())->writeLine("MyWidget request to exit");
            RootInheritedWidget::of(this->getContext())->requestExit();
        }
    }

    void initState() override
    {
        super::initState();
        auto self = Object::cast<>(this);
        _timer = Timer::delay(
            this, [self] { self->_requestExit(); }, Duration(2000));
    }

    void dispose() override
    {
        _timer->dispose();
        super::dispose();
    }

    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

Object::Ref<State<StatefulWidget>> MyWidget::createState() { return Object::create<_MyWidgetState>(); }

int main()
{
    runApp(Object::create<MyWidget>());
    return EXIT_SUCCESS;
}