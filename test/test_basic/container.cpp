#include "async_runtime.h"
#include <iostream>

void testList()
{
    std::cout << "list test" << std::endl;

    ref<List<int>> list = {1, 2, 2, 3};
    std::cout << "Loop List: ";
    for (auto &element : list)
        std::cout << element << ' ';
    std::cout << std::endl;
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

void testSet()
{
    std::cout << "set test" << std::endl;

    ref<Set<int>> set = {1, 2, 2, 3};
    std::cout << "Loop Set: ";
    for (auto &element : set)
        std::cout << element << ' ';
    std::cout << std::endl;
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

void testMap()
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

    std::cout << "Loop Map: ";
    for (auto &element : map)
        std::cout << element.first << " : " << element.second << std::endl;
    std::cout << std::endl;

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

int main()
{
    testList();
    testSet();
    testMap();
    return EXIT_SUCCESS;
}