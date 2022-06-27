#include "async_runtime/object/object.h"
#include <algorithm>
#include <assert.h>

int main()
{
    auto range = RangeIterable::from(0, 5);
    auto expect = {0, 1, 2, 3, 4};
    assert(std::equal(range->begin(), range->end(), expect.begin()));
    std::cout << range << std::endl;
}
