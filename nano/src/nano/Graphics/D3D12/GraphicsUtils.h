#pragma once
#include "nano/Core/BaseDefines.h"
#include "nano/Platform/Windows/MinWindows.h"
#include <exception>

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

NANO_FORCEINLINE void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw std::exception();
    }
}
} // namespace Nano
