# libvlcLogInterop

An interop helper for using libvlc's logging API in managed code, like [LibVLCSharp](https://code.videolan.org/videolan/LibVLCSharp/)

## The basics

If you are using a libvlc binding like `LibVlcSharp` and you want to get logs as strings in your code, just install this package. All the nasty details are hidden for you.

If you are trying to build a libvlc binding in C# or Java, you can use this library. Please read how it works below.

If you want to build a C# or Java library that needs to bind to a native library that asks for a printf-like callback, read our story and how we solved it.

## The need
By default, when you are writing an application using libvlc, logs are being printed to stderr. This might not be desirable, especially if several players are involved in the same application.

The libvlc [exposes an API](https://www.videolan.org/developers/vlc/doc/doxygen/html/group__libvlc__log.html) that lets the application get the log messages.

While this API is great for C/C++ applications using libvlc, its printf-like signature makes it really hard to integrate in higher level-languages like C#.

I tried several ways to make that work in pure C#, without luck.

This basic library is here to help you get those messages with a managed-friendly API.

## The API

This library exposes 3 functions:

```C
void libvlcLogInteropCallback(void *data, int level, const void *ctx, const char *fmt, va_list args)
```

This is the libvlc log callback itself. Get a pointer to this function and pass it to `libvlc_log_set`. Your language doesn't have to know what a va_list is, it's between libvlc and this libvlcLogInterop. In fact, the function should be considered as opaque.

However, you must pass an argument to `libvlc_log_set` so that this library knows its context and can call your code. This argument is created by calling this function:

```C
void* makeCallbackData(libvlcFormattedLogCallback callback, void *data, int minLevel)
```

It allocates an opaque struct that contains your managed `callback` and the info about the minimal log level you're interested in. 

This minimal log level is used to avoid formatting and marshalling logs you're not interested in.

You can optionally pass arbitrary data in the form of an opaque pointer.

```C
void destroyCallbackData(void *callbackData)
```

This destroys the callback data allocated with `makeCallbackData`


On your side, you will have to implement a callback that matches this signature:

```C
typedef void (*libvlcFormattedLogCallback)(void *data, int level, const void *ctx, const char *message);
```

## More technical stuffs

`va_list` handling is a nightmare, especially in C#.

One idea was to reverse-engineer what a `va_list` is on the supported platforms and use that knowledge to reimplement `va_copy` and `va_end` in .net.

That would hypothetically allow us to call `vsnprintf` and `vsprintf` from the C standard library.

You can find the work [here](https://github.com/jeremyVignelles/va-list-interop-demo).

The point is : it's really a hard work to list all platforms : some are documented, some are not... 

One thing finished to put a stop on that research: Once all this is done, you'll have to find a dynamic library that exposes both `vsnprintf` and `vsprintf`, and that is available on all platforms. At the time being, Windows does not provide such thing, and we can't assume that all other platforms have a shared libgcc runtime available.

Long story short : it was not practical at all.

Then, the idea grew that we could have a native proxy, built statically with the same compiler than libvlc. We don't have to link against libvlc though, the routing is made by the managed binding library.

## How to build

You will need docker (on linux, docker for windows, docker for mac...).
Then, in a bash, run

```
SUFFIX=beta01 ./build.sh
```

Just remove the env variable if you need a release NuGet package instead of a debug one.

Don't forget to increment the number at each version you have installed in your machine, if you want to avoid nuget caching issues.