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
    lateref<StreamController<int>> _controller;
    lateref<Timer> _timer;
    int _counter = 0;

    void initState() override
    {
        super::initState();
        _controller = Object::create<StreamController<int>>();
        _timer = Timer::periodic(1000, [this] { //
            std::cout << "Sink data: " << _counter << std::endl;
            _controller->sink(_counter++);
            if (this->_counter > 5)
            {
                _timer->cancel();
                _controller->close();
                Future<int>::delay(500, [this] { //
                    RootWidget::of(context)->exit();
                    return 0;
                });
            }
        });
        _timer->start();
    }

    void dispose() override
    {
        _timer->cancel();
        _controller->close();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return Object::create<StreamBuilder<int>>(
            _controller,
            [](ref<BuildContext>, ref<AsyncSnapshot<int>> snapshot) { //
                std::cout << AsyncSnapshot<>::ConnectionState::toString(snapshot->state) << ' ';
                if (snapshot->hasData())
                    std::cout << snapshot->data() << std::endl;
                else
                    std::cout << std::endl;
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
