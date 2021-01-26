#pragma once

#include "periodic_output.h"
#include "notification_test.h"
#include "rebuild_test.h"

class MainActivity : public StatelessWidget
{
    Object::Ref<Widget> build(Object::Ref<BuildContext> context) override
    {
        return Logger::file(
            MultiChildWidget::fromChildren({
                Object::create<PeriodicOutput>(LeafWidget::factory(), Logger::of(context)), // PeriodicOutput
                Object::create<NotificationTest>(),                                         // NotificationTest
                Object::create<RebuildTest>(),                                              // RebuildTest
            }),
            "app.log");
    }
};
