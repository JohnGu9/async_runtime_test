#define ASYNC_RUNTIME_DISABLE_CONSOLE
#include "async_runtime.h"

class FutureTest : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _FutureTestState : public State<FutureTest>
{

    void initState() override
    {
        Future<void>::race(self(), {
                                       Future<void>::delay(self(), Duration(2000), [=] { LogInfo("Future#0 completed"); })->timeout(Duration(1000))->than([=] { LogInfo("Future#0 timout"); }),
                                       Future<void>::delay(self(), Duration(2000), [=] { LogInfo("Future#1 completed"); }),
                                       Future<void>::delay(self(), Duration(3000), [=] { LogInfo("Future#2 completed"); }),

                                   })
            ->than([=] { LogInfo("Future::race completed"); /* the future complete with the first completed future */ })
            ->than([=] {
                Future<void>::wait(self(), {
                                               Future<void>::delay(self(), Duration(2000), [=] { LogInfo("Future#3 completed"); }),
                                               Future<void>::delay(self(), Duration(2000), [=] { LogInfo("Future#4 completed"); }),
                                               Future<void>::delay(self(), Duration(3000), [=] { LogInfo("Future#5 completed"); }),
                                           })
                    ->than([=] { LogInfo("Future::wait completed"); /* the future complete with the last completed future */ })
                    ->than([=] { Process::of(context)->exit(); });
            });
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

ref<State<>> FutureTest::createState()
{
    return Object::create<_FutureTestState>();
}

int main()
{
    return runApp(Object::create<FutureTest>());
}
