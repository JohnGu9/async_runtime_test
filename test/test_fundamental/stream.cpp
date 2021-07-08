#define ASYNC_RUNTIME_DISABLE_CONSOLE
#include "async_runtime.h"

class StreamTest : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _StreamTestState : public State<StreamTest>
{
    using super = State<StreamTest>;

    int _counter;
    lateref<StreamController<int>> _streamController;
    lateref<BroadcastStreamController<int>> _broadcastStreamController;
    lateref<StreamSubscription<int>> _streamSubscription;
    lateref<Timer> _timer;

    void initState() override
    {
        super::initState();
        _counter = 0;
        _streamController = Object::create<StreamController<int>>(self());
        _broadcastStreamController = Object::create<BroadcastStreamController<int>>(self());
        _streamController->stream->listen([this](const int &value)
                                          { _broadcastStreamController->sink(value); });
        _streamController->stream->onClose([this]
                                           { _broadcastStreamController->close(); });

        Timer::delay(self(), Duration(1500), [this]
                     {
                         LogInfo("Start broadcast");
                         _streamSubscription = _broadcastStreamController->stream->listen([this](const int &value)
                                                                                          { LogInfo("value: {}", value); });
                     });
        _timer = Timer::periodic(self(), Duration(500), [this]
                                 {
                                     _streamController->sink(_counter++);
                                     if (_counter >= 5)
                                         Process::of(context)->exit();
                                 });
    }

    void dispose() override
    {
        _streamController->close();
        _timer->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext> context) override
    {
        return LeafWidget::factory();
    }
};

inline ref<State<>> StreamTest::createState()
{
    return Object::create<_StreamTestState>();
}

int main()
{
    return runApp(Object::create<StreamTest>());
}