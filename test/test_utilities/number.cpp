#include "async_runtime/utilities/number.h"

static void integer_test();
static void float_test();
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

    integer_test();
    float_test();
}

static void integer_test()
{
    ref<Number> s = (short)10;
    ref<Number> i = 10;
    ref<Number> l = 10l;
    ref<Number> ll = 10ll;

    assert(i == s);
    assert(i == l);
    assert(i == ll);

    assert(s == 10);
    assert(i == 10);
    assert(l == 10);
    assert(ll == 10);
}

static void float_test()
{
    ref<Number> f = 10.0f;
    ref<Number> d = 10.0;
    assert(f == d);

    assert(f == 10.0f);
    assert(d == 10.0);
}
