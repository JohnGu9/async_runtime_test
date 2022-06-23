#include "async_runtime/object/object.h"
#include <algorithm>
#include <assert.h>

static void testBasic();
static void testMerge();
static void testLateInit();
static void testMixKey();
int main()
{
    testBasic();
    testMerge();
    testLateInit();
    testMixKey();
}

static void testBasic()
{
    // ref<Map<ref<String>, ref<String>>> map = {}; // compile error
    // use Map<ref<String>, lateref<String>>::create() to create a empty map
    ref<Map<ref<String>, ref<String>>> map = {
        {"A", "B"},
        {"B", "C"},
        {"C", "D"},
    };
    map["K"] = "B";
    map["A"] = "C++";
    map->removeKey("B");
    assert(map->size() == 3);
    assert(map["A"] == "C++");
    assert(map["C"] == "D");
    assert(map["K"] == "B");

    for (const auto &element : {"A", "C", "K"})
        assert(map->containsKey(element));

    assert(!map->containsKey("B"));

    std::cout << map << std::endl;
    std::cout << map->keys() << std::endl;
    std::cout << map->values() << std::endl;

    std::cout << "forEach Map: " << std::endl;
    map->forEach([](const ref<Pair<const ref<String>, ref<String>>> &pair)
                 { std::cout << pair->first << " : " << pair->second << std::endl; });
    std::cout << "A value is " << map["A"] << std::endl;

    std::cout << "Mapped Map: " << std::endl;
    auto mapped = map->map<ref<String>>([](const ref<Pair<const ref<String>, ref<String>>> &pair)
                                        { return pair->second + " mapped"; });
    assert(mapped->size() == 3);
    assert(mapped["A"] == "C++ mapped");
    assert(mapped["C"] == "D mapped");
    assert(mapped["K"] == "B mapped");
    std::cout << mapped << std::endl;
}

static void testMerge()
{
    ref<Map<int, int>> map0 = {
                           {1, 1},
                           {2, 2},
                           {3, 3},
                       },
                       map1 = {
                           {3, 3},
                           {4, 4},
                           {5, 5},
                       };

    auto merge0 = Map<int, int>::merge({map0, map1});
    assert(merge0->size() == 5);
    for (const auto &key : {1, 2, 3, 4, 5})
    {
        assert(merge0->containsKey(key));
        assert(merge0[key] == key);
    }
    assert(!merge0->containsKey(6));
    std::cout << merge0 << std::endl;
}

static void testLateInit()
{
    lateref<Map<int, ref<String>>> map;
    map = Map<int, ref<String>>::create();
    map = {{1, "1"}, {2, "2"}};
    assert(map[1] == "1");
    assert(map->size() == 2);
    std::cout << map << std::endl;
}

class A : public Object
{
};
static void testMixKey()
{
    ref<Map<ref<Object>, int>> map = Map<ref<Object>, int>::create();
    map[ref<String>("1")] = 1;
    map[Object::create<A>()] = 123;
    map[ref<String>("2")] = 2;
    assert(map->size() == 3);

    std::cout << map << std::endl;

    map[ref<String>("2")] = 4;
    map[Object::create<A>()] = 321;
    assert(map->size() == 4);

    std::cout << map << std::endl;
}
