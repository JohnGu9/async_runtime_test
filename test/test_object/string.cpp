#include "async_runtime/object/object.h"
#include <assert.h>

static void stringNonNullAndNullable();
static void stringConnect();
static void stringCompare();
static void stringTemplate();
static void stringSplit();
static void stringSubString();
static void stringCaseTransform();
static void stringMap();

int main()
{
    std::cout << std::boolalpha;
    stringNonNullAndNullable();
    stringConnect();
    stringCompare();
    stringTemplate();
    stringSplit();
    stringSubString();
    stringCaseTransform();
    stringMap();
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

    nullableString.ifNotNull([](ref<String> nonNullString) { //
        std::cout << nonNullString << std::endl;
    });

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
    ref<String> string5 = "  Hello World  ";
    assert(string0 == string1);
    assert(!(string0 != string1));
    assert(string0 == string5->trim());

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

    // down cast
    ref<Object> object = string0;
    assert(object == string1);
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
        std::cout << element << std::endl;
    }
    assert(list->size() == 5);
    assert(list[0] == "A");
    assert(list[0] == "B");
    assert(list[0] == "C");
    assert(list[0] == "D");
    assert(list[0] == "E");

    std::cout << "list connect: " << String::connect(list[0], list[1], list[2], list[3], list[4]) << std::endl;
    std::cout << "list[0] == 'A': " << (list[0] == "A") << std::endl;
    std::cout << "list length: " << list->size() << std::endl;
}

static void stringSubString()
{
    ref<String> base = "  .Hello..  ";
    ref<String> trimSpace = base->trim();
    ref<String> trimDot = trimSpace->trim(".");
    assert(trimSpace == ".Hello..");
    assert(trimDot == "Hello");
    assert(trimSpace->findLastNotOf('o', 5) == 4);
}

static void stringCaseTransform()
{
    ref<String> origin = "Abc.;'213";
    auto upper = origin->toUpperCase();
    auto lower = origin->toLowerCase();
    assert(upper == "ABC.;'213");
    assert(lower == "abc.;'213");
}

static void stringMap()
{
    /* option<String> is totally fine as Map's key */
    ref<Map<option<String>, int>> map = {
        {"A", 1},
        {"B", 2},
    };

    map[nullptr] = 3;
    assert(map->size() == 3);
    assert(map["A"] == 1);
    assert(map["B"] == 2);
    assert(map[nullptr] == 3);

    std::cout << map << std::endl;

    map["A"] = 5;
    map["C"] = 3;
    map[nullptr] = 4;
    assert(map->size() == 4);
    assert(map["A"] == 5);
    assert(map["B"] == 2);
    assert(map["C"] == 3);
    assert(map[nullptr] == 4);

    std::cout << map << std::endl;
}
