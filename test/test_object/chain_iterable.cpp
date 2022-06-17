#include "async_runtime/object/object.h"
#include <algorithm>
#include <assert.h>

int main()
{
    ref<List<int>> list0 = {1, 2, 3, 4},
                   list1 = {5, 6, 7};
    auto chain = ChainIterable<int>::from({list0, list1});
    assert(chain->size() == list0->size() + list1->size());
    std::cout << chain << std::endl;

    ref<List<int>> expect = {1, 2, 3, 4, 5, 6, 7};
    assert(std::equal(chain->begin(), chain->end(), expect->begin()));
    return EXIT_SUCCESS;
}