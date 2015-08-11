# libring version
VERSION = 1.1

# files
SRC = ring.c
OBJ = ${SRC:.c=.o}

# targets
TARGET_STATIC = libring.a
TARGET_SHARED = libring.so
TARGET_HEADER = ring.h

# paths
PREFIX = /usr

# flags
CFLAGS = -DVERSION=\"${VERSION}\" -std=c99 -O2 -Wall -Winline -Werror -Wextra

# compiler and linker
CC = gcc
AR = ar

# distribution files
DISTFILES = Makefile README.md LICENSE ${SRC} ${TARGET_HEADER}

############################################################################################
############################################################################################

all: ${TARGET_SHARED} ${TARGET_STATIC}

options:
	@echo libring build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "CC       = ${CC}"

${TARGET_STATIC}: ${SRC}
	${CC} -c ${CFLAGS} $<
	${AR} rcs ${TARGET_STATIC} ${OBJ}

${TARGET_SHARED}: ${SRC}
	${CC} -shared -o ${TARGET_SHARED} -fPIC ${CFLAGS} $<

clean:
	@echo clean up
	@rm -f ${OBJ} ${TARGET_SHARED} ${TARGET_STATIC} testcase

dist: clean
	@echo creating dist tarball
	mkdir -p libring-${VERSION}
	cp -R ${DISTFILES} libring-${VERSION}
	tar -cvzf libring-${VERSION}.tgz libring-${VERSION}
	rm -rf libring-${VERSION}

install: all
	@echo installing library files ${TARGET} to ${DESTDIR}${PREFIX}/lib
	@mkdir -p ${DESTDIR}${PREFIX}/lib
	@cp -f ${TARGET_SHARED} ${DESTDIR}${PREFIX}/lib
	@ln -f -s ${TARGET_SHARED} libring-${VERSION}.so
	@mv -f libring-${VERSION}.so ${DESTDIR}${PREFIX}/lib
	@cp -f ${TARGET_STATIC} ${DESTDIR}${PREFIX}/lib

uninstall:
	@echo removing library files from ${DESTDIR}${PREFIX}/lib
	@rm -f ${DESTDIR}${PREFIX}/lib/${TARGET_STATIC}
	@rm -f ${DESTDIR}${PREFIX}/lib/${TARGET_SHARED}
	@rm -f ${DESTDIR}${PREFIX}/lib/libring-${VERSION}.so


.PHONY: all options clean dist install uninstall
