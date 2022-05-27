#include "async_runtime/fundamental/file.h"
#include <iostream>

static const auto FILENAME = "test_file.txt";

void task();
int main()
{
    EventLoop::run(task);
    return 0;
}

static FutureOr<int> writeFile(ref<File> file);
static FutureOr<int> appendFile(ref<File> file);
static FutureOr<int> readFile(ref<File> file);

void task()
{
#ifdef _WIN32
    static const auto S_IRUSR = _S_IREAD;
    static const auto S_IWUSR = _S_IWRITE;
#endif
    File::unlink(FILENAME)
        ->then<ref<File>>([]
                          { return File::fromPath(FILENAME, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR); })
        ->then<int>(writeFile)
        ->then<ref<File>>([]
                          { return File::fromPath(FILENAME, O_WRONLY | O_APPEND, 0); })
        ->then<int>(appendFile)
        ->then<ref<File>>([]
                          { return File::fromPath(FILENAME, O_RDONLY, 0); })
        ->then<int>(readFile);
}

static FutureOr<int> writeFile(ref<File> file)
{
    // unlikely have error
    // so I skip error checking :)
    assert(file->error() > -1);
    return file->write("This is a useless file for testing. \n")
        ->then<int>([file]
                    { return file->close(); });
}

static FutureOr<int> appendFile(ref<File> file)
{
    assert(file->error() > -1);
    return file->writeAll({"OK\n", "NOT OK\n", "OK\n", "NOT OK\n"})
        ->then<int>([file]
                    { return file->writeAll({"A\n", "B\n", "C\n", "D\n"}); })
        ->then<int>([file]
                    { return file->close(); });
}

static FutureOr<int> readFile(ref<File> file)
{
    // also unlikely have error
    // but I have to demo error-checking :(
    // File::Error is also a File object that doesn't implement any function but [error]
    lateref<File::Error> error;
    if (file->cast<File::Error>().isNotNull(error)) // [[unlikely]]
    {
        std::cout << "File open failed with code" << error->error() << std::endl;
        return 0;
    }
    else
    {
        return file->stat()
            ->then<ref<String>>([file](ref<File::Stat> stat) { //
                stat->toStringStream(std::cout);
                return file->read();
            })
            ->then<int>([file](ref<String> value) { //
                auto except = "This is a useless file for testing. \n"
                              "OK\n"
                              "NOT OK\n"
                              "OK\n"
                              "NOT OK\n"
                              "A\n"
                              "B\n"
                              "C\n"
                              "D\n";

                assert(value == except);
                std::cout << value << std::endl;
                return file->close();
            });
    }
}
