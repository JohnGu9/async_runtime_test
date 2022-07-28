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

class B : public Object
{
public:
    B()
    {
        std::cout << "B constructor" << std::endl;
    }
    ~B()
    {
        std::cout << "B destructor" << std::endl;
    }
    void call()
    {
        std::cout << "B call " << (size_t)this << std::endl;
    }

    Function<void()> bindCall()
    {
        return BIND_FUNCTION(call); // bind this and B::call
    }
};

int main()
{
    std::cout << "testing lvalue: " << std::endl;

    assert(A::instance_amount == 0);
    assert(A::construct_times == 0);

    {
        A a;
        Function<void(const A &)> lvalue = [](const A &) { //
            assert(A::instance_amount == 1);
            assert(A::construct_times == 1);
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
            assert(A::instance_amount == 1);
            assert(A::construct_times == 2);
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
                  << "Hello world" << std::endl
                  << std::endl;
    };
    fn();

    {
        Function<void()> fn = [] {};
        {
            auto b = Object::create<B>();
            fn = b->bindCall();
        }
        fn();
        {
            auto b = Object::create<B>();
            fn = Function<void()>::bind<>(&B::call, b.get());
        }
        fn();
    }
    std::cout << "testing bind completed " << std::endl;

    return EXIT_SUCCESS;
}
