#include "async_runtime/object/object.h"
#include <algorithm>
#include <array>
#include <assert.h>
#include <list>

int main()
{
    std::array<int, 5> array = {1, 2, 3, 4, 5};
    std::list<int> link = {1, 2, 3, 4, 5};
    auto list = {1, 2, 3, 4, 5};
    auto wrapper0 = IteratorWrapperIterable<>::from(array.begin(), array.end());
    auto wrapper1 = IteratorWrapperIterable<>::from(array);
    auto wrapper2 = IteratorWrapperIterable<>::from(link);
    auto wrapper3 = IteratorWrapperIterable<>::from(static_cast<const std::array<int, 5> &>(array));
    auto wrapper4 = IteratorWrapperIterable<>::from(list);
    assert(std::equal(wrapper0->begin(), wrapper0->end(), wrapper1->begin()));
    std::cout << wrapper0 << std::endl
              << wrapper1 << std::endl
              << wrapper2 << std::endl
              << wrapper3 << std::endl
              << wrapper4 << std::endl;

    return EXIT_SUCCESS;
}
