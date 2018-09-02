#include "libvlcLogInterop.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct CallbackData {
    void* managedData;
    libvlcFormattedLogCallback managedCallback;
    int minLogLevel;
} CallbackData;

void libvlcLogInteropCallback(void *data, int level, const void *ctx, const char *fmt, va_list args)
{
    CallbackData* callbackData = (CallbackData*)data;
    if(level >= callbackData->minLogLevel)
    {
        va_list argsCopy;
        int length = 0;

        va_copy (argsCopy, args);
        length = vsnprintf(NULL, 0, fmt, argsCopy);
        va_end (argsCopy);

        char *str = malloc(length + 1);
        if (str != NULL)
        {
            va_copy (argsCopy, args);
            vsprintf (str, fmt, argsCopy);
            va_end (argsCopy);
        }
        else
        {
            // Failed to allocate log message, drop it.
            return;
        }

        callbackData->managedCallback(callbackData->managedData, level, ctx, str);

        free(str);
    }
}

void* makeCallbackData(libvlcFormattedLogCallback callback, void *data, int minLevel)
{
    CallbackData* result = malloc(sizeof(CallbackData));
    result->managedCallback = callback;
    result->managedData = data;
    result->minLogLevel = minLevel;
    return result;
}

void destroyCallbackData(void *callbackData)
{
    free(callbackData);
}