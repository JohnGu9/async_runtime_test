#include "async_runtime.h"

class MyWidget : public StatelessWidget
{
    ref<Widget> build(ref<BuildContext> context) override
    {
        return LeafWidget::factory();
    }
};

int main()
{
    runApp(Object::create<MyWidget>());
    return 0;
}
