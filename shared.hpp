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

typedef uint32_t Event;
typedef uint32_t SerializedEvent;

static const char* std_ID = "event_1024";
static const char* std_ID_format = "event_%04d";

static std::pair<SerializedEvent*, std::size_t> d;
static std::pair<SerializedEvent*, std::size_t> l2_d;


static std::size_t recvd_size;
static uint priority = 0;


static uint L1_MEM_SIZE = 15;
static uint L2_MEM_SIZE = 10;
static uint TO_Q_SIZE = 5;
static uint FROM_Q_SIZE = 5;




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

static char* label(uint n){
  char* ID = (char*)malloc(sizeof(std_ID));
  std::sprintf(ID, std_ID_format, n);  
  return ID;
}

static char* label(char *s){
  return label(atoi(s));
}

static Event newEvent(){
  //Generate an event with random content
  //Generating Randon content number between 1 and 1000
  return (rand() % 1000) + 1;
}

static SerializedEvent serializeEvent(Event event) {
  //Just do noting here now, will be implement the logic for serialize a farm event
  return (SerializedEvent) event;
}

static Event unserializeEvent(SerializedEvent serialized_event) {
   //Just do noting here now, will be implement the logic for unserialize a farm event
   return (Event) serialized_event;
}



/*
 * L1 trigger function
 */
static bool computeL1Trigger(Event event) {
   if (event % 2 == 0) {
       return 1; //even
   }
   return 0; //odd
}


/*
 * L2 trigger function
 */
static bool computeL2Trigger(Event event) {
  if( event % 3 == 0 ) {
    return 1; //divisible by 3
  }
  return 0; //divisible by 3
}
