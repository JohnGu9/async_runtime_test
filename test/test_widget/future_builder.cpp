#include "async_runtime.h"

class MyWidget : public StatefulWidget
{
public:
    MyWidget() {}
    Object::Ref<State<>> createState() override;
};

class _MyWidgetState : public State<MyWidget>
{
    using super = State<MyWidget>;
    Object::Ref<Completer<int>> _completer;

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

    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        return Object::create<FutureBuilder<int>>(
            _completer->future,
            [](Object::Ref<BuildContext>, Object::Ref<AsyncSnapshot<int>> snapshot) {
                debug_print(AsyncSnapshot<>::ConnectionState::toString(snapshot->state));
                if (snapshot->hasData())
                {
                    debug_print(snapshot->data);
                }
                return LeafWidget::factory();
            });
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