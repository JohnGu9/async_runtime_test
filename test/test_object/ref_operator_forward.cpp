#include "async_runtime/object/object.h"
#include <assert.h>

class A : public Object
{
public:
    int operator()() const { return 0; }
    char operator[](size_t i) const { return 'a'; }
    bool operator!() const { return false; }
    ref<A> operator~() { return self(); }
};

int main()
{
    auto a = Object::create<A>();
    assert(a() == 0 && a[10] == 'a' && !a == false && ~a == a);
    std::cout << std::boolalpha
              << "a(): " << a() << std::endl
              << "a[]: " << a[0] << std::endl
              << "!a: " << !a << std::endl;
    return EXIT_SUCCESS;
}
