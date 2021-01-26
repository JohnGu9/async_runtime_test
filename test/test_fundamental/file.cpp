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
            this, [self] {
                self->_file->overwrite("This is app log. \n")
                    ->than<void>([self] {
                        self->_file->readWordAsStream()
                            ->listen([self](String word) { debug_print(word); })
                            ->onClose([self] {
                                self->_file->read()
                                    ->than<void>([self](const String &value) {
                                        debug_print(value);
                                        RootInheritedWidget::of(self->getContext())->requestExit();
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