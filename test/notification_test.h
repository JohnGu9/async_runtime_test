#pragma once

#include "async_runtime.h"

class MessageNotification : public Notification
{
public:
    MessageNotification(String &&message_) : message(std::forward<String>(message_)) {}
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
    Object::Ref<State<StatefulWidget>> createState() override;
};

class __BubbleNotificationState : public State<_BubbleNotification>
{
    using super = State<_BubbleNotification>;
    Object::Ref<Timer> _timer;

    void initState() override
    {
        super::initState();
        auto self = Object::cast<>(this);
        _timer = Timer::periodic(
            this,
            [self] {
                if (self->getMounted())
                    Object::create<MessageNotification>("Bubble message test")->dispatch(self->getContext());
            },
            Duration(5000));
    }

    void dispose() override
    {
        _timer->dispose();
        super::dispose();
    }

    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

inline Object::Ref<State<StatefulWidget>> _BubbleNotification::createState()
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
