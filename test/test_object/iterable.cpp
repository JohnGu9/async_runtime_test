#include "async_runtime/object/object.h"
#include <algorithm>
#include <assert.h>

int main()
{
    ref<Iterable<int>> list0 = {1, 2, 3, 4};
    ref<Iterable<ref<Pair<const int, int>>>> list1 = {{1, 2}, {3, 4}};
    std::cout << list0 << std::endl
              << list1 << std::endl;

    return EXIT_SUCCESS;
}
