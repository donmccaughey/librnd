AM_CPPFLAGS = -D_SVID_SOURCE
AM_CFLAGS = -std=c99

lib_LIBRARIES = librnd.a
include_HEADERS = rnd.h
librnd_a_SOURCES = rnd.c

LDADD = $(lib_LIBRARIES)
check_PROGRAMS = rnd_tests
TESTS = $(check_PROGRAMS)

EXTRA_DIST = LICENSE README.md

