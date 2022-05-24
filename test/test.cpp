
#include "main_activity.h"

int main(int argc, char **args)
{
    runApp(Logger::file(Object::create<MainActivity>(), "main_activity.log"));
    return 0;
}
