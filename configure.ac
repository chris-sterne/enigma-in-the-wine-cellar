dnl Process this file with autoconf to produce a configure script.

AC_INIT([enigma-in-the-wine-cellar], 5.1.4, [chris_sterne@hotmail.com])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CXX

PKG_CHECK_MODULES(ENIGMA_IN_THE_WINE_CELLAR,
[gtkmm-3.0 >= 3.24.0]
[glesv2 >= 3.2])

AC_OUTPUT([Makefile src/Makefile])
