#include <iostream>
#include "async_runtime/basic/value_notifier.h"

int main(const int argc, char **args)
{
    auto notifier = Object::create<ValueNotifier<bool>>(false);
    Function<void()> fn = [&notifier] {
        info_print("Value changed to " << notifier->value);
    };
    Function<> &ref = fn;
    notifier->addListener(fn);

    std::cout << "add same function will not call twice when [notifyListeners]" << std::endl;
    notifier->addListener(fn); // not valid

    std::cout << "Change value (only call function once)" << std::endl;
    notifier->setValue(true);

    std::cout << "Not change value" << std::endl;
    notifier->setValue(true);

    std::cout << "Remove Listener and change value" << std::endl;
    notifier->removeListener(fn);
    notifier->setValue(false);

    std::cout << "Dispose" << std::endl;
    notifier->dispose();

    std::cout << "Invalid access after dispose() and cause assert exception. " << std::endl;
    notifier->setValue(false);

    return EXIT_SUCCESS;
}