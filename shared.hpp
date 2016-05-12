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

typedef uint32_t Event;
typedef uint32_t SerializedEvent;
SerializedEvent std_data = 0;


struct l1TriggerResponse {
	uint event_id;
	bool l1_result;
};
l1TriggerResponse std_resp;


const char* std_ID = "event_1024";
const char* std_ID_format = "event_%04d";

std::pair<SerializedEvent*, std::size_t> d;

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

Event newEvent(){
  //Generate an event with random content
  //Generating Randon content number between 1 and 1000
  return (rand() % 1000) + 1; 
}

SerializedEvent serializeEvent(Event event) {
   //Just do noting here now, will be implement the logic for serialize a farm event
   return (SerializedEvent) event;
} 

Event unserializeEvent(SerializedEvent serialized_event) {
   //Just do noting here now, will be implement the logic for unserialize a farm event
   return (Event) serialized_event;
} 

/*
 * L1 trigger function
 */
bool computeL1Trigger(Event event) {
   if (event % 2 == 0) {
       return 1; //odd
   } 
   return 0; //even
}

