CC=g++
CFLAGS1=-std=c++11 -c -I/usr/include/boost -g #-Wall
CFLAGS2=-L /lib -lrt -lpthread -lboost_system -lboost_thread

all:
	-./Clean
	-rm A B

	$(CC) utils/AExecutable.cpp $(CFLAGS1)
	$(CC) QueueReceiver.cpp -I. $(CFLAGS1)
	$(CC) exceptions/NA62Error.cpp -I. $(CFLAGS1)


	$(CC) A.cpp -I. $(CFLAGS1)
	$(CC) A.o QueueReceiver.o AExecutable.o NA62Error.o -o A $(CFLAGS2)


	$(CC) B.cpp $(CFLAGS1)
	$(CC) B.o -o B $(CFLAGS2)

	make clean

Clean:
	-rm Clean
	$(CC) Clean.cpp $(CFLAGS1)
	$(CC) Clean.o -o Clean $(CFLAGS2)
	rm Clean.o

clean:
	rm *.o
