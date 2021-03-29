# Simple Makefile for minizip that is used for update extraction
TARGET = minizip
CXX_FLAGS = -Wall
INCLUDE_PATH = # Add path to minizip include here
LIBRARY_PATH = # Add path to minizip here
LIBRARIES = -lz -lminizip

all:	minizip.o
	g++ minizip.o -o -L$(LIBRARY_PATH) $(LIBRARIES) $(TARGET)
	
minizip.o:
	g++ minizip.cpp -c $(INCLUDE_PATH) -o minizip.o
	
clean:
	rm -rf *.o extract
