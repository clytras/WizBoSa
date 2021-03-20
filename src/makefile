# Shared lib config
#all: $(OBJECTS)
#	$(CC) -o wizbosa $(OBJECTS) -lconfig++

CC=g++
LIBDIR=lib
BDIR=../builddir
ODIR=$(BDIR)/objdir
OBJECTS=$(ODIR)/wizbosa.o $(ODIR)/application.o $(ODIR)/console.o $(ODIR)/config.o $(ODIR)/sutils.o $(ODIR)/futils.o $(ODIR)/mutils.o $(ODIR)/nutils.o $(ODIR)/filefind.o
LIBCONFIGPP=/usr/lib/x86_64-linux-gnu/libconfig++.a

all: $(OBJECTS)
	$(CC) -o $(BDIR)/wizbosa $(OBJECTS) $(LIBCONFIGPP) -static

$(ODIR)/wizbosa.o: wizbosa.h wizbosa.cpp
	$(CC) -c wizbosa.cpp -o $(ODIR)/wizbosa.o

$(ODIR)/application.o: application.h application.cpp wizbosa.h
	$(CC) -c application.cpp -o $(ODIR)/application.o

$(ODIR)/console.o: ./wiz/console.h ./wiz/console.cpp
	$(CC) -c ./wiz/console.cpp -o $(ODIR)/console.o

$(ODIR)/config.o: config.h config.cpp
	$(CC) -c config.cpp -o $(ODIR)/config.o

$(ODIR)/sutils.o: ./wiz/utils/sutils.h ./wiz/utils/sutils.cpp
	$(CC) -c ./wiz/utils/sutils.cpp -o $(ODIR)/sutils.o

$(ODIR)/futils.o: ./wiz/utils/futils.h ./wiz/utils/futils.cpp
	$(CC) -c ./wiz/utils/futils.cpp -o $(ODIR)/futils.o

$(ODIR)/mutils.o: ./wiz/utils/mutils.h ./wiz/utils/mutils.cpp
	$(CC) -c ./wiz/utils/mutils.cpp -o $(ODIR)/mutils.o
		
$(ODIR)/nutils.o: ./wiz/utils/nutils.h ./wiz/utils/nutils.cpp
	$(CC) -c ./wiz/utils/nutils.cpp -o $(ODIR)/nutils.o
		
$(ODIR)/filefind.o: ./wiz/filefind.h ./wiz/filefind.cpp
	$(CC) -c ./wiz/filefind.cpp -o $(ODIR)/filefind.o
