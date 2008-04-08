#!/bin/sh

gcc -DHAVE_CONFIG_H -I. -I. -I.. -I.. -I../src -I/usr/include/SDL -O0 -Wall -c -o playmidisdl.o playmidisdl.c

/bin/sh ../libtool --mode=link gcc  -O0 -Wall   -o playmidisdl  playmidisdl.o ../src/libtimidity.la  -lm -lSDLmain -lSDL
