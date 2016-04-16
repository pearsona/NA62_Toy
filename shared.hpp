#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <time.h>
#include <typeinfo>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

using namespace boost::interprocess;

typedef int data_type;
data_type *temp = (data_type *)malloc(sizeof(data_type *));

//typedef allocator<data_type, managed_shared_memory::segment_manager> data_allocator; 


const char* std_ID = "i_1024";
const char* std_ID_format = "i_%04d";
char* ID = (char *)malloc(sizeof(std_ID));
std::pair<data_type*, std::size_t> d;

unsigned int DATA_HOLD_SIZE = 15;
unsigned int TO_Q_SIZE = 2;
unsigned int FROM_Q_SIZE = 5;


std::size_t recvd_size;
unsigned int priority = 0;

