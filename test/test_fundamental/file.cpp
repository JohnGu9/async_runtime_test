#include <iostream>
#include "async_runtime/fundamental/file.h"

void task();
int main()
{
    EventLoop::ensure();
    EventLoop::run(task);
}

void task()
{
    auto file = File::fromPath("test_file.txt");
    file->overwrite("This is a useless file for testing. ")->than<int>([file](const int &)
                                                                       {
        file->exists()
            ->than<ref<String>>([file](const bool &exists)
            {
                if (exists)
                {
                    return FutureOr<ref<String>>(file->read());
                } else {
                    return FutureOr<ref<String>>("File not exists");
                } 
            })
            ->than<int>([file](const ref<String> &value)
            { 
                std::cout << "Value: " << std::endl  << value;
                file->dispose();
                return 0; 
            });

        return 0; });
}
