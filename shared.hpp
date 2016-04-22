#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <typeinfo>
#include <cmath>
#include <boost/lexical_cast.hpp>
#include <boost/container/string.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

using namespace boost::interprocess;


typedef boost::container::string string;

typedef uint32_t data_type;
data_type std_data = 0;

typedef int resp_type;
resp_type std_resp = 0;

/*typedef char* resp_type;
  resp_type std_resp = "00000";*/

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

char* label(char *s){
  return label(atoi(s));
}


data_type newData(uint n){
  //return (boost::lexical_cast<boost::container::string>(n % 2) + (boost::lexical_cast<boost::container::string>((n + 1) % 2)));
  //return (boost::lexical_cast<boost::container::string>(n));

  return n;
}

/*
void process(data_type *d, uint randomness){
  *d = randomness;
  *d = (randomness % 2) ? "true" : "false";
  string zero =  boost::lexical_cast<boost::container::string>(0);
  string one =  boost::lexical_cast<boost::container::string>(1);
  *d = (randomness % 2) ? one + one : zero + zero;
}
*/

char parity(data_type* d){
  return boost::lexical_cast<uint>(*d) % 2 ? '1' : '0';
}



resp_type serialize(char b, data_type* x){
  resp_type r = *x;
  if( b == '1' ) r *= -1; //odd --> negative
  
  return r;
 

  /*char resp[5];// = (resp_type)malloc(sizeof(std_resp)*5);
  resp[0] = b;
  resp[1] = (char)x;

  return resp;
  */
}

int deserialize(resp_type r){
  /*string s = boost::lexical_cast<boost::container::string>(std::abs(x));

  return ( x > 0 ? "0" : "1" ) + s;
  */

  /*char c[4];
  for(int i = 0; i < 4; i++) c[i] = r[i + 1];
  */

  return r;//atoi(r);
}
