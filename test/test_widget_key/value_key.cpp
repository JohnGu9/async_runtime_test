#include "lifecycle_notify_widget.h"

class MyWidget : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _MyWidgetState : public State<MyWidget>
{
    using super = State<MyWidget>;
    ref<String> text = "text";

    void initState() override
    {
        super::initState();
        Future<int>::delay(1000, 0)
            ->then([this] {       //
                setState([this] { //
                    std::cout << "not change ValueKey's value" << std::endl;
                    text = "text"; // not change value, just only update widget without rebuild the subtree
                });
            })
            ->then<int>([]
                        { return Future<int>::delay(1000, 0); })
            ->then([this] {       //
                setState([this] { //
                    std::cout << "change ValueKey's value" << std::endl;
                    text = "new text"; // change the ValueKey's value to force system rebuild the subtree and mount a new _ChildState
                });
            })
            ->then<int>([]
                        { return Future<int>::delay(1000, 0); })
            ->then([this]
                   { exitApp(context); });
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return Object::create<LifecycleNotifyWidget>(Object::create<ValueKey<ref<String>>>(text));
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
