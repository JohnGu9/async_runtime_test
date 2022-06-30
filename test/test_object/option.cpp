#include "async_runtime/object/object.h"
#include <assert.h>

class A : public Object
{
};

int main()
{
    ref<A> r = Object::create<A>();

    option<A> f = r;
    option<A> e = nullptr;
    assert(!Object::isNull<>(f));
    assert(Object::isNull<>(e));

    e = f;
    f = nullptr;
    assert(Object::isNull<>(f));
    assert(!Object::isNull<>(e));

    std::cout << f << std::endl
              << e << std::endl
              << f.ifNotNullElse([&]
                                 { return r; })
              << std::endl;
}
