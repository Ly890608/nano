#pragma once
#include "nano/Core/BaseDefines.h"
#include <fmt/format.h>
#include <string_view>

namespace Nano
{
enum ELogLevel
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

struct Log
{
    static void Print(ELogLevel level, std::string_view message);
    static void Flush();

    template <typename S, typename... Args, typename Char = fmt::char_t<S>>
    static std::string GetFullMessage(ELogLevel level, const S &format_str, Args &&... args)
    {
#ifdef NANO_PLATFORM_ANDROID
        return fmt::format(format_str, args...);
#else
        const std::string str = fmt::format(format_str, args...);
        switch (level)
        {
        case LOG_DEBUG:
            return fmt::format("[Nano][Debug]: {}", str);
        case LOG_INFO:
            return fmt::format("[Nano][Info]: {}", str);
        case LOG_WARNING:
            return fmt::format("[Nano][Warning]: {}", str);
        case LOG_ERROR:
            return fmt::format("[Nano][Error]: {}", str);
        default:
            return fmt::format("[Nano][Unknown]: {}", str);
        }
#endif
    }

    template <typename S, typename... Args, typename Char = fmt::char_t<S>>
    NANO_FORCEINLINE static void PrintFormat(ELogLevel level, const S &format_str, Args &&... args)
    {
        Print(level, GetFullMessage(level, format_str, args...));
    }
};

#if NANO_USE_LOG
#define NANO_DEBUG(...) Log::PrintFormat(LOG_DEBUG, __VA_ARGS__)
#define NANO_INFO(...) Log::PrintFormat(LOG_INFO, __VA_ARGS__)
#define NANO_WARNING(...) Log::PrintFormat(LOG_WARNING, __VA_ARGS__)
#define NANO_ERROR(...) Log::PrintFormat(LOG_ERROR, __VA_ARGS__)
#else
#define NANO_DEBUG(...)
#define NANO_INFO(...)
#define NANO_WARNING(...)
#define NANO_ERROR(...)
#endif

} // namespace Nano
