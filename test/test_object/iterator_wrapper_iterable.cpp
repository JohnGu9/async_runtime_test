#include "async_runtime/object/object.h"
#include <algorithm>
#include <array>
#include <assert.h>

int main()
{
    std::array<int, 5> array = {1, 2, 3, 4, 5};
    auto list = {1, 2, 3, 4, 5};
    auto wrapper0 = IteratorWrapperIterable<int, std::array<int, 5>::iterator>::from(array.begin(), array.end());
    auto wrapper1 = IteratorWrapperIterable<int, std::initializer_list<int>::iterator>::from(list.begin(), list.end());
    assert(std::equal(wrapper0->begin(), wrapper0->end(), wrapper1->begin()));
    std::cout << wrapper0 << std::endl
              << wrapper1 << std::endl;

    return EXIT_SUCCESS;
}
