CC=g++
CFLAGS1=-c -Wall -I/usr/include/boost -g
CFLAGS2=-L /lib -lrt -lpthread

all:
	make prepare

	$(CC) $(CFLAGS1) A3.cpp
	$(CC) $(CFLAGS2) A3.o -o A3


	$(CC) $(CFLAGS1) B3.cpp
	$(CC) $(CFLAGS2) B3.o -o B3


	$(CC) $(CFLAGS1) ab3Clean.cpp
	$(CC) $(CFLAGS2) ab3Clean.o -o ab3Clean

	$(CC) $(CFLAGS1) viewData.cpp
	$(CC) $(CFLAGS2) viewData.o -o viewData	

	make clean

prepare:
	-./ab3Clean
	-rm A3 B3 ab3Clean viewData

clean:
	-rm *.o
