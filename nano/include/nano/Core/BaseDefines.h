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
