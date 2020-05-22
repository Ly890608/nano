#pragma once
#include "nano/Core/Application.h"
#include "nano/Core/BaseDefines.h"

#ifdef __cplusplus
extern "C"
{
#endif

    NANO_API const char *nano_version();
    NANO_API const char *nano_module_name();

#ifdef __cplusplus
}
#endif
