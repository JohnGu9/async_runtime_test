#pragma once

#include "async_runtime.h"

class MessageNotification : public Notification
{
public:
    static ref<MessageNotification> fromString(ref<String> message)
    {
        return Object::create<MessageNotification>(message);
    }
    MessageNotification(ref<String> message) : message(message) {}
    finalref<String> message;
};

class NotificationTest : public StatelessWidget
{
public:
    NotificationTest(option<Key> key = nullptr) : StatelessWidget(key) {}
    ref<Widget> build(ref<BuildContext> context) override;
};

class _BubbleNotification : public StatefulWidget
{
public:
    _BubbleNotification(option<Key> key = nullptr) : StatefulWidget(key) {}
    ref<State<>> createState() override;
};

class __BubbleNotificationState : public State<_BubbleNotification>
{
    using super = State<_BubbleNotification>;
    lateref<Timer> _timer;

    void initState() override
    {
        super::initState();
        _timer = Timer::periodic(self(), Duration::fromMilliseconds(1000), [this] {
            MessageNotification::fromString("Bubble message test")->dispatch(context);
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

inline ref<State<>> _BubbleNotification::createState()
{
    return Object::create<__BubbleNotificationState>();
}

inline ref<Widget> NotificationTest::build(ref<BuildContext> context)
{
    return Object::create<NotificationListener<MessageNotification>>(
        Object::create<_BubbleNotification>(),
        [context](ref<MessageNotification> notification) {
            LogInfo(notification->message);
            return true;
        });
}
