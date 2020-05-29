#include "Core/Log.h"
#include <stdio.h>
#ifdef NANO_PLATFORM_ANDROID
#include <android/log.h>
#endif

namespace Nano
{
void Log::Print(ELogLevel level, std::string_view message)
{
#ifdef NANO_PLATFORM_ANDROID
    switch (level)
    {
    case LOG_DEBUG:
        __android_log_write(ANDROID_LOG_DEBUG, "Nano", "%s", message.data());
        break;
    case LOG_INFO:
        __android_log_write(ANDROID_LOG_INFO, "Nano", "%s", message.data());
        break;
    case LOG_WARNING:
        __android_log_write(ANDROID_LOG_WARN, "Nano", "%s", message.data());
        break;
    case LOG_ERROR:
        __android_log_write(ANDROID_LOG_ERROR, "Nano", "%s", message.data());
        break;
    }
#else
    switch (level)
    {
    case LOG_DEBUG:
    case LOG_INFO:
    case LOG_WARNING:
        fprintf(stdout, "%s", message.data());
        break;
    case LOG_ERROR:
        fprintf(stderr, "%s", message.data());
        break;
    }
#endif
}

void Log::Flush()
{
#ifdef NANO_PLATFORM_ANDROID
#else
    fflush(stdout);
#endif
}
} // namespace Nano
