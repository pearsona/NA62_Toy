CC=g++
CFLAGS1=-c -std=c++11 -Wall -I/usr/include/boost -g
CFLAGS2=-L /lib -lrt -lpthread

all:
	make prepare

	$(CC) $(CFLAGS1) A.cpp
	$(CC) $(CFLAGS2) A.o -o A


	$(CC) $(CFLAGS1) B.cpp
	$(CC) $(CFLAGS2) B.o -o B


	$(CC) $(CFLAGS1) Clean.cpp
	$(CC) $(CFLAGS2) Clean.o -o Clean


	$(CC) $(CFLAGS1) viewData.cpp
	$(CC) $(CFLAGS2) viewData.o -o viewData	


	make clean

prepare:
	-./Clean
	-rm A B Clean viewData

clean:
	rm *.o
