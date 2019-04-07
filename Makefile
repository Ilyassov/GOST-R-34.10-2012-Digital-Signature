CC=g++
CFLAGS=-c -Wall -O3

all: sign genpkey

g512sum: g512sum.o gsumfuncs.o
	$(CC) g512sum.o gsumfuncs.o -o g512sum

g256sum: g256sum.o gsumfuncs.o
	$(CC) g256sum.o gsumfuncs.o -o g256sum

g512sum.o: g512sum.cpp gsum.h const.h
	$(CC) $(CFLAGS) -c g512sum.cpp

g256sum.o: g256sum.cpp gsum.h const.h
	$(CC) $(CFLAGS) -c g256sum.cpp

gsumfuncs.o: gsumfuncs.cpp gsum.h const.h
	$(CC) $(CFLAGS) -c gsumfuncs.cpp

genpkey: genpkey.cpp functions.cpp
	$(CC) functions.cpp genpkey.cpp -o genpkey

sign: sign.cpp functions.o gsumfuncs.o
	$(CC) functions.o sign.cpp gsumfuncs.o -o sign

functions.o: functions.cpp structure.hpp parameters.hpp
	$(CC) $(CFLAGS) -c functions.cpp

clean:
	rm -rf *.o main g512sum g256sum genpkey sign
