#include "async_runtime/utilities/boolean.h"

int main()
{
    ref<Boolean> a = true;
    ref<Boolean> b = true;

    assert(a == b);
    assert(a == true);
    assert(b == true);

    b = false;

    assert(a != b);
    assert(a == true);
    assert(b != true);

    std::cout << a << std::endl
              << b << std::endl;
    return EXIT_SUCCESS;
}
