#include "async_runtime/object/object.h"
#include <iostream>

class A
{
public:
    A() { std::cout << "A constructor" << std::endl; }
    ~A() { std::cout << "A destructor" << std::endl; }
};

int main()
{
    std::cout << "testing lvalue: " << std::endl;
    {
        A a;
        Function<void(const A &)> lvalue = [](const A &) { //
        };
        lvalue(a);
        std::cout << "function exited" << std::endl;
    }
    std::cout << "testing lvalue completed " << std::endl
              << std::endl;

    std::cout << "testing rvalue: " << std::endl;
    {
        Function<void(A &&)> rvalue = [](A &&) { //
        };
        rvalue(A());
        std::cout << "function exited" << std::endl;
    }
    std::cout << "testing rvalue completed " << std::endl;

    return EXIT_SUCCESS;
}