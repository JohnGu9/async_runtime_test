#include "async_runtime.h"

struct MyWidget : StatefulWidget
{
    ref<State<>> createState() override;
};

struct _MyWidgetState : State<MyWidget>
{
    using super = ::State<MyWidget>;
    lateref<Timer> _timer;

    void initState() override
    {
        super::initState();
        auto self = self();
        _timer = Timer::delay(
            this, Duration(2000), [this, self] {
                if (this->mounted)
                {
                    Logger::of(context)->writeLine("MyWidget request to exit");
                    Process::of(context)->exit(1);
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
        return LeafWidget::factory();
    }
};

ref<State<>> MyWidget::createState() { return Object::create<_MyWidgetState>(); }

int main()
{
    const auto exitCode = runApp(Object::create<MyWidget>());
    assert(exitCode == 1);
    std::cout << "exited with code " << exitCode << std::endl;
    return exitCode;
    // or directly return like this:
    // return runApp(Object::create<MyWidget>());
}