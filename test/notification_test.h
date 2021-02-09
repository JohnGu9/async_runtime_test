#pragma once

#include "async_runtime.h"

class MessageNotification : public Notification
{
public:
    static Object::Ref<MessageNotification> fromString(String message)
    {
        return Object::create<MessageNotification>(message);
    }
    MessageNotification(String message) : message(message) {}
    String message;
};

class NotificationTest : public StatelessWidget
{
public:
    NotificationTest(Object::Ref<Key> key = nullptr) : StatelessWidget(key) {}
    Object::Ref<Widget> build(Object::Ref<BuildContext> context) override;
};

class _BubbleNotification : public StatefulWidget
{
public:
    _BubbleNotification(Object::Ref<Key> key = nullptr) : StatefulWidget(key) {}
    Object::Ref<State<>> createState() override;
};

class __BubbleNotificationState : public State<_BubbleNotification>
{
    using super = State<_BubbleNotification>;
    Object::Ref<Timer> _timer;

    void initState() override
    {
        super::initState();
        auto self = Object::cast<>(this);
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

    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

inline Object::Ref<State<>> _BubbleNotification::createState()
{
    return Object::create<__BubbleNotificationState>();
}

inline Object::Ref<Widget> NotificationTest::build(Object::Ref<BuildContext> context)
{
    return Object::create<NotificationListener>(
        Object::create<_BubbleNotification>(),
        [context](Object::Ref<Notification> notification) {
            if (Object::Ref<MessageNotification> messageNotification = notification->cast<MessageNotification>())
            {
                Logger::of(context)->writeLine(messageNotification->message);
                return true;
            }
            return false;
        });
}
