#include "async_runtime/utilities/number.h"
#include <assert.h>
#include <map>

static void integer_test();
static void float_test();
static void literals_test();

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
    map[i] = i;
    std::cout << map << std::endl;

    std::map<option<Number>, ref<Number>> m = {
        {1, 1},
        {1.2, 1.2},
        {-10, -10},
        {nullptr, 10000},
    };
    for (const auto &pair : m)
        std::cout << pair.first << " : " << pair.second << std::endl;

    integer_test();
    float_test();
    literals_test();
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

    assert(i < 100);
    assert(i > 1);
    assert(i < 100.0f);
    assert(i > 1.0);
}

static void float_test()
{
    ref<Number> f = 10.0f;
    ref<Number> d = 10.0;
    ref<Number> ld = 10.0l;

    assert(f == d);
    assert(d == ld);

    assert(f == 10.0f);
    assert(d == 10.0);
    assert(ld == 10.0l);

    assert(f > 1);
    assert(d < 100.0l);
    assert(ld < 10.2);
}

static void literals_test()
{
    auto a = 1.2_Number;
    auto b = 10_Number;

    assert(a == 1.2l);
    assert(b == 10);
}
