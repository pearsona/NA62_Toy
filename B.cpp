#include "shared.hpp"

//Main
int main(int argc, char *argv[]){

  srand(time(NULL));

  try{
    managed_shared_memory l1_shm(open_only, "l1_shm");
    managed_shared_memory l2_shm(open_only, "l2_shm");
    //Link to Queue
    message_queue *toCheckQ = new message_queue(open_only, "toCheck");
    message_queue *fromCheckQ = new message_queue(open_only, "fromCheck");
	
    //Try to dequeue "data" to be checked, check it, and try to enqueue it
    EventID *ev = new EventID;
    TriggerResponse response;
    while (1) {
      while( !toCheckQ->try_receive((void *)ev, sizeof(EventID), recvd_size, priority) ){
	usleep(1);
      }
      if( recvd_size != sizeof(EventID) ) continue;
		
      char* ID = label(ev->id);

      if( ev->level == 1 ){
	d = l1_shm.find<SerializedEvent>(ID);
	if ( d.first != 0 ) {
	  Event temp_event = unserializeEvent(*d.first);
	  bool result = computeL1Trigger(temp_event);
	
	  response.level = 1;
			
	} else {
	  LOG_INFO("Error couldn't find this piece of data in l1...");
	  continue;
	}
      }
      else if( ev->level == 2 ){
	l2_d = l2_shm.find<SerializedEvent>(ID);
	if ( l2_d.first != 0 ) {
	  Event temp_event = unserializeEvent(*l2_d.first);
	  bool result = computeL2Trigger(temp_event);

	  response.level = 2;
			
	} else {
	  LOG_INFO("Error couldn't find this piece of data in l2...");
	  continue;
		
	}

      }

		
		
      LOG_INFO("Received event: " << ev->id <<" Value: "<< temp_event<<" L"<< ev->level<<" processing algorithm result: "<< result);
      response.event_id = ev->id;
      response.result = result;
      while( !fromCheckQ->try_send(&response, sizeof(TriggerResponse), priority) ){
	usleep(10);
      }
      //Slowdown the code just for understand what happen
      usleep(1000000);
	

	
    }
  } catch(interprocess_exception e){
    LOG_ERROR(e.what());
  }

  return 0;

}
