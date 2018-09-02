#ifndef LIBVLC_LOG_INTEROP_H
#define LIBVLC_LOG_INTEROP_H

#include <stdarg.h>

#if defined (_WIN32) && defined (DLL_EXPORT)
# define EXPORT __declspec(dllexport)
#elif defined (__GNUC__) && (__GNUC__ >= 4)
# define EXPORT __attribute__((visibility("default")))
#else
# define EXPORT
#endif


typedef void (*libvlcFormattedLogCallback)(void *data, int level, const void *ctx, const char *message);

EXPORT void libvlcLogInteropCallback(void *data, int level, const void *ctx, const char *fmt, va_list args);
EXPORT void* makeCallbackData(libvlcFormattedLogCallback callback, void *data, int minLevel);
EXPORT void destroyCallbackData(void *callbackData);

#endif