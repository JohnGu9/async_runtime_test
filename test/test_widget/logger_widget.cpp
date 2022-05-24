#include "async_runtime.h"

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
            if (!mounted)                                 // [[unlikely]]
                return;

            // just output logger handler
            Logger::of(context)->writeLine("Logger::of(context)");  // root widget wrap a default logger context that output message to console
            LogInfo("LogInfo stream like " << 123 << ' ' << "abc"); // implicitly use context's logger
            LogInfo("LogInfo format like {} {}", 123, "abc");       // implicitly use context's logger

            // just output to stdout
            RootWidget::of(context)->cout->writeLine("RootWidget::of(context)->cout"); // root widget's cout is thread safe
            std::cout << "std::cout" << std::endl;                                     // it's also ok to use std::cout to print in master event loop
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
    enum Now
    {
        cout,
        file,
        none
    };
    using super = State<LoggerSwitch>;
    Now _now = cout;

    void initState() override
    {
        super::initState();
        _now = cout;                                                                        // output log to stdout
        Future<int>::delay(Duration(2500), [this] {                                         // delay 2.5s
            setState([this] {                                                               //
                _now = file;                                                                //
            });                                                                             // switch to output log to file
            return 0;                                                                       //
        })                                                                                  //
            ->then<int>([] {                                                                //
                return Future<int>::delay(Duration(2500), 0);                               //
            })                                                                              // delay 2.5s
            ->then<ref<File>>([this] {                                                      //
                setState([this] {                                                           //
                    _now = none;                                                            //
                });                                                                         // switch no child
                return File::fromPath(FILENAME, O_RDONLY, 0);                               // open the log file
            })                                                                              //
            ->then<ref<String>>([](ref<File> file) {                                        //
                lateref<File::Error> error;                                                 //
                if (file->cast<File::Error>().isNotNull(error))                             //
                {                                                                           //
                    return Future<ref<String>>::value(                                      //
                        String::formatFromString("File open failed: {} ", error->error())); //
                }                                                                           //
                return file->read();                                                        // read file content
            })                                                                              //
            ->then<int>([this](ref<String> value) {                                         //
                auto root = RootWidget::of(context);                                        //
                root->cout->writeLine("File content: ");                                    // put the content to stdout
                root->cout->writeLine(value);                                               //
                root->exit();                                                               //
                return 0;                                                                   //
            });
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        switch ((_now))
        {
        case cout:
            return Logger::cout(widget->child);
        case file:
            return Logger::file(widget->child, FILENAME);
        default:
            return LeafWidget::factory();
        }
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
