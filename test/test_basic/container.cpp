#include "async_runtime/basic/object.h"
#include <iostream>

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
    std::cout << list << std::endl;

    std::cout << "forEach List: ";
    list->forEach([](const int &element)
                  { std::cout << element << ' '; });
    std::cout << std::endl
              << "index: 1, element: " << list[1] << std::endl;

    std::cout << "forEach Mapped List: ";
    auto mapped = list->map<std::string>([](const int &value)
                                         { return std::to_string(value + 1); });
    mapped->forEach([](const std::string &element)
                    { std::cout << element << ' '; });
    std::cout << std::endl
              << "index: 1, element: " << mapped[1] << std::endl
              << std::endl;
}

static void testSet()
{
    std::cout << "set test" << std::endl;

    ref<Set<int>> set = {1, 2, 2, 3};
    std::cout << set << std::endl;

    std::cout << "forEach Set: ";
    set->forEach([](const int &element)
                 { std::cout << element << ' '; });
    std::cout << std::endl;

    std::cout << "forEach Mapped Set: ";
    auto mapped = set->map<std::string>([](const int &value)
                                        { return std::to_string(value + 1); });
    mapped->forEach([](const std::string &element)
                    { std::cout << element << ' '; });
    std::cout << std::endl
              << std::endl;
}
static void testMap()
{
    std::cout << "map test" << std::endl;

    lateref<Map<ref<String>, std::string>> map;
    // map = {}; // compile error
    // use Object::create<Map<ref<String>, std::string>>>() to create a empty map
    map = {
        {"A", "B"},
        {"C", "D"},
    };
    map["K"] = "B";
    map["A"] = "C++";

    std::cout << map << std::endl;

    std::cout << "forEach Map: " << std::endl;
    map->forEach([](const std::pair<const ref<String>, std::string> &pair)
                 { std::cout << pair.first << " : " << pair.second << std::endl; });
    std::cout << "A value is " << map["A"] << std::endl;

    std::cout << "forEach Mapped Set: " << std::endl;
    auto mapped = map->map<std::string>([](const std::string &value)
                                        { return value + " mapped"; });
    mapped->forEach([](const std::pair<const ref<String>, std::string> &pair)
                    { std::cout << pair.first << " : " << pair.second << std::endl; });
    std::cout << std::endl;
}
