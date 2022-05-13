#pragma once

#include "async_runtime.h"
#include "notification_test.h"
#include "periodic_output.h"
#include "rebuild_test.h"

class MainActivity : public StatelessWidget
{
    ref<Widget> build(ref<BuildContext> context) override
    {
        return Logger::file(
            /* path */ "app.log", // redirect default logger to file
            /* child */ MultiChildWidget::fromChildren(/* children */ {
                Object::create<PeriodicOutput>(LeafWidget::factory(), Logger::of(context)), // PeriodicOutput, explicit set logger handle by pass argument MainActivity's Logger (by default is std::cout)
                Object::create<NotificationTest>(),                                         // NotificationTest, use the logger from NotificationTest's context >> file [app.log]
                Logger::file("rebuild_test.log", Object::create<RebuildTest>()),            // RebuildTest, wrap new logger that redirect logger to another file >> file [rebuild_test.log]
            })                                                                              // MultiChildWidget
        );                                                                                  // Logger
    }
};
