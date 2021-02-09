#include "async_runtime.h"

struct MyWidget : StatefulWidget
{
    Object::Ref<State<>> createState() override;
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
            this, Duration(2000), [self] {
                self->_notifier->setValue(!self->_notifier->getValue());
                if (self->_notifier->getValue() == false)
                {
                    debug_print("Request Exit");
                    RootInheritedWidget::of(self->context)->requestExit();
                }
            });
    }

    void dispose() override
    {
        _timer->cancel();
        super::dispose();
    }

    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        return Object::create<ValueListenableBuilder<bool>>(
            _notifier,
            [](Object::Ref<BuildContext> context, bool value, Object::Ref<Widget> child) {
                debug_print("current value: " << value);
                return LeafWidget::factory();
            });
    }
};

Object::Ref<State<>> MyWidget::createState() { return Object::create<_MyWidgetState>(); }

int main()
{
    runApp(Object::create<MyWidget>());
    return EXIT_SUCCESS;
}