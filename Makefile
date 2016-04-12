CC=g++
CFLAGS1=-c -Wall -I../boost_1_60_0 -g
CFLAGS2=-L /lib -lrt

all:
	make prepare

	$(CC) $(CFLAGS1) A2.cpp
	$(CC) $(CFLAGS2) A2.o -o A2


	$(CC) $(CFLAGS1) B2.cpp
	$(CC) $(CFLAGS2) B2.o -o B2


	$(CC) $(CFLAGS1) ab2Clean.cpp
	$(CC) $(CFLAGS2) ab2Clean.o -o ab2Clean

	$(CC) $(CFLAGS1) viewData.cpp
	$(CC) $(CFLAGS2) viewData.o -o viewData	

	make clean

prepare:
	-./ab2Clean
	-rm A2 B2 ab2Clean viewData

clean:
	-rm *.o