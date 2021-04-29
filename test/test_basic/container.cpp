#include "async_runtime.h"
#include <iostream>

int main()
{
    lateref<List<int>> list = {1, 2, 3};
    for (auto &element : list)
        std::cout
            << element << std::endl;
    std::cout << list[1] << std::endl;

    lateref<Set<int>> set = {1, 2, 3};
    for (auto &element : set)
        std::cout << element << std::endl;

    lateref<Map<std::string, std::string>> map = {
        {"A", "B"},
        {"C", "D"},
    };
    map["K"] = "B";
    for (auto &element : map)
        std::cout << element.first << " : " << element.second << std::endl;
    std::cout << "A value is " << map["A"] << std::endl;

    return EXIT_SUCCESS;
}