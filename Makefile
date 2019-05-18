CC=g++
CFLAGS=-c -Wall -O3

all: genpkey convpkey
# all: sign genpkey convpkey

g512sum: g512sum.o gsumfuncs.o
	$(CC) g512sum.o gsumfuncs.o -o g512sum

g256sum: g256sum.o gsumfuncs.o
	$(CC) g256sum.o gsumfuncs.o -o g256sum

g512sum.o: g512sum.cpp gsum.h const.h
	$(CC) $(CFLAGS) -c g512sum.cpp

g256sum.o: src/g256sum.cpp
	$(CC) $(CFLAGS) src/g256sum.cpp

gsumfuncs.o: src/gsumfuncs.cpp
	$(CC) $(CFLAGS) src/gsumfuncs.cpp

genpkey: genpkey.o functions.o
	$(CC) functions.o genpkey.o -o genpkey

genpkey.o: src/genpkey.cpp
	$(CC) $(CFLAGS) src/genpkey.cpp

convpkey: convpkey.o functions.o BigUInt.o
	$(CC) functions.o convpkey.o BigUInt.o -o convpkey

convpkey.o: src/convpkey.cpp
	$(CC) $(CFLAGS) src/convpkey.cpp

# sign: sign.o functions.o gsumfuncs.o
# 	$(CC) functions.o sign.o gsumfuncs.o -o sign

# sign.o: src/sign.cpp
# 	$(CC) $(CFLAGS) src/sign.cpp

BigUInt.o: src/BigUInt.cpp
	$(CC) $(CFLAGS) src/BigUInt.cpp

functions.o: src/functions.cpp
	$(CC) $(CFLAGS) -c src/functions.cpp

clean:
	rm -rf *.o main g512sum g256sum genpkey sign convpkey

.PHONY: clean