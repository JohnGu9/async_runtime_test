#define ASYNC_RUNTIME_DISABLE_CONSOLE
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
            self(), Duration(2000), [this] {
                _notifier->setValue(!_notifier->value);
                if (_notifier->getValue() == false)
                {
                    LogInfo("Request Exit");
                    Process::of(context)->exit();
                }
            });
    }

    void dispose() override
    {
        _timer->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return Object::create<ValueListenableBuilder<bool>>(
            _notifier,
            [this](ref<BuildContext> _, bool value, option<Widget> child) {
                LogInfo("current value: " << value);
                return LeafWidget::factory();
            });
    }
};

ref<State<>> MyWidget::createState() { return Object::create<_MyWidgetState>(); }

int main()
{
    return runApp(Object::create<MyWidget>());
}