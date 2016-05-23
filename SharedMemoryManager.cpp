/*#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <typeinfo>
#include <cmath>
#include <atomic>
#include <boost/timer/timer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/container/string.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include "structs/TriggerResponse.h"
#include "structs/EventID.h"
#include "utils/AExecutable.h"
#include "options/Logging.h"*/

#include "SharedMemoryManager.h"

namespace na62 {
uint SharedMemoryManager::l1_mem_size_;
uint SharedMemoryManager::l2_mem_size_;
uint SharedMemoryManager::to_q_size_;
uint SharedMemoryManager::from_q_size_;

boost::interprocess::managed_shared_memory* SharedMemoryManager::l1_shm_;
boost::interprocess::managed_shared_memory* SharedMemoryManager::l2_shm_;

boost::interprocess::message_queue * SharedMemoryManager::trigger_queue_;
boost::interprocess::message_queue * SharedMemoryManager::trigger_response_queue_;

void SharedMemoryManager::initialize(){

	l1_mem_size_ = 10;
	l2_mem_size_ = 10;
	to_q_size_ = 2;
	from_q_size_ = 2;

	try {
		l1_shm_ = new boost::interprocess::managed_shared_memory(boost::interprocess::create_only, "l1_shm", l1_mem_size_*32*sizeof(l1_SerializedEvent));
	} catch(boost::interprocess::interprocess_exception& e) {
		LOG_ERROR(e.what());
	}

	try {
		l2_shm_ = new boost::interprocess::managed_shared_memory(boost::interprocess::create_only, "l2_shm", l2_mem_size_*32*sizeof(l2_SerializedEvent));
	} catch(boost::interprocess::interprocess_exception& e) {
		LOG_ERROR(e.what());
	}

	try {
		trigger_queue_ = new boost::interprocess::message_queue(boost::interprocess::create_only, "trigger_queue", to_q_size_, sizeof(EventID));
	} catch(boost::interprocess::interprocess_exception& e) {
		LOG_ERROR(e.what());
	}

	try {
		trigger_response_queue_ = new boost::interprocess::message_queue(boost::interprocess::create_only, "trigger_response_queue", from_q_size_, sizeof(EventID));
	} catch(boost::interprocess::interprocess_exception& e) {
		LOG_ERROR(e.what());
	}
}


bool SharedMemoryManager::storeL1Event(uint event_id, l1_Event l1_event){
	std::pair<l1_SerializedEvent*, std::size_t> l1_d;
    l1_d = l1_shm_->find<l1_SerializedEvent>(label(event_id));

    if( !l1_d.first ){
    	EventID trigger_message;
    	trigger_message.id = event_id;
    	trigger_message.level = 1;

    	uint message_priority_ = 0;

		l1_SerializedEvent temp_serialized_event = serializeEvent(l1_event);
		l1_shm_->construct<l1_SerializedEvent>(ID)(temp_serialized_event);

  	   //Enqueue Data
  	   //=============
  	    while( !trigger_queue_->try_send(trigger_message, sizeof(EventID), priority) ){
  	  	  //TODO sleep for a while
  	    }
  	    //LOG_INFO("Sended event id: "<<ev->id<<" for l"<<ev->level<<" processing");
  	    return true;
    } else {
    	LOG_WARNING("Event: "<< event_id << "already in the memory!");
    	return false;
    }

}


//Producing Labels for l1_Events
//============================
static char* label(uint n){
	//TODO possible not use a new
	char* ID = new char[sizeof(std_ID)];
	std::sprintf(ID, std_ID_format, n);
	return ID;
}

//bool SharedMemoryManager::existL1SharedMemory() {
//	try {
//		l1_shm_ = new boost::interprocess::managed_shared_memory(boost::interprocess::create_only, "l1_shm", l1_mem_size_*32*sizeof(l1_SerializedEvent));
//		return true;
//	} catch(boost::interprocess::interprocess_exception& e) {
//		LOG_ERROR(e.what());
//		return false
//	}
//}









/*
// Type Definitions and Constants
//================================

typedef boost::container::string string;

typedef uint32_t l1_Event;
typedef uint32_t l1_SerializedEvent;
typedef uint64_t l2_Event;
typedef uint64_t l2_SerializedEvent;

static const char* std_ID = "event_1024";
static const char* std_ID_format = "event_%04d";

static std::pair<l1_SerializedEvent*, std::size_t> l1_d;
static std::pair<l2_SerializedEvent*, std::size_t> l2_d;


static std::size_t recvd_size;
static uint priority = 0;


static uint L1_MEM_SIZE = 10;
static uint L2_MEM_SIZE = 10;
static uint TO_Q_SIZE = 2;
static uint FROM_Q_SIZE = 2;





// Functions
//==========


//Producing Labels for l1_Events
//============================
static char* label(uint n){
  char* ID = new char[sizeof(std_ID)];
  std::sprintf(ID, std_ID_format, n);  
  return ID;
}

static char* label(char *s){
  return label(atoi(s));
}

/*
//Produce a New l1_Event
//====================
static l1_Event newEvent(){
  //Generate an event with random content
  //Generating Randon content number between 1 and 1000
  return (rand() % 1000) + 1;
}


//Serialization and Unserialization
//==================================
static l1_SerializedEvent serializeEvent(l1_Event event) {
  //Just do noting here now, will be implement the logic for serialize a farm event
  return (l1_SerializedEvent) event;
}

static l1_SerializedEvent serializeEvent(l2_Event event) {

  return (l2_SerializedEvent) event;
}

static l1_Event unserializeEvent(l1_SerializedEvent serialized_event) {
   //Just do noting here now, will be implement the logic for unserialize a farm event
   return (l1_Event) serialized_event;
}

static l2_Event unserializeEvent(l2_SerializedEvent serialized_event) {
  return (l2_Event) serialized_event;
}


//L1 to L2 Conversion
//====================
static l2_Event l1tol2(l1_Event event){
  return (l2_Event) event;
}
*/


/*
 * L1 trigger function
 */
/*static bool computeL1Trigger(l1_Event event) {
   if (event % 2 == 0) {
       return 1; //even
   }
   return 0; //odd
}*/


/*
 * L2 trigger function
 */
/*static bool computeL2Trigger(l2_Event event) {
  if( event % 3 == 0 ) {
    return 1; //divisible by 3
  }
  return 0; //divisible by 3
}*/
}
