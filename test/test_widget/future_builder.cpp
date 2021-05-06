#define ASYNC_RUNTIME_DISABLE_CONSOLE
#include "async_runtime.h"

class MyWidget : public StatefulWidget
{
public:
    MyWidget() {}
    ref<State<>> createState() override;
};

class _MyWidgetState : public State<MyWidget>
{
    using super = State<MyWidget>;
    lateref<Completer<int>> _completer;

    void initState() override
    {
        super::initState();
        auto self = self();
        _completer = Object::create<Completer<int>>(this);
        _completer->future->than([self, this] {
            Future<void>::delay(this, Duration(1000), [self, this] {
                Process::of(context)->exit();
            });
        });
        auto timer = Timer::delay(this, Duration(2000), [self, this] {
            _completer->complete(2);
        });
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return Object::create<FutureBuilder<int>>(
            _completer->future,
            [this](ref<BuildContext>, ref<AsyncSnapshot<int>> snapshot) {
                LogInfo(AsyncSnapshot<>::ConnectionState::toString(snapshot->state));
                if (snapshot->hasData())
                {
                    LogInfo(snapshot->data);
                }
                return LeafWidget::factory();
            });
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