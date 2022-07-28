#include "async_runtime/object/object.h"
#include <algorithm>
#include <assert.h>

class A : public Object
{
    const int _value;

public:
    A(int value) : _value(value) {}
    int value() { return _value + 1; }
};

int main()
{
    ref<List<ref<A>>> list = {
        Object::create<A>(1),
        Object::create<A>(2),
        Object::create<A>(3),
    };
    auto mapped = CachedMapIterable<ref<A>, int>::from(list, [](const ref<A> &origin) -> int { //
        return origin->value();
    });
    auto expect = {2, 3, 4};
    assert(std::equal(mapped->begin(), mapped->end(), expect.begin()));
    std::cout << mapped << std::endl;

    return EXIT_SUCCESS;
}
