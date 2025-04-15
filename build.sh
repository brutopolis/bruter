#!/bin/bash

# this script is meant for linux,
# all filepath are unix based, the commands are also unix based
# should work well on mac, though i haven't tested it, and have no plans to do so.
# mingw might work with some tweaks, but i haven't tested it, might do soon or later.

# libbruter, it is, bruter.c and bruter.h are platform independent, and should(and need to) work on any platform that has a C compiler.
# libbruter use no other libraries beside the standard C library
# if libbruter is does not work on a platform, it is a bug, and should be reported.

# usage function
usage() {
    echo "usage: $0 [--debug] [-cc || --compiler gcc] [-h || --help] [--extra 'extra cc tags'] [--no-shared] [--no-static]"
    exit 1
}

# origin
ORIGIN=$(pwd)

# default values
DEBUG=0
CC="gcc -Wformat=0"
EXTRA=""
NO_SHARED=0
NO_STATIC=0

# parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --debug) DEBUG=1; shift ;;
        -cc) CC="$2"; shift 2 ;;
        --compile) CC="$2"; shift 2 ;;
        --extra) EXTRA="$2"; shift 2 ;;
        --no-shared) NO_SHARED=1; shift ;;
        --no-static) NO_STATIC=1; shift ;;
        --help) usage ;;
        -h) usage ;;
        *) echo "unknown option: $1"; usage ;;
    esac
done

if [[ $DEBUG -eq 1 ]]; then
    DEBUGARGS='-g'
    echo "debug mode enabled"
else
    DEBUGARGS=""
fi

echo "compiler: $CC"

rm -rf build
mkdir -p build

if [[ $NO_SHARED -eq 0 ]]; then # also build a shared library
    echo "building shared library"
    $CC src/bruter.c -o build/libbruter.so -shared -fPIC -O3 -Iinclude $DEBUGARGS $EXTRA
fi

if [[ $NO_STATIC -eq 0 ]]; then
    echo "building static library"
    $CC src/bruter.c -o build/libbruter.a -c -O3 -Iinclude $DEBUGARGS $EXTRA
fi