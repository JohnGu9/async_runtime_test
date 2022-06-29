#include "async_runtime/utilities/number.h"

int main()
{
    ref<Number> i = 10;
    ref<Number> d = 10.1;
    std::cout << i << std::endl
              << d << std::endl;

    ref<String> key = "key",
                value = "value";
    ref<Map<ref<Object>, ref<Object>>> map = {
        {i, d},
        {d, i},
        {key, value},
    };
    std::cout << map << std::endl;
}
