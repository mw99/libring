#!/bin/bash

TARGET="testcases-static"
SRC="testcases.c"

## FLAGS
CFLAGS="-O2 -std=c99 -pipe -Winline -Wall -Wextra -Werror -Wno-unused"
LDLIBS="libring.a"
CC="gcc"

make && $CC $CFLAGS -o $TARGET $SRC $LDLIBS && ./$TARGET

