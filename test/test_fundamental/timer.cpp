#define ASYNC_RUNTIME_DISABLE_CONSOLE
#include "async_runtime.h"

class MainActivity : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _MainActivityState : public State<MainActivity>
{
    using super = State<MainActivity>;
    lateref<Timer> _timer;
    size_t _count = 0;

    void initState() override
    {
        super::initState();
        auto self = self();
        _timer = Timer::periodic(/* state */ self,
                                 /* duration */ Duration::fromMilliseconds(1000),
                                 /* callback */ [this]
                                 {
                                     if (mounted)
                                     {
                                         if (++_count > 5)
                                             Process::of(context)->exit();
                                         else
                                             LogInfo("Timer::periodic callback"); // will be canceled so that original _timer only be called twice
                                     }
                                 }); // async runtime's timer is async that not block the thread

        // you can hold no timer reference that you do not want to cancel it.
        // but be careful for the state life cycle.
        // check the [mounted] flag making sure state is alive.
        // if state is not alive, you can't access the context.
        Timer::delay(/* state */ self,
                     /* duration */ Duration::fromMilliseconds(2000),
                     /* callback */ [this, self
                                     /* this timer is out of control of the state, so add a ref of this state to make sure the state resource not release until the function called */]
                     {
                         if (mounted /*check State::mounted is not necessary because timer will always cancel when the state dispose in this case*/)
                         {
                             LogInfo("Timer::delay callback");

                             _timer->cancel(); // canceled so that original _timer only be called twice
                             _count = 0;
                             _timer = Timer::periodic(self(), Duration(1000), [this]
                                                      {
                                                          if (++_count > 4)
                                                              Process::of(context)->exit();
                                                          else
                                                              LogInfo("Timer::periodic new callback"); // new _timer will be called four times
                                                      });
                         }
                     });
    }

    void dispose() override
    {
        LogInfo(__FUNCTION__);
        // remember release the timer resource when state dispose
        // if not cancel the timer, will cause runtime error and memory leak
        _timer->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext> context) override
    {
        LogInfo(__FUNCTION__);
        return LeafWidget::factory();
    }
};

inline ref<State<StatefulWidget>> MainActivity::createState()
{
    return Object::create<_MainActivityState>();
}

int main()
{
    return runApp(Object::create<MainActivity>());
}