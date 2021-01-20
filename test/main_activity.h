#pragma once

#include "periodic_output.h"
#include "notification_test.h"
#include "rebuild_test.h"

class MainActivity : public StatelessWidget
{
    Object::Ref<Widget> build(Object::Ref<BuildContext> context) override
    {
        // Don't declare as static when access the context!
        Object::List<Object::Ref<Widget>> children = {
            Object::create<PeriodicOutput>(LeafWidget::factory(), Logger::of(context)), // PeriodicOutput
            Object::create<NotificationTest>(),                                         // NotificationTest
            Object::create<RebuildTest>(),                                              // RebuildTest
        };
        return Object::create<LoggerBlocker>(Object::create<MultiChildWidget>(children));
    }
};
