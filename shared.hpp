#include <cstdlib>
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
#include "options/Logging.h"


using namespace boost::interprocess;



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




// NA62 Namespace
//================

namespace na62 {

  class QueueReceiver: public AExecutable {
  public:
    QueueReceiver();
    virtual ~QueueReceiver();
    static boost::interprocess::message_queue *fromCheckQ_;
    static boost::interprocess::message_queue *toCheckQ_;
    static boost::interprocess::managed_shared_memory *l1_shm;
    static boost::interprocess::managed_shared_memory *l2_shm;
    //message_queue *fromCheckQ = new message_queue(open_or_create, "fromCheck", FROM_Q_SIZE, sizeof(l1TriggerResponse));

  private:
    virtual void thread() override;
    virtual void onInterruption() override;
    std::atomic<bool> running_;
  };

}






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



/*
 * L1 trigger function
 */
static bool computeL1Trigger(l1_Event event) {
   if (event % 2 == 0) {
       return 1; //even
   }
   return 0; //odd
}


/*
 * L2 trigger function
 */
static bool computeL2Trigger(l2_Event event) {
  if( event % 3 == 0 ) {
    return 1; //divisible by 3
  }
  return 0; //divisible by 3
}
