#include "async_runtime.h"

struct MyWidget : StatefulWidget
{
    ref<State<>> createState() override;
};

struct _MyWidgetState : State<MyWidget>
{
    using super = State<MyWidget>;
    lateref<File> _file;

    void _readWriteFile()
    {
        _file->exists()
            ->than<void>([this](const bool &exists) {
                LogInfo("File exists: " << exists);
                if (exists)
                    _file->size()
                        ->than<void>([this](const size_t &size) { LogInfo("File size: " << size); });
            })
            ->than([this] {
                LogInfo("File override ");
                _file->overwrite("This is app log. \n")
                    ->than([this] {
                        LogInfo("File readWordAsStream: ");
                        auto subscription = _file->readWordAsStream()
                                                ->listen([this](ref<String> word) { LogInfo(word); });
                        subscription->stream->onClose([this] {
                            LogInfo("File stream close ");
                            LogInfo("File read: ");
                            _file->read()
                                ->than<void>([this](const ref<String> &value) {
                                    LogInfo(value);
                                    LogInfo("To remove file: " << _file->path);
                                    _file->remove()->than<void>(
                                        [this](const int &code) {
                                            LogInfo("Remove code: " << code);
                                            Process::of(context)->exit();
                                        });
                                });
                        });
                    });
            });
    };

    void initState() override
    {
        super::initState();
        _file = File::fromPath(this, "app.log");
        _readWriteFile();
    }

    void dispose() override
    {
        _file->dispose();
        LogInfo("dispose");
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

inline ref<State<StatefulWidget>> MyWidget::createState()
{
    return Object::create<_MyWidgetState>();
}

int main()
{
    return runApp(Object::create<MyWidget>());
}