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

    a = 0_Boolean;
    assert(a == false);
    a = 1_Boolean;
    assert(a == true);

    return EXIT_SUCCESS;
}
