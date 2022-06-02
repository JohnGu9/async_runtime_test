#include "async_runtime/utilities/value_notifier.h"
#include "async_runtime/utilities/console_format.h"
#include <iostream>

int main(const int argc, char **args)
{
    auto notifier = Object::create<ValueNotifier<bool>>(false);
    Function<void()> fn = [&notifier] { //
        info_print("Value changed to " << notifier->value);
    };
    assert(notifier->getValue() == false);

    notifier->addListener(fn);

    std::cout << "add same function will not call twice when [notifyListeners]" << std::endl;
    notifier->addListener(fn); // not valid but no error

    std::cout << "Change value (only call function once)" << std::endl;
    notifier->setValue(true);
    assert(notifier->getValue() == true);

    std::cout << "Not change value" << std::endl;
    notifier->setValue(true);

    std::cout << "Remove Listener and change value" << std::endl;
    notifier->removeListener(fn);
    notifier->setValue(false);

    std::cout << "Dispose" << std::endl;
    notifier->dispose();
    
    try
    {
        std::cout << "Invalid access after dispose() and cause assert exception. " << std::endl;
        notifier->setValue(false);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return EXIT_SUCCESS;
}
