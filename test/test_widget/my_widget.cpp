#include "../main_activity.h"
#include "async_runtime/elements/root_element.h"

int main()
{
    ref<RootElement> root = Object::create<RootElement>(Object::create<MainActivity>());
    root->attach();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    root->onCommand("ls");

    std::this_thread::sleep_for(std::chrono::seconds(2));
    root->onCommand("reassembly");

    std::this_thread::sleep_for(std::chrono::seconds(2));
    root->detach();
    return EXIT_SUCCESS;
}