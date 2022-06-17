#include "async_runtime/object/object.h"
#include <algorithm>
#include <assert.h>

static void testBasic();
static void testMerge();
int main()
{
    testBasic();
    testMerge();
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
        assert(merge0->containsKey(key));
    assert(!merge0->containsKey(6));
    std::cout << merge0 << std::endl;
}
