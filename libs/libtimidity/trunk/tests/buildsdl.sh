#!/bin/sh

gcc -DHAVE_CONFIG_H -I. -I. -I.. -I.. -I../src -I../../SDL-1.2.13/include/SDL -O0 -Wall -c -o playmidisdl.o playmidisdl.c

/bin/sh ../libtool --mode=link gcc  -O0 -Wall   -o playmidisdl.exe  playmidisdl.o ../src/libtimidity.la  -lm -L../../SDL-1.2.13/lib -lmingw32 -lSDLmain -lSDL -mwindows