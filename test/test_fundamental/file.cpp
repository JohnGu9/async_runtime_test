#include "async_runtime.h"

struct MyWidget : StatefulWidget
{
    ref<State<StatefulWidget>> createState() override;
};

struct _MyWidgetState : State<MyWidget>
{
    using super = State<MyWidget>;
    lateref<File> _file;

    void _readWriteFile()
    {
        auto self = self();
        self->_file->exists()
            ->than<void>([self](const bool &exists) {
                debug_print("File exists: " << exists);
                if (exists)
                    self->_file->size()
                        ->than<void>([](const size_t &size) { debug_print("File size: " << size); });
            })
            ->than([self] {
                debug_print("File override ");
                self->_file->overwrite("This is app log. \n")
                    ->than([self] {
                        debug_print("File readWordAsStream: ");
                        auto subscription = self->_file->readWordAsStream()
                                                ->listen([self](ref<String> word) { debug_print(word); });
                        subscription->stream->onClose([self] {
                            debug_print("File stream close ");
                            debug_print("File read: ");
                            self->_file->read()
                                ->than<void>([self](const ref<String> &value) {
                                    debug_print(value);
                                    debug_print("To remove file: " << self->_file->path);
                                    self->_file->remove()->than<void>(
                                        [self](const int &code) {
                                            debug_print("Remove code: " << code);
                                            Process::of(self->context)->exit();
                                        });
                                });
                        });
                    });
            });
    };

    void initState() override
    {
        super::initState();
        debug_print("initState");
        _file = File::fromPath(this, "app.log");
        _readWriteFile();
    }

    void dispose() override
    {
        _file->dispose();
        debug_print("dispose");
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