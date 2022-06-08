#include "lifecycle_notify_widget.h"

class MyWidget : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _MyWidgetState : public State<MyWidget>
{
    using super = State<MyWidget>;
    ref<GlobalKey<_LifecycleNotifyWidgetState>> key =
        Object::create<GlobalKey<_LifecycleNotifyWidgetState>>();

    option<_LifecycleNotifyWidgetState> _lastState;

    void initState() override
    {
        super::initState();
        Future<int>::delay(1000, 0)
            ->then([this] { //
                this->_lastState = key->getCurrentState();
                setState([this] { //
                    std::cout << "Get widget from GlobalKey: " << key->getCurrentWidget().assertNotNull() << std::endl;
                    std::cout << "not change GlobalKey" << std::endl;
                });
            })
            ->then<int>([]
                        { return Future<int>::delay(1000, 0); })
            ->then([this] { //
                assert(this->_lastState == key->getCurrentState());
                setState([this] { //
                    std::cout << "Get widget from GlobalKey: " << key->getCurrentWidget().assertNotNull() << std::endl;
                    std::cout << "change GlobalKey" << std::endl;
                    key = Object::create<GlobalKey<_LifecycleNotifyWidgetState>>(); // change the GlobalKey will make target widget's subtree rebuild
                });
            })
            ->then<int>([]
                        { return Future<int>::delay(1000, 0); })
            ->then([this] { //
                assert(this->_lastState != key->getCurrentState());
                std::cout << "Get widget from GlobalKey: " << key->getCurrentWidget().assertNotNull() << std::endl;
                RootWidget::of(context)->exit();
            });
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return Object::create<LifecycleNotifyWidget>(key);
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
