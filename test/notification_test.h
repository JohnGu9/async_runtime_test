#pragma once

#include "async_runtime.h"

class MessageNotification : public Notification
{
public:
    static ref<MessageNotification> fromString(String message)
    {
        return Object::create<MessageNotification>(message);
    }
    MessageNotification(String message) : message(message) {}
    String message;
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
        auto self = self();
        _timer = Timer::periodic(this, Duration(2000), [self] {
            if (self->mounted)
            {
                Logger::of(self->context)->writeLine("Start testing bubble message");
                MessageNotification::fromString("Bubble message test")->dispatch(self->context);
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

inline ref<State<>> _BubbleNotification::createState()
{
    return Object::create<__BubbleNotificationState>();
}

inline ref<Widget> NotificationTest::build(ref<BuildContext> context)
{
    return Object::create<NotificationListener>(
        Object::create<_BubbleNotification>(),
        [context](ref<Notification> notification) {
            lateref<MessageNotification> messageNotification;
            if (notification->cast<MessageNotification>().isNotNull(messageNotification))
            {
                Logger::of(context)->writeLine(messageNotification->message);
                return true;
            }
            return false;
        });
}
