#pragma once

#include "async_runtime.h"
#include "periodic_output.h"
#include "notification_test.h"
#include "rebuild_test.h"

class MainActivity : public StatelessWidget
{
    ref<Widget> build(ref<BuildContext> context) override
    {
        return Logger::file(
            "app.log",
            MultiChildWidget::fromChildren({
                Object::create<PeriodicOutput>(LeafWidget::factory(), Logger::of(context)), // PeriodicOutput
                Object::create<NotificationTest>(),                                         // NotificationTest
                Logger::file("rebuild_test.log", Object::create<RebuildTest>()),            // RebuildTest
            })                                                                              // MultiChildWidget
        );                                                                                  // Logger
    }
};
