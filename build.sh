#!/bin/bash
#
# This script builds the library using dockcross (https://github.com/dockcross/dockcross)
# 
# I installed them using :
#
#   mkdir ~/bin
#   cd ~/bin
#   docker run --rm dockcross/linux-x64 > dockcross-linux-x64
#   chmod +x dockcross-linux-x64
#
# repeat for all images listed below. Make sure ~/bin is in your path
# (since the bin folder did not exist at the time of login, I had to source `~/.profile`, but restarting the session might be enough)
#

function build {
    COMPILER="$1"
    echo "Building for $COMPILER..."
    shift
    mkdir -p bin/$COMPILER

    if [[ $COMPILER == windows-* ]]; then
        SHARED_LIBRARY_OPTIONS=
        SHARED_LIBRARY_EXTENSION=".dll"
        EXECUTABLE_EXTENSION=".exe"
    else
        SHARED_LIBRARY_OPTIONS=-fPIC
        SHARED_LIBRARY_EXTENSION=".so"
        EXECUTABLE_EXTENSION=
    fi
    dockcross-${COMPILER} bash -c "\$CC -shared -Wall -Werror -o bin/$COMPILER/libvlcLogInterop${SHARED_LIBRARY_EXTENSION} libvlcLogInterop.c ${SHARED_LIBRARY_OPTIONS}"
    dockcross-${COMPILER} bash -c "\$CC '-Wl,-rpath=\$ORIGIN' -Wall -Werror -o bin/$COMPILER/testLogInterop${EXECUTABLE_EXTENSION} test/main.c -Lbin/$COMPILER -lvlcLogInterop"
}

build linux-x86
build linux-x64
build windows-x86
build windows-x64
build linux-armv7
