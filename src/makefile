# Shared lib config
#all: $(OBJECTS)
#	$(CC) -o wizbosa $(OBJECTS) -lconfig++

CC=g++
LIBDIR=lib
BDIR=../builddir
ODIR=$(BDIR)/objdir
OBJECTS=wizbosa.o application.o console.o config.o sutils.o futils.o mutils.o nutils.o filefind.o


all: $(OBJECTS)
  $(CC) -o wizbosa $(OBJECTS) $(LIBDIR)/libconfig++.a --outdir=$(BDIR)

wizbosa.o: wizbosa.h wizbosa.cpp
  $(CC) -c wizbosa.cpp --outdir=$(ODIR)

application.o: application.h application.cpp wizbosa.h
  $(CC) -c application.cpp --outdir=$(ODIR)

console.o: ./wiz/console.h ./wiz/console.cpp
  $(CC) -c ./wiz/console.cpp --outdir=$(ODIR)

config.o: config.h config.cpp
  $(CC) -c config.cpp --outdir=$(ODIR)

sutils.o: ./wiz/utils/sutils.h ./wiz/utils/sutils.cpp
  $(CC) -c ./wiz/utils/sutils.cpp --outdir=$(ODIR)

futils.o: ./wiz/utils/futils.h ./wiz/utils/futils.cpp
  $(CC) -c ./wiz/utils/futils.cpp --outdir=$(ODIR)

mutils.o: ./wiz/utils/mutils.h ./wiz/utils/mutils.cpp
  $(CC) -c ./wiz/utils/mutils.cpp --outdir=$(ODIR)
  
nutils.o: ./wiz/utils/nutils.h ./wiz/utils/nutils.cpp
  $(CC) -c ./wiz/utils/nutils.cpp --outdir=$(ODIR)
  
filefind.o: ./wiz/filefind.h ./wiz/filefind.cpp
  $(CC) -c ./wiz/filefind.cpp --outdir=$(ODIR)
