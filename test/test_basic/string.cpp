#include "async_runtime.h"

int main()
{
    ref<String> nonNullString = "nonNullString";
    option<String> nullableString = "nullableString";
    std::cout << nonNullString << std::endl;
    // std::cout << nullableString << std::endl;
    //           ^^^^^^^^^^^^^^^^^
    // error: option<String> can't print. Only ref<String> can print

    // to print option<String>, please check the null situation
    lateref<String> lateString;
    if (nullableString.isNotNull(lateString))
    {
        std::cout << lateString << std::endl; // it work
    }

    // If you can assert. But it may intro null reference into program again.
    std::cout << nullableString.assertNotNull() << std::endl; // it work

    option<String> reallyNullString = nullptr;
    // std::cout << reallyNullString.assertNotNull() << std::endl;
    //                              ^^^^^^^^^^^^^^^^
    // error: it can compile but must fail in runtime

    // use isNotNullElse api
    ref<String> notReallyNullString = reallyNullString.isNotNullElse([] { return "notReallyNullString"; });
    std::cout << notReallyNullString << std::endl; // it work

    //
    //
    // String compare
    //

    ref<String> string0 = "Hello World";
    ref<String> string1 = "Hello World";
    assert(string0 == string1);
    assert(!(string0 != string1));

    option<String> string2 = "Hello World";
    option<String> string3 = "Hello World";
    option<String> string4 = nullptr;
    // with ref<String>
    assert(string2 == string0);
    assert(!(string2 != string0));
    // with option<String>
    assert(string2 == string3);
    assert(!(string2 != string3));
    // with nullptr
    assert(string2 != string4);
    assert(!(string2 == string4));
    assert(string4 == nullptr);
    assert(!(string4 != nullptr));

    //
    //

    // String template
    // format String from source and arguments

    // init from String::format
    ref<String> formatedString =
        ref<String>("Hello, {}! It's {} today. Temperature is {} degree. The answer is {}. ")
            ->format("Kiddy", "rain", 26, true); // match "{}"
    std::cout << formatedString << std::endl;    // Hello, Kiddy! It's rain today. Temperature is 26 degree. The answer is true.

    // init from String::formatFromString
    formatedString = String::formatFromString("Hello, {}! It's {} today. Temperature is {} degree. The answer is {}. ",
                                              "Kiddy", "rain", 26, true);
    std::cout << formatedString << std::endl;

    //init from String::formatFromIterator
    auto str = std::string("Hello, {}! It's {} today. Temperature is {} degree. The answer is {}. ");
    formatedString = String::formatFromIterator(str.begin(), str.end(), "Kiddy", "rain", 26, true);
    std::cout << formatedString << std::endl;

    //
    //

    // String format exception case(String format would not throw any error)
    // more arguments or less arguments

    formatedString = ref<String>("Hello, {}! It's {} today. Temperature is {} degree. ")->format("Kiddy", "rain"); // less arguments is fine
    std::cout << formatedString << std::endl;                                                                      // Hello, Kiddy! It's rain today. Temperature is {} degree.

    formatedString = ref<String>("Hello, {}! It's {} today. ")->format("Kiddy", "rain", 26); // more arguments is fine but with error message on console
    std::cout << formatedString << std::endl;                                                // Hello, Kiddy! It's rain today.

    //
    //

    // String::split
    //

    auto list = formatedString->split(" ");
    for (const auto &element : list)
    {
        std::cout << element << std::endl;
    }
    assert(list->size() == 5);
    std::cout << "list length: " << list->size() << std::endl;
    return EXIT_SUCCESS;
}
