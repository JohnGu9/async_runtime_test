#define ASYNC_RUNTIME_DISABLE_CONSOLE
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
                std::stringstream ss;
                for (size_t i = 0; i < 500; i++)
                    ss << "[" << i << "] This is app log. " << std::endl;
                _file->overwrite(ss.str() /* build ref<String> from std::string */)
                    ->than([this] {
                        LogInfo("File readWordAsStream: ");
                        std::shared_ptr<size_t> amountOfWords = std::make_shared<size_t>(0); // async api run code out of current scope, make sure the resource live longer than the async task
                        auto stream = _file->readWordAsStream();
                        stream->listen([amountOfWords](ref<String> word) { (*amountOfWords)++; });
                        stream->onClose([this, amountOfWords] {
                            LogInfo("File stream close with amount of word: {}. ", *amountOfWords);
                            LogInfo("File read: ");
                            _file->read()
                                ->than<void>([this](const ref<String> &value) {
                                    LogInfo(std::endl
                                            << value);
                                    LogInfo("To remove file: \"{}\"", _file->path);
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
        _file = File::fromPath(self(), "app.log");
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