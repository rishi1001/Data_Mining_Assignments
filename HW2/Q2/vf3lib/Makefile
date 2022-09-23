CC=g++
CFLAGS= -std=c++11 -O3 -Wno-deprecated
LDFLAGS= -lpthread -latomic
CPPFLAGS= -I./include

all: vf3

vf3:
	$(CC) $(CFLAGS) $(CPPFLAGS) -o bin/$@ main.cpp

clean:
	rm bin/*
