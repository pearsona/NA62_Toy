CC=g++
BOOST_INC=/usr/include/boost
BOOST_INC=/usr/local/include/boost/

CFLAGS1=-std=c++11 -c -I$(BOOST_INC) -g #-Wall



CFLAGS2=-L /lib -lrt -lpthread -lboost_system -lboost_thread

all:
	-rm A B Clean

	$(CC) utils/AExecutable.cpp $(CFLAGS1)
	$(CC) SharedMemoryManager.cpp $(CFLAGS1)
	$(CC) QueueReceiver.cpp -I. $(CFLAGS1)
	$(CC) exceptions/NA62Error.cpp -I. $(CFLAGS1)


	$(CC) A.cpp -I. $(CFLAGS1)
	$(CC) A.o QueueReceiver.o AExecutable.o NA62Error.o SharedMemoryManager.o -o A $(CFLAGS2)


	#$(CC) B.cpp $(CFLAGS1)
	#$(CC) B.o -o B $(CFLAGS2)


	$(CC) Clean.cpp $(CFLAGS1)
	$(CC) Clean.o SharedMemoryManager.o -o Clean $(CFLAGS2)

clean:
	rm *.o
