#include "async_runtime.h"
#include <assert.h>

static int thisValue = 0;

class MyInherited : public InheritedWidget
{
public:
    const int value;

    MyInherited(const int &value, ref<Widget> child, option<Key> key = nullptr) : InheritedWidget(child, key), value(value) {}
    bool updateShouldNotify(ref<InheritedWidget> oldWidget) override
    {
        return this->value != oldWidget->covariant<MyInherited>()->value;
    }
};

class Child : public StatelessWidget
{
    ref<Widget> build(ref<BuildContext> context) override
    {
        const auto &value = context->dependOnInheritedWidgetOfExactType<MyInherited>().assertNotNull()->value;
        LogInfo("MyInherited::value == " << value);
        assert(thisValue == value);
        return LeafWidget::factory();
    }
};

class P : public StatefulWidget
{
public:
    class StateImplement;
    ref<State<StatefulWidget>> createState() override;
};

class P::StateImplement : public State<P>
{
    lateref<Timer> _timer;
    using super = State<P>;

public:
    void initState() override
    {
        super::initState();
        _timer = Timer::periodic(1000, [this] { //
            if (thisValue == 2)
            {
                exitApp(context);
                return;
            }
            this->setState([] { //
                ++thisValue;
            });
        });
        _timer->start();
    }

    void dispose() override
    {
        _timer->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext> context) override
    {
        return Object::create<MyInherited>(thisValue, Object::create<Child>());
    }
};

ref<State<StatefulWidget>> P::createState() { return Object::create<P::StateImplement>(); }

int main()
{
    runApp(Object::create<P>());
    return EXIT_SUCCESS;
}
