#pragma once
#include "nano/Core/BaseDefines.h"

namespace Nano
{
template <typename T>
NANO_FORCEINLINE void SafeRelease(T &ptr)
{
    if (ptr)
    {
        ptr->Release();
        ptr = nullptr;
    }
}
} // namespace Nano
