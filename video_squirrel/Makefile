# Project: Video Squirrel

CPP  = g++ -D__DEBUG__
CC   = gcc -D__DEBUG__
OBJ  = video_squirrel.o avilib/avimisc.o avilib/avilib.o avilib/avidump.o realmedia_reader.o MPEGInfo.o $(RES)
LINKOBJ  = video_squirrel.o avilib/avimisc.o avilib/avilib.o avilib/avidump.o realmedia_reader.o MPEGInfo.o $(RES)
LIBS = `wx-config --libs` -lxml2
#-liconv
INCS =  -I"../libxml2-2.5.4/include"  -I"../matroska_cdl" 
CXXINCS =  -I"./" -I"../libxml2-2.5.4/include" -I"../matroska_cdl" 
BIN  = video_squirrel
CXXFLAGS = $(CXXINCS) `wx-config --cxxflags`
CFLAGS=-O3 -funroll-loops -ffast-math -DLINUX -Wall

.PHONY: all all-before all-after clean clean-custom

all: all-before video_squirrel all-after


clean: clean-custom
	rm -f $(OBJ) $(BIN)

$(BIN): $(LINKOBJ)
	$(CPP) $(LINKOBJ) -o "video_squirrel" $(LIBS)

video_squirrel.o: video_squirrel.cpp
	$(CPP) -c video_squirrel.cpp -o video_squirrel.o $(CXXFLAGS)

avilib/avimisc.o: avilib/avimisc.c
	$(CPP) -c avilib/avimisc.c -o avilib/avimisc.o $(CXXFLAGS) $(CFLAGS)

avilib/avilib.o: avilib/avilib.c
	$(CPP) -c avilib/avilib.c -o avilib/avilib.o $(CXXFLAGS) $(CFLAGS)

avilib/avidump.o: avilib/avidump.c
	$(CPP) -c avilib/avidump.c -o avilib/avidump.o $(CXXFLAGS) $(CFLAGS)

realmedia_reader.o: realmedia_reader.cpp
	$(CPP) -c realmedia_reader.cpp -o realmedia_reader.o $(CXXFLAGS)

MPEGInfo.o: MPEGInfo.cpp
	$(CPP) -c MPEGInfo.cpp -o MPEGInfo.o $(CXXFLAGS)
