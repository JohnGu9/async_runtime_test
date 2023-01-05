#pragma once
#include "async_runtime.h"
#include "notification_test.h"
#include "periodic_output.h"
#include "rebuild_test.h"

#ifdef _WIN32
static const auto S_IRUSR = _S_IREAD;
static const auto S_IWUSR = _S_IWRITE;
#endif

class MainActivity : public StatefulWidget
{
    ref<State<StatefulWidget>> createState() override;
};

class _MainActivityState : public State<MainActivity>
{
    using super = State<MainActivity>;
    option<File> _file;

    void initState() override
    {
        super::initState();
        File::fromPath("rebuild_test.log", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR)
            ->then<int>([this](const ref<File> &file) { //
                if (this->mounted)
                {
                    setState([this, file] { //
                        this->_file = file;
                    });
                }
                else
                {
                    file->close();
                }
                return 0;
            });
    }

    void dispose() override
    {
        if_not_null(_file)
        {
            _file->close();
        }
        end_if();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext> context) override
    {
        if_not_null(_file)
        {
            return MultiChildWidget::fromChildren({
                Object::create<PeriodicOutput>(LeafWidget::factory(), Logger::of(context)), // PeriodicOutput, explicit set logger handle by pass argument MainActivity's Logger (by default is std::cout)
                Object::create<NotificationTest>(),                                         // NotificationTest, use the logger from NotificationTest's context (by default is std::cout)
                Logger::file(Object::create<RebuildTest>(), _file),                         // RebuildTest, wrap new logger that redirect logger to another file >> file [rebuild_test.log]
            });
        }
        end_if();
        return MultiChildWidget::fromChildren({
            Object::create<PeriodicOutput>(LeafWidget::factory(), Logger::of(context)), 
            Object::create<NotificationTest>(),      
        });                                                                             // MultiChildWidget
    }
};

inline ref<State<StatefulWidget>> MainActivity::createState()
{
    return Object::create<_MainActivityState>();
}
