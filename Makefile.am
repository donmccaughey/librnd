include_HEADERS = rnd.h

AM_CFLAGS = -std=c99
lib_LIBRARIES = librnd.a
librnd_a_SOURCES = $(rnd_HEADERS) rnd.c

LDADD = $(lib_LIBRARIES)
check_PROGRAMS = test/rnd_test

TESTS = $(check_PROGRAMS)

EXTRA_DIST = LICENSE README.md

