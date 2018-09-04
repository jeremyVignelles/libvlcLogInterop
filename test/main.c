#include "../libvlcLogInterop.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

enum libvlc_log_level { LIBVLC_DEBUG =0, LIBVLC_NOTICE =2, LIBVLC_WARNING =3, LIBVLC_ERROR =4 };

static int result = -1;

void formattedLogTestCallback(void *data, int level, const void *ctx, const char *message)
{
    if(strcmp("Hello, World ! You should see 2 emojis here : 👩‍💻 👨‍💻", message))
    {
        printf("Called with the wrong message %s \n", message);
        result = 1;
        return;
    }
    
    if(result != -1)
    {
        printf("Already called!\n");
        result = 2;
        return;
    }
    
    printf("Got correct message!\n");

    if(strcmp(data, "context"))
    {
        printf("data should be equal to \"context\"\n");
        result = 3;
        return;
    }

    if(ctx != NULL)
    {
        printf("ctx should be null\n");
        result = 4;
        return;
    }

    result = 0;
}

void sendLog(void *data, int level, const void *ctx, const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    libvlcLogInteropCallback(data, level, ctx, fmt, va);
    va_end(va);
}

int main(int argc, char** argv)
{
    void* callbackData = makeCallbackData(formattedLogTestCallback, "context", LIBVLC_WARNING);

    sendLog(callbackData, LIBVLC_DEBUG, NULL, "This should not be displayed");
    sendLog(callbackData, LIBVLC_NOTICE, NULL, "This should not be displayed");

    sendLog(callbackData, LIBVLC_WARNING, NULL, "Hello, %s ! You should see %ld emojis here : %s", "World", 2, "👩‍💻 👨‍💻");

    destroyCallbackData(callbackData);
    return result;
}

