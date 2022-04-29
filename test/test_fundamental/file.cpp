#include "async_runtime/fundamental/file.h"
#include <iostream>

static const auto FILENAME = "test_file.txt";

void task();
FutureOr<int> writeFile(ref<File> file);
FutureOr<int> appendFile(ref<File> file);
FutureOr<int> readFile(ref<File> file);

int main()
{
    EventLoop::run(task);
    return 0;
}

void task()
{
    File::unlink(FILENAME)
        ->then<ref<File>>([](const int &)
                          { return File::fromPath(FILENAME, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR); })
        ->then<int>(writeFile)
        ->then<ref<File>>([](const int &)
                          { return File::fromPath(FILENAME, O_WRONLY | O_APPEND, 0); })
        ->then<int>(appendFile)
        ->then<ref<File>>([](const int &)
                          { return File::fromPath(FILENAME, O_RDONLY, 0); })
        ->then<int>(readFile);
}

FutureOr<int> writeFile(ref<File> file)
{
    // unlikely have error
    // so I skip error checking :)
    return file->write("This is a useless file for testing. \n")
        ->then<int>([file](const int &)
                    { return file->close(); });
}

FutureOr<int> appendFile(ref<File> file)
{
    return file->writeAll({"OK\n", "NOT OK\n", "OK\n", "NOT OK\n"})
        ->then<int>([file](const int &)
               { return file->writeAll({"A\n", "B\n", "C\n", "D\n"}); })
        ->then<int>([file](const int &)
                    { return file->close(); });
}

FutureOr<int> readFile(ref<File> file)
{
    // also unlikely have error
    // but I have to demo error-checking :(
    // File::Error is also a File object that doesn't implement any function but [openCode] or [error]
    lateref<File::Error> error;
    if (file->cast<File::Error>().isNotNull(error)) // [[unlikely]]
    {
        std::cout << "File open failed with code" << error->openCode() << std::endl;
        return 0;
    }
    else
    {
        return file->stat()
            ->then<ref<String>>([file](ref<File::Stat> stat)
                                {
                                    stat->toStringStream(std::cout);
                                    return file->read(); })
            ->then<int>([file](ref<String> value)
                        {
                            std::cout << value << std::endl;
                            return file->close(); });
    }
}
