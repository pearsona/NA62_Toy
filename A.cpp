#include "SharedMemoryManager.h"

using namespace na62;


int main(int argc, char* argv[]){

  srand(time(NULL));
  

  SharedMemoryManager::initialize();
 // if (SharedMemoryManager::eraseL1SharedMemory()) {
//	  LOG_INFO("l1 memory erased");
  //}

  //Create & Fill Shared Memory, Start Receiver, and Enqueue Data
  //==============================================================



  //Starting Receiver
  //==================
  //QueueReceiver* receiver = new QueueReceiver();
  //receiver->startThread("QueueReceiver");

  try{

    //Creating and Enqueing Events
    //=============================
    uint event_id_to_process = 0;
    EventID *ev = new EventID;
    
    while (1) {
      usleep(10000);
      ev->id = event_id_to_process;
      ev->level = 1;

      
      //Check to Make Sure Event is Still Waiting for L1 Processing
      // If Not, Add New Event
      //============================================================
      char *ID = label(event_id_to_process);
      l1_d = l1_shm->find<l1_SerializedEvent>(ID);
      
      if( !l1_d.first ){
    	  l1_Event temp_event = newEvent();
    	  l1_SerializedEvent temp_serialized_event = serializeEvent(temp_event);
    	  l1_shm->construct<l1_SerializedEvent>(ID)(temp_serialized_event);
      }

      //Enqueue Data
      //=============
      while( !toCheckQ->try_send(ev, sizeof(EventID), priority) ){
    	  //usleep(10);
      }
      //LOG_INFO("Sended event id: "<<ev->id<<" for l"<<ev->level<<" processing");

      event_id_to_process++;
      event_id_to_process %= L1_MEM_SIZE + 1;


      //usleep(1000000);
    }


  } catch(interprocess_exception& e) {
    LOG_ERROR(e.what());
  }



  /*
   * Join other threads
   */
  //AExecutable::JoinAll();

  return 0;
}
