#include "async_runtime.h"
#include <assert.h>

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
        _completer = Object::create<Completer<int>>();
        _completer->then<int>([this] {                         //
            return Future<int>::delay(Duration(1000), [this] { //
                RootWidget::of(context)->exit();
                return 0;
            });
        });
        Timer::delay(Duration(2000), [this]
                     { _completer->complete(123); })
            ->start();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return Object::create<FutureBuilder<int>>(
            _completer,
            [](ref<BuildContext>, ref<AsyncSnapshot<int>> snapshot) { //
                assert(snapshot->state != AsyncSnapshot<>::ConnectionState::none);
                std::cout << AsyncSnapshot<>::ConnectionState::toString(snapshot->state) << ' ';
                if (snapshot->hasData())
                {
                    assert(snapshot->state == AsyncSnapshot<>::ConnectionState::done);
                    assert(snapshot->data() == 123);
                    std::cout << snapshot->data() << std::endl;
                }
                else
                {
                    std::cout << std::endl;
                }
                return LeafWidget::factory();
            });
    }
};

inline ref<State<>> MyWidget::createState()
{
    return Object::create<_MyWidgetState>();
}

int main()
{
    runApp(Object::create<MyWidget>());
    return 0;
}
