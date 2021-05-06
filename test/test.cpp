
#include "main_activity.h"

int main(int argc, char **args)
{
    return runApp(Logger::file("main_activity.log", Object::create<MainActivity>()));
}
