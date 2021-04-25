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

    ref<String> formatedString =
        ref<String>("Hello, {}! It's {} today. Temperature is {} degree. The answer is {}. ")
            ->format("Kiddy", "rain", 26, true); // match "{}"
    std::cout << formatedString << std::endl;    // Hello, Kiddy! It's rain today. Temperature is 26 degree. The answer is true.
    formatedString = String::formatFromString("Hello, {}! It's {} today. Temperature is {} degree. The answer is {}. ",
                                    "Kiddy", "rain", 26, true);
    std::cout << formatedString << std::endl;

    formatedString = ref<String>("Hello, {}! It's {} today. Temperature is {} degree. ")->format("Kiddy", "rain"); // less arguments is fine
    std::cout << formatedString << std::endl;                                                                      // Hello, Kiddy! It's rain today. Temperature is {} degree.

    formatedString = ref<String>("Hello, {}! It's {} today. ")->format("Kiddy", "rain", 26); // more arguments is fine but with error message on console
    std::cout << formatedString << std::endl;                                                // Hello, Kiddy! It's rain today.

    return EXIT_SUCCESS;
}