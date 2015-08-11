#!/bin/bash

TARGET="testcases-dynamic"
SRC="testcases.c"

## FLAGS
CFLAGS="-O2 -std=c99 -pipe -Winline -Wall -Wextra -Werror -Wno-unused"
LDFLAGS="-L. -lring"
CC="gcc"

make && $CC $CFLAGS $LDFLAGS -o $TARGET $SRC $LDLIBS && LD_LIBRARY_PATH=$PWD ./$TARGET

