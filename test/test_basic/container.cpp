#include "async_runtime/basic/container.h"
#include <iostream>

class MyClass
{
public:
    MyClass()
    {
        map = {};
        assert(map);
    }
    Map<int, std::string> map;
};

int main()
{
    auto myClass = MyClass();
    List<int> list = List<int>::empty();
    assert(list);
    list = {1, 2, 3};
    for (auto &element : list)
        std::cout << element << std::endl;
    std::cout << list[1] << std::endl;

    Set<int> set = Set<int>::empty();
    assert(set);
    set = {1, 2, 3};
    for (auto &element : set)
        std::cout << element << std::endl;

    Map<std::string, std::string> map = Map<std::string, std::string>::empty();
    assert(map);
    map = {
        {"A", "B"},
        {"C", "D"},
    };
    map["K"] = "B";
    for (auto &element : map)
        std::cout << element.first << " : " << element.second << std::endl;
    std::cout << "A value is " << map["A"] << std::endl;

    return EXIT_SUCCESS;
}