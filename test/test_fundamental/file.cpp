#include "async_runtime/fundamental/file.h"
#include <iostream>

void task();
int main()
{
    EventLoop::run(task);
}

void task()
{
    auto file = File::fromPath("test_file.txt");
    file->overwrite("This is a useless file for testing. ")
        ->then<int>([file](const int &)
                    {
        file->exists()
            ->then<ref<String>>([file](const bool &exists)
            {
                if (exists)
                {
                    return FutureOr<ref<String>>(file->read());
                } else {
                    return FutureOr<ref<String>>("File not exists");
                } 
            })
            ->then<int>([file](const ref<String> &value)
            { 
                std::cout << "Value: " << std::endl  << value;
                file->dispose();
                return 0; 
            });

        return 0; });
}
