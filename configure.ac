#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT(smoothd, 1.0.0, minfrin@sharp.fm)
AC_CONFIG_AUX_DIR(build-aux)
AM_INIT_AUTOMAKE([dist-bzip2])
AC_CONFIG_SRCDIR([smoothd.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
PKG_PROG_PKG_CONFIG

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([getopt])

PKG_CHECK_MODULES([URING], [liburing >= 2.0], [], [
    AC_MSG_ERROR([liburing 2.0+ is required but was not found. Please install liburing-dev / liburing-devel.])
])

AC_CONFIG_FILES([Makefile smoothd.spec])
AC_OUTPUT

