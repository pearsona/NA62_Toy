CC=g++
CFLAGS1=-std=c++11 -c -Wall -I/usr/include/boost -g
CFLAGS2=-L /lib -lrt -lpthread -lboost_system -lboost_thread

all:
	make prepare

	$(CC) utils/AExecutable.cpp $(CFLAGS1)
	$(CC) QueueReceiver.cpp -I. $(CFLAGS1)
	$(CC) exceptions/NA62Error.cpp -I. $(CFLAGS1)


	$(CC) A.cpp -I. $(CFLAGS1)
	$(CC) A.o QueueReceiver.o AExecutable.o NA62Error.o -o A $(CFLAGS2)


	$(CC) B.cpp $(CFLAGS1)
	$(CC) B.o -o B $(CFLAGS2)


	$(CC) Clean.cpp $(CFLAGS1)
	$(CC) Clean.o -o Clean $(CFLAGS2)


	$(CC) viewData.cpp $(CFLAGS1)
	$(CC) viewData.o -o viewData $(CFLAGS2)


#	make clean

prepare:
	-./Clean
	-#rm A B Clean viewData

clean:
	rm *.o A B Clean
