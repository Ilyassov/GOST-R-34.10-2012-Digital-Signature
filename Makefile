CC=g++
CFLAGS=-c -Wall -O3

all:	main

fileResWrite: fileResWrite.cpp functions.cpp
	$(CC) fileResWrite.cpp functions.cpp -o fileResWrite

genpkey: genpkey.cpp functions.cpp
	$(CC) functions.cpp genpkey.cpp -o genpkey

sign:	sign.cpp functions.cpp
	$(CC) functions.cpp sign.cpp -o sign

functions.o: functions.cpp structure.hpp parameters.hpp
	$(CC) $(CFLAGS) -c functions.cpp

clean:
	rm -rf *.o main
