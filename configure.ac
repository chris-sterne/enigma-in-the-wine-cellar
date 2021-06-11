dnl Process this file with autoconf to produce a configure script.

AC_INIT([enigma-in-the-wine-cellar], 1.0.1, [chris_sterne@hotmail.com])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CXX

PKG_CHECK_MODULES(ENIGMA_IN_THE_WINE_CELLAR,
[gtkmm-3.0 >= 3.24.0]
[glesv2 >= 3.2]
[vorbisfile >= 1.3.7]
[libpulse-mainloop-glib >= 14.2])

AC_OUTPUT([Makefile src/Makefile src/meshes/Makefile])
