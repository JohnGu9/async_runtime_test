#include "async_runtime.h"

struct MyWidget : StatefulWidget
{
    Object::Ref<State<StatefulWidget>> createState() override;
};

struct _MyWidgetState : State<MyWidget>
{
    using super = State<MyWidget>;
    Object::Ref<File> _file;
    Object::Ref<Timer> _timer;

    void initState() override
    {
        super::initState();
        debug_print("initState");
        auto self = Object::cast<>(this);
        _file = File::fromPath(this, "app.log");
        _timer = Timer::delay(
            this,
            [self] {
                self->_file->exists()
                    ->than<void>([self](const bool &exists) { debug_print("File exists: " << exists); })
                    ->than<void>([self] {
                        self->_file->overwrite("This is app log. \n")
                            ->than<void>([self] {
                                self->_file->readWordAsStream()
                                    ->listen([self](String word) { debug_print(word); })
                                    ->onClose([self] {
                                        self->_file->read()
                                            ->than<void>([self](const String &value) {
                                                debug_print(value);
                                                debug_print("To remove file: " << self->_file->path);
                                                self->_file->remove()->than<void>(
                                                    [self](const int &code) {
                                                        debug_print("Remove code: " << code);
                                                        RootInheritedWidget::of(self->context)->requestExit();
                                                    });
                                            });
                                    });
                            });
                    });
            },
            Duration::fromMilliseconds(1000));
    }

    void dispose() override
    {
        _timer->dispose();
        _file->dispose();
        super::dispose();
    }

    Object::Ref<Widget> build(Object::Ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

inline Object::Ref<State<StatefulWidget>> MyWidget::createState()
{
    return Object::create<_MyWidgetState>();
}

int main()
{
    runApp(Object::create<MyWidget>());
    return EXIT_SUCCESS;
}