#include "async_runtime/object/object.h"
#include <assert.h>

class A
{
public:
    static size_t instance_amount;
    static size_t construct_times;
    A()
    {
        std::cout << "A constructor" << std::endl;
        instance_amount++;
        construct_times++;
    }
    ~A()
    {
        std::cout << "A destructor" << std::endl;
        instance_amount--;
    }
};
size_t A::instance_amount = 0;
size_t A::construct_times = 0;

int main()
{
    std::cout << "testing lvalue: " << std::endl;
    {
        A a;
        Function<void(const A &)> lvalue = [](const A &) { //
        };
        lvalue(a);
        assert(A::instance_amount == 1);
        assert(A::construct_times == 1);
        std::cout << "function exited" << std::endl;
    }

    assert(A::instance_amount == 0);
    assert(A::construct_times == 1);
    std::cout << "testing lvalue completed " << std::endl
              << std::endl;

    std::cout << "testing rvalue: " << std::endl;
    {
        Function<void(A &&)> rvalue = [](A &&) { //
        };
        rvalue(A());
        assert(A::instance_amount == 0);
        assert(A::construct_times == 2);
        std::cout << "function exited" << std::endl;
    }

    assert(A::construct_times == 2);
    std::cout << "testing rvalue completed " << std::endl;

    lateref<Fn<void()>> fn;
    fn = [] { //
        std::cout << std::endl
                  << "Hello world" << std::endl;
    };
    fn();

    return EXIT_SUCCESS;
}
