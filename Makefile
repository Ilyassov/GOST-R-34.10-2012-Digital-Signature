CC=g++
CFLAGS=-c -Wall -O2

all: sign genpkey convpkey verify g256sum g512sum

g512sum: g512sum.o gsumfuncs.o
	$(CC) g512sum.o gsumfuncs.o -o g512sum

g256sum: g256sum.o gsumfuncs.o
	$(CC) g256sum.o gsumfuncs.o -o g256sum

g512sum.o: src/g512sum.cpp
	$(CC) $(CFLAGS) src/g512sum.cpp

g256sum.o: src/g256sum.cpp
	$(CC) $(CFLAGS) src/g256sum.cpp

verify: verify.o functions.o gsumfuncs.o
	$(CC) verify.o functions.o gsumfuncs.o -o verify

genpkey: genpkey.o functions.o
	$(CC) functions.o genpkey.o -o genpkey

genpkey.o: src/genpkey.cpp
	$(CC) $(CFLAGS) src/genpkey.cpp

convpkey: convpkey.o functions.o
	$(CC) functions.o convpkey.o -o convpkey

convpkey.o: src/convpkey.cpp
	$(CC) $(CFLAGS) src/convpkey.cpp

sign: sign.o functions.o gsumfuncs.o
	$(CC) functions.o sign.o gsumfuncs.o -o sign

gsumfuncs.o: src/gsumfuncs.cpp
	$(CC) $(CFLAGS) src/gsumfuncs.cpp

sign.o: src/sign.cpp
	$(CC) $(CFLAGS) src/sign.cpp

functions.o: src/functions.cpp
	$(CC) $(CFLAGS) src/functions.cpp

verify.o: src/verify.cpp
	$(CC) $(CFLAGS) src/verify.cpp

clean:
	rm -rf *.o main g512sum g256sum genpkey sign convpkey verify

.PHONY: clean