#include "nano.h"
#include <iostream>

int main(int argc, char *argv[])
{
    const char *version = nano_version();
    const char *module_name = nano_module_name();
    std::cout << version << std::endl;
    std::cout << module_name << std::endl;
    return 0;
}