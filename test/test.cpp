
#include "main_activity.h"

int main(int argc, char **args)
{
    runApp(Logger::file("main_activity.log", Object::create<MainActivity>()));
    return 0;
}
