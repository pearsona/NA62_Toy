#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <time.h>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>

using namespace boost::interprocess;

typedef bool data_type;
data_type *temp = (data_type *)malloc(sizeof(data_type *));


static int DATA_HOLD_SIZE = 25*sizeof(data_type);
static int TO_Q_SIZE = 5;
static int FROM_Q_SIZE = 5;


std::size_t recvd_size;
unsigned int priority = 0;
