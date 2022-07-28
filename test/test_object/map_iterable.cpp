#include "async_runtime/object/object.h"
#include <algorithm>
#include <assert.h>

class A : public Object
{
public:
    const int value;
    A(int value) : value(value) {}
};

int main()
{
    ref<List<ref<A>>> list = {
        Object::create<A>(1),
        Object::create<A>(2),
        Object::create<A>(3),
    };
    auto mapped = MapIterable<ref<A>, int>::from(list, [](const ref<A> &origin) -> const int &
                                                 { return origin->value; });
    auto expect = {1, 2, 3};
    assert(std::equal(mapped->begin(), mapped->end(), expect.begin()));
    std::cout << mapped << std::endl;

    return EXIT_SUCCESS;
}
