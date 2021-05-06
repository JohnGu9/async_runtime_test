#pragma once

#include "async_runtime.h"
#include "periodic_output.h"
#include "notification_test.h"
#include "rebuild_test.h"
#include "command_echo.h"

class MainActivity : public StatelessWidget
{
    ref<Widget> build(ref<BuildContext> context) override
    {
        return Logger::file(
            "app.log", // redirect default logger to file
            MultiChildWidget::fromChildren({
                Object::create<PeriodicOutput>(LeafWidget::factory(), Logger::of(context)), // PeriodicOutput, explicit set logger handle by pass argument >> std::cout
                Object::create<NotificationTest>(),                                         // NotificationTest, use the logger from NotificationTest's context >> file [app.log]
                Logger::file("rebuild_test.log", Object::create<RebuildTest>()),            // RebuildTest, wrap new logger that redirect logger to another file >> file [rebuild_test.log]
                Object::create<CommandEcho>(),                                              //
            })                                                                              // MultiChildWidget
        );                                                                                  // Logger
    }
};
