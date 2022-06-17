#include "async_runtime/object/object.h"
#include <algorithm>
#include <assert.h>

static void testList();
static void testSet();
static void testMap();
int main()
{
    testList();
    testSet();
    testMap();
    return EXIT_SUCCESS;
}

static void testList()
{
    std::cout << "list test" << std::endl;

    ref<List<int>> list = {1, 2, 2, 3};
    assert(list->size() == 4);

    auto expect = {1, 2, 2, 3};
    assert(std::equal(list->begin(), list->end(), expect.begin()));

    assert(list->contains(2));
    assert(!list->contains(4));
    std::cout << list << std::endl;

    std::cout << "forEach List: ";
    list->forEach([](const int &element)
                  { std::cout << element << ' '; });
    std::cout << std::endl
              << "index: 1, element: " << list[1] << std::endl;

    std::cout << "forEach Mapped List: ";
    auto mapped = list->map<std::string>([](const int &value)
                                         { return std::to_string(value + 1); });
    auto expect0 = {"2", "3", "3", "4"};
    assert(std::equal(mapped->begin(), mapped->end(), expect0.begin()));

    std::cout << mapped << std::endl
              << "index: 1, element: " << mapped[1] << std::endl
              << std::endl;
}

static void testSet()
{
    std::cout << "set test" << std::endl;

    ref<Set<int>> set = {1, 2, 2, 3};
    assert(set->size() == 3);
    for (const auto &element : {1, 2, 3})
        assert(set->contains(element));
    assert(!set->contains(4));
    std::cout << set << std::endl;

    std::cout << "forEach Set: ";
    set->forEach([](const int &element)
                 { std::cout << element << ' '; });
    std::cout << std::endl;

    std::cout << "forEach Mapped Set: ";
    auto mapped = set->map<std::string>([](const int &value)
                                        { return std::to_string(value + 1); });
    for (const auto &element : {"2", "3", "4"})
        assert(mapped->contains(element));

    std::cout << mapped << std::endl
              << std::endl;
}

static void testMap()
{
    std::cout << "map test" << std::endl;

    // map = {}; // compile error
    // use Map<ref<String>, lateref<String>>::create() to create a empty map
    // ref<String> can't be the Value type of Map for now
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

    std::cout << "forEach Mapped Set: " << std::endl;
    auto mapped = map->map<ref<String>>([](const ref<Pair<const ref<String>, ref<String>>> &pair)
                                            { return pair->second + " mapped"; });

    assert(mapped->size() == 3);
    assert(mapped["A"] == "C++ mapped");
    assert(mapped["C"] == "D mapped");
    assert(mapped["K"] == "B mapped");
    std::cout << mapped << std::endl;
}
