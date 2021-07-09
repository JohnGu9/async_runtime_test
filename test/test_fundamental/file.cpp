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
            /* check file exists */
            ->than<long long>([this](const bool &exists) -> FutureOr<long long>
                              {
                                  LogInfo("File exists: " << exists);
                                  if (exists)
                                      return {_file->size()};
                                  else
                                      return {0};
                              })
            /* print file size */
            ->than<void>([this](const long long &size) -> FutureOr<void>
                         {
                             LogInfo("File size: " << size);
                             return {};
                         })
            /* overwrite file */
            ->than<void>([this]() -> FutureOr<void>
                         {
                             LogInfo("File override ");
                             std::stringstream ss;
                             for (size_t i = 0; i < 500; i++)
                                 ss << "[" << i << "] This is app log. " << std::endl;
                             return {_file->overwrite(ss.str() /* build ref<String> from std::string */)};
                         })
            /* read file */
            ->than([this]
                   {
                       LogInfo("File readWordAsStream: ");
                       std::shared_ptr<size_t> amountOfWords = std::make_shared<size_t>(0); // async api run code out of current scope, make sure the resource live longer than the async task
                       auto stream = _file->readWordAsStream();
                       stream->listen([amountOfWords](const ref<String> &word)
                                      { (*amountOfWords)++; });
                       stream->asFuture()
                           ->than<ref<String>>([this, amountOfWords]() -> FutureOr<ref<String>>
                                               {
                                                   LogInfo("File stream close with amount of word: {}. ", *amountOfWords);
                                                   LogInfo("File read: ");
                                                   return {_file->read()};
                                               })
                           /* remove file */
                           ->than<int>([this](const ref<String> &value) -> FutureOr<int>
                                       {
                                           LogInfo(std::endl
                                                   << value);
                                           LogInfo("To remove file: \"{}\"", _file->path);
                                           return {_file->remove()};
                                       })
                           /* check remove file state */
                           ->than<void>([this](const int &code) -> FutureOr<void>
                                        {
                                            LogInfo("Remove code: " << code);
                                            Process::of(context)->exit();
                                            return {};
                                        });
                   });
    }

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