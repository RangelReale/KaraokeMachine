noinst_PROGRAMS = playtimidity

playtimidity_SOURCES = playtimidity.cpp
playtimidity_CXXFLAGS = -I/usr/include/SDL
playtimidity_LDADD = $(top_builddir)/src/tse3/libtse3.la -ltimidity -lSDL

DISTCLEANFILES = ./.deps/* ./.deps/.P

INCLUDES = -I$(top_srcdir)/src
