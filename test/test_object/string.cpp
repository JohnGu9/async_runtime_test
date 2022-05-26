#include "async_runtime/object/object.h"

static void stringNonNullAndNullable();
static void stringConnect();
static void stringCompare();
static void stringTemplate();
static void stringSplit();

int main()
{
    std::cout << std::boolalpha;
    stringNonNullAndNullable();
    stringConnect();
    stringCompare();
    stringTemplate();
    stringSplit();
    return EXIT_SUCCESS;
}

static void stringNonNullAndNullable()
{
    ref<String> nonNullString = "nonNullString";
    option<String> nullableString = "nullableString";
    std::cout << nonNullString << std::endl;
    /**
     * std::cout << nullableString << std::endl;
     *            ^^^^^^^^^^^^^^^^^
     * error: option<String> can't print. Only ref<String> can print
     */

    /**
     * @brief to print option<String>, please check the null situation
     */
    lateref<String> lateString;
    if (nullableString.isNotNull(lateString))
    {
        std::cout << lateString << std::endl;
    }

    /**
     * @brief If you can assert. But it may intro null reference into program again.
     */
    std::cout << nullableString.assertNotNull() << std::endl;

    option<String> reallyNullString = nullptr;
    /**
     * std::cout << reallyNullString.assertNotNull() << std::endl;
     *                               ^^^^^^^^^^^^^^^^
     * error: it can compile but fail in runtime
     */

    /**
     * @brief use [ref::isNotNullElse] api to ensure a not null ref
     *
     */
    ref<String> notReallyNullString = reallyNullString.isNotNullElse([]
                                                                     { return "notReallyNullString"; });
    std::cout << notReallyNullString << std::endl;
}

static void stringConnect()
{
    ref<String> string0 = "Connect";
    ref<String> string1 = " to ";
    ref<String> string2 = "World";

    ref<String> connect0 = String::connect(string0, string1, string2); // maybe better performance
    ref<String> connect1 = string0 + string1 + string2;

    std::cout << connect0 << std::endl;
    std::cout << connect1 << std::endl;
}

static void stringCompare()
{
    ref<String> string0 = "Hello World";
    ref<String> string1 = "Hello World";
    assert(string0 == string1);
    assert(!(string0 != string1));

    option<String> string2 = "Hello World";
    option<String> string3 = "Hello World";
    option<String> string4 = nullptr;
    // with ref<String>
    assert(string2.assertNotNull() == string0);
    assert(!(string2.assertNotNull() != string0));
    // with option<String>
    assert(string2 == string3);
    assert(!(string2 != string3));
    // with nullptr
    assert(string2 != string4);
    assert(!(string2 == string4));
    assert(string4 == nullptr);
    assert(!(string4 != nullptr));
}

static void stringTemplate()
{
    /**
     * @brief init from String::format
     */
    ref<String> formattedString =
        ref<String>("Hello, {}! It's {} today. Temperature is {} degree. The answer is {}. ")
            ->format("Kiddy", "rain", 26, true); // match "{}"
    std::cout << formattedString << std::endl;   // Hello, Kiddy! It's rain today. Temperature is 26 degree. The answer is true.

    /**
     * @brief init from String::formatFromString
     */
    formattedString = String::formatFromString("Hello, {}! It's {} today. Temperature is {} degree. The answer is {}. ",
                                               "Kiddy", "rain", 26, true); // maybe better performance
    std::cout << formattedString << std::endl;

    /**
     * @brief init from String::formatFromIterator
     */
    auto str = std::string("Hello, {}! It's {} today. Temperature is {} degree. The answer is {}. ");
    formattedString = String::formatFromIterator(str.begin(), str.end(), "Kiddy", "rain", 26, true); // maybe better performance
    std::cout << formattedString << std::endl;

    /**
     * @brief String format exception case (String format would not throw any error)
     * more arguments or less arguments
     */
    formattedString = ref<String>("Hello, {}! It's {} today. Temperature is {} degree. ")->format("Kiddy", "rain"); // less arguments is fine
    std::cout << formattedString << std::endl;                                                                      // Hello, Kiddy! It's rain today. Temperature is {} degree.

    formattedString = ref<String>("Hello, {}! It's {} today. ")->format("Kiddy", "rain", 26); // more arguments is fine but with error message on console
    std::cout << formattedString << std::endl;                                                // Hello, Kiddy! It's rain today.
}

static void stringSplit()
{
    ref<String> value = "A B C  D    E";
    auto list = value->split(" ");   // unlike python, the empty string will be ignored
    for (const auto &element : list) // so only [5] elements in [list]
    {
        assert(element->length() > 0);
        std::cout << element << std::endl;
    }
    assert(list->size() == 5);
    std::cout << "list connect: " << String::connect(list[0], list[1], list[2], list[3], list[4]) << std::endl;
    std::cout << "list[0] == 'A': " << (list[0] == "A") << std::endl;
    std::cout << "list length: " << list->size() << std::endl;
}