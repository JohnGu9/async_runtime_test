#include "async_runtime.h"

#ifdef _WIN32
#pragma warning(disable : 4573)
#endif

#ifdef _WIN32
static const auto S_IRUSR = _S_IREAD;
static const auto S_IWUSR = _S_IWRITE;
#endif

static const auto FILENAME = "logger_widget_test.log";

class MyWidget : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _MyWidgetState : public State<MyWidget>
{
    using super = State<MyWidget>;
    lateref<Timer> _timer;

    void initState() override
    {
        super::initState();
        _timer = Timer::periodic(Duration(1000), [this] { //
            // just output logger handler
            Logger::of(context)->writeLine("Logger::of(context)");  // root widget wrap a default logger context that output message to console
            LogInfo("LogInfo stream like " << 123 << ' ' << "abc"); // implicitly use context's logger
            LogInfo("LogInfo format like {} {}", 123, "abc");       // implicitly use context's logger

            // just output to stdout
            RootWidget::of(context)->cout->writeLine("RootWidget::of(context)->cout"); // root widget's cout is thread safe
            std::cout << "std::cout" << std::endl;                                     // it's also ok to use std::cout to print in master event loop
            std::cout << std::endl;
            return;
        });
        _timer->start();
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

inline ref<State<>> MyWidget::createState()
{
    return Object::create<_MyWidgetState>();
}

class LoggerSwitch : public StatefulWidget
{
    ref<State<>> createState() override;

public:
    finalref<Widget> child;
    LoggerSwitch(ref<Widget> child) : StatefulWidget(), child(child) {}
};

class _LoggerSwitchState : public State<LoggerSwitch>
{

    using super = State<LoggerSwitch>;
    option<File> _file;

    void initState() override
    {
        super::initState();
        std::cout << "Logger output to cout" << std::endl; // output log to stdout

        Future<>::wait(
            Future<int>::delay(Duration(2500), 0),
            File::fromPath(FILENAME, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR))
            ->then<ref<File>>([](const ref<Future<>::Package<int, ref<File>>> &package) { //
                return package->getValue<1>();
            })
            ->then<int>([this](const ref<File> &file) { //
                setState([this, file] {                 //
                    std::cout << "Logger output to file" << std::endl;
                    _file = file;
                });
                return 0;
            })
            ->then<int>([] { //
                return Future<int>::delay(Duration(2500), 0);
            })
            ->then<int>([this] { //
                exitApp(context);
                return 0;
            });
    }

    void dispose() override
    {
        if_not_null(_file)
        {
            _file->close()
                ->then<ref<File>>([] { //
                    return File::fromPath(FILENAME, O_RDONLY, 0);
                })
                ->then<int>([](const ref<File> &file) { //
                    return file->read()
                        ->then<int>([file](const ref<String> &str) { //
                            std::cout << std::endl
                                      << "File content: " << std::endl
                                      << str << std::endl;
                            return file->close();
                        });
                });
        }
        end_if();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        if_not_null(_file)
        {
            return Logger::file(widget->child, _file);
        }
        end_if();
        return Logger::cout(widget->child);
    }
};

inline ref<State<>> LoggerSwitch::createState()
{
    return Object::create<_LoggerSwitchState>();
}

int main()
{
    runApp(Object::create<LoggerSwitch>(Object::create<MyWidget>()));
    return 0;
}
