#include "nano.h"
#include <iostream>

int main(int argc, char *argv[])
{
    const char *module_name = nano_module_name();
    const char *version = nano_version();
    std::cout << "module_name : " << module_name << std::endl;
    std::cout << "version : " << version << std::endl;

    return 0;
}