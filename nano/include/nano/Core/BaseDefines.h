#pragma once

#if defined(__ANDROID__)
#define NANO_PLATFORM_ANDROID
#elif defined(__APPLE__)
#define NANO_PLATFORM_APPLE
#elif defined(_WIN32) || defined(_WIN64)
#define NANO_PLATFORM_WINDOWS
#elif defined(__EMSCRIPTEN__)
#define NANO_PLATFROM_WEB
#endif

#if defined(NANO_PLATFORM_WINDOWS)
#define NANO_DLLEXPORT __declspec(dllexport)
#define NANO_DLLIMPORT __declspec(dllimport)
#define NANO_FORCEINLINE __forceinline
#else
#define NANO_DLLEXPORT __attribute__((visibility("default")))
#define NANO_DLLIMPORT
#define NANO_FORCEINLINE __attribute__((always_inline))
#endif

#if defined(NANO_STATIC)
#define NANO_API
#else
#if defined(NANO_EXPORT)
#define NANO_API NANO_DLLEXPORT
#else
#define NANO_API NANO_DLLIMPORT
#endif
#endif

#include <cstddef>
#include <cstdint>

namespace Nano
{
using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using size_t = std::size_t;
} // namespace Nano
