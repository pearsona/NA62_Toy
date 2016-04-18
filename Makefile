CC=g++
CFLAGS1=-c -std=c++11 -Wall -I/usr/include/boost -g
CFLAGS2=-L /lib -lrt -lpthread

all:
	make prepare

	$(CC) A3.cpp     $(CFLAGS1)
	$(CC) A3.o -o A3 $(CFLAGS2)

	$(CC) B3.cpp     $(CFLAGS1)
	$(CC) B3.o -o B3 $(CFLAGS2)

	$(CC) ab3Clean.cpp           $(CFLAGS1)
	$(CC) ab3Clean.o -o ab3Clean $(CFLAGS2)

	$(CC) viewData.cpp           $(CFLAGS1)
	$(CC) viewData.o -o viewData $(CFLAGS2) 


	make clean

prepare:
	-./ab3Clean
	-rm A3 B3 ab3Clean viewData

clean:
	-rm *.o
