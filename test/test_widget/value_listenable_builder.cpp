#include "async_runtime.h"

struct MyWidget : StatefulWidget
{
    Object::Ref<State<StatefulWidget>> createState() override;
};

struct _MyWidgetState : State<MyWidget>
{
    using super = ::State<MyWidget>;
    Object::Ref<Timer> _timer;
    Object::Ref<ValueNotifier<bool>> _notifier;

    void initState() override
    {
        super::initState();
        auto self = Object::cast<>(this);
        _notifier = Object::create<ValueNotifier<bool>>(false);
        _timer = Timer::periodic(
            this,
            [self] {
                self->_notifier->setValue(!self->_notifier->getValue());
                if (self->_notifier->getValue() == false)
                {
                    debug_print("Request Exit");
                    RootInheritedWidget::of(self->getContext())->requestExit();
                }
            },
            Duration(2000));
    }

    void dispose() override
    {
        _timer->dispose();
        super::dispose();
    }

    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        return Object::create<ValueListenableBuilder<bool>>(
            [](Object::Ref<BuildContext> context, bool value, Object::Ref<Widget> child) {
                debug_print("current value: " << value);
                return LeafWidget::factory();
            },
            _notifier);
    }
};

Object::Ref<State<StatefulWidget>> MyWidget::createState() { return Object::create<_MyWidgetState>(); }

int main()
{
    runApp(Object::create<MyWidget>());
    return EXIT_SUCCESS;
}