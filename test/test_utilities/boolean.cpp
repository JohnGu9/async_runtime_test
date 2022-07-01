#include "async_runtime/utilities/boolean.h"

int main()
{
    ref<Boolean> a = true;
    ref<Boolean> b = true;

    assert(a == b);

    b = false;

    assert(a != b);

    std::cout << a << std::endl
              << b << std::endl;
    return EXIT_SUCCESS;
}
