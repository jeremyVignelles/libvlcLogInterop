#!/bin/bash
#
# This script builds the library using crossbuild (https://github.com/multiarch/crossbuild)
# 

function build {
    HOST="$1"
    echo "Building for $HOST..."
    shift
    mkdir -p bin/$HOST

    if [[ $HOST == *-mingw32 ]]; then
        SHARED_LIBRARY_OPTIONS=
        SHARED_LIBRARY_EXTENSION=".dll"
        EXECUTABLE_EXTENSION=".exe"
        CC=$HOST-gcc
        RPATH_FIX=
    else
        SHARED_LIBRARY_OPTIONS=-fPIC
        if [[ $HOST == *-darwin ]]; then
            SHARED_LIBRARY_EXTENSION=".dylib"
            RPATH_FIX='-Wl,-rpath,@loader_path'
        else
            SHARED_LIBRARY_EXTENSION=".so"
            RPATH_FIX='-Wl,-rpath,$ORIGIN'
        fi
        EXECUTABLE_EXTENSION=
    fi
    docker run --rm -v $(pwd):/workdir -e CROSS_TRIPLE=$HOST multiarch/crossbuild cc -shared -Wall -Werror -o bin/$HOST/libvlcLogInterop${SHARED_LIBRARY_EXTENSION} libvlcLogInterop.c ${SHARED_LIBRARY_OPTIONS}
    docker run --rm -v $(pwd):/workdir -e CROSS_TRIPLE=$HOST multiarch/crossbuild cc -Wall -Werror -o bin/$HOST/testLogInterop${EXECUTABLE_EXTENSION} test/main.c -Lbin/$HOST -lvlcLogInterop ${RPATH_FIX}
}

build x86_64-linux-gnu
build arm-linux-gnueabihf
build x86_64-w64-mingw32
build i686-w64-mingw32
build x86_64-apple-darwin
