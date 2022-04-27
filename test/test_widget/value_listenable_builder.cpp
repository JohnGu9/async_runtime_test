#include "async_runtime.h"

struct MyWidget : StatefulWidget
{
    ref<State<>> createState() override;
};

struct _MyWidgetState : State<MyWidget>
{
    using super = ::State<MyWidget>;
    lateref<Timer> _timer;
    lateref<ValueNotifier<bool>> _notifier;

    void initState() override
    {
        super::initState();
        _notifier = Object::create<ValueNotifier<bool>>(false);
        _timer = Timer::periodic(
            Duration(2000), [this](ref<Timer>)
            {
                _notifier->setValue(!_notifier->value);
                if (_notifier->getValue() == false)
                {
                    std::cout << "Request Exit" << std::endl;
                    RootWidget::of(context)->exit();
                } });
        _timer->start();
    }

    void dispose() override
    {
        _timer->cancel();
        super::dispose();
    }

    Function<ref<Widget>(ref<BuildContext>, bool, option<Widget>)> builder =
        [](ref<BuildContext> _, bool value, option<Widget> child)
    {
        std::cout << "current value: " << value << std::endl;
        return LeafWidget::factory();
    };

    ref<Widget> build(ref<BuildContext>) override
    {
        return Object::create<ValueListenableBuilder<bool>>(
            _notifier, builder);
    }
};

ref<State<>> MyWidget::createState() { return Object::create<_MyWidgetState>(); }

int main()
{
    runApp(Object::create<MyWidget>());
    return 0;
}