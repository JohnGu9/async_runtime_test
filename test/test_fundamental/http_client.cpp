#include <iostream>
#include "async_runtime.h"
#include "async_runtime/fundamental/http.h"

class MyWidget : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _MyWidgetState : public State<MyWidget>
{
    using super = State<MyWidget>;
    lateref<Http::Client> _client;

    void initState() override
    {
        super::initState();
        _client = Object::create<Http::Client>(this, "http://www.google.com");
        _client->get("/")->than<void>([this](const ref<Http::Client::Result> &res) {
            debug_print("Get result");
            RootInheritedWidget::of(this->context)->exit();
        });
    }

    void dispose() override
    {
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

int main()
{
    runApp(Object::create<MyWidget>());
    return EXIT_SUCCESS;
}