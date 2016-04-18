#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <typeinfo>
#include <boost/lexical_cast.hpp>
#include <boost/container/string.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

using namespace boost::interprocess;

typedef boost::container::string string;
typedef string data_type;
data_type std_data = "00"; data_type temp = std_data;

//typedef allocator<data_type, managed_shared_memory::segment_manager> data_allocator; 


const char* std_ID = "event_1024";
const char* std_ID_format = "event_%04d";
std::pair<data_type*, std::size_t> d;

uint DATA_HOLD_SIZE = 15;
uint TO_Q_SIZE = 5;
uint FROM_Q_SIZE = 5;


std::size_t recvd_size;
uint priority = 0;


char* label(uint n){

  char* ID = (char*)malloc(sizeof(std_ID));
  std::sprintf(ID, std_ID_format, n);
  
  return ID;
}

data_type newData(uint n){
  return (boost::lexical_cast<boost::container::string>(n % 2) + (boost::lexical_cast<boost::container::string>((n + 1) % 2)));
  //return (boost::lexical_cast<boost::container::string>(n));
}
