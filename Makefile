CC=g++
CFLAGS1=-c -std=c++11 -Wall -I/usr/include/boost -g
CFLAGS2=-L /lib -lrt -lpthread

all:
	make prepare

	$(CC) A.cpp $(CFLAGS1)
	$(CC) A.o -o A $(CFLAGS2)


	$(CC) B.cpp $(CFLAGS1)
	$(CC) B.o -o B $(CFLAGS2)


	$(CC) Clean.cpp $(CFLAGS1)
	$(CC) Clean.o -o Clean $(CFLAGS2)


	$(CC) viewData.cpp $(CFLAGS1)
	$(CC) viewData.o -o viewData $(CFLAGS2)


	make clean

prepare:
	-./Clean
	-rm A B Clean viewData

clean:
	rm *.o
