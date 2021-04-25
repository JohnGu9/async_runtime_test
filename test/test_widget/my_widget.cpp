#define ASYNC_RUNTIME_DISABLE_CONSOLE
#include "async_runtime.h"
#include "../main_activity.h"

int main()
{
    ref<RootElement> root = Object::create<RootElement>(Object::create<MainActivity>());
    root->attach();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    root->onCommand("ls");

    std::this_thread::sleep_for(std::chrono::seconds(2));
    root->onCommand("reassembly");

    std::this_thread::sleep_for(std::chrono::seconds(4));
    root->detach();
    return EXIT_SUCCESS;
}