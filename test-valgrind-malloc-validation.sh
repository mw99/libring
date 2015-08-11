#!/bin/bash

TARGET="testcases-valgrind"
SRC="testcases.c"

## FLAGS
CFLAGS="-O2 -std=c99 -pipe -Winline -Wall -Wextra -Werror -Wno-unused"
LDFLAGS=""
LDLIBS="libring.a"
CC="gcc"



function run_valgrind()
{
	VOFILE="valgrind_output.txt"

	valgrind --leak-check=full --log-file=$VOFILE ./$TARGET

	TMP=$(egrep -o 'All heap blocks were freed' $VOFILE)

	if [[ -n "$TMP" ]]; then
		echo "No memory wasted :)"
		echo $TMP
	else
		echo "Memory leaking :("
		grep -A 1000 'HEAP SUMMARY' $VOFILE 
	fi
}

make && $CC $CFLAGS $LDFLAGS -o $TARGET $SRC $LDLIBS && run_valgrind



