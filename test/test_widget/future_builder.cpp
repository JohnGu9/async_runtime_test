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
    ref<Completer<int>> _completer;

    void initState() override
    {
        super::initState();
        auto self = Object::cast<>(this);
        _completer = Object::create<Completer<int>>(this);
        _completer->future->than([self, this] {
            Future<void>::delay(this, Duration(1000), [self, this] {
                RootInheritedWidget::of(this->context)->requestExit();
            });
        });
        auto timer = Timer::delay(this, Duration(2000), [self] {
            self->_completer->complete(2);
        });
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return Object::create<FutureBuilder<int>>(
            _completer->future,
            [](ref<BuildContext>, ref<AsyncSnapshot<int>> snapshot) {
                debug_print(AsyncSnapshot<>::ConnectionState::toString(snapshot->state));
                if (snapshot->hasData())
                {
                    debug_print(snapshot->data);
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
    runApp(Object::create<MyWidget>());
    return EXIT_SUCCESS;
}