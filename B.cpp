#include "shared.hpp"

//Main
int main(int argc, char *argv[]){

  srand(time(NULL));

  //Benchmarking Variables
  //=======================
  uint l1_num = 0, l2_num = 0;


  try{
    
    //Initialize Links to Shared Memory and Queues
    //=============================================

    managed_shared_memory *l1_shm = new managed_shared_memory(open_or_create, "l1_shm", L1_MEM_SIZE*32*sizeof(l1_SerializedEvent));
    managed_shared_memory *l2_shm = new managed_shared_memory(open_or_create, "l2_shm", L2_MEM_SIZE*32*sizeof(l2_SerializedEvent));
    
    message_queue *toCheckQ = new message_queue(open_or_create, "toCheck", TO_Q_SIZE, sizeof(EventID));
    message_queue *fromCheckQ = new message_queue(open_or_create, "fromCheck", FROM_Q_SIZE, sizeof(TriggerResponse));

	

    //Dequeue data, decide whether to L1/L2 trigger on it, and enqueue result
    //========================================================================

    EventID *ev = new EventID;
    TriggerResponse response;
    l1_Event l1_temp_event;
    l2_Event l2_temp_event;
    bool result;

    while (1) {

      //Dequeing Data
      //==============
      while( !toCheckQ->try_receive((void *)ev, sizeof(EventID), recvd_size, priority) ){
	//usleep(10);
      }

      if( recvd_size != sizeof(EventID) ) continue;
		
      char* ID = label(ev->id);


      //L1 Triggering Decision
      //=======================
      if( ev->level == 1 ){
	l1_d = l1_shm->find<l1_SerializedEvent>(ID);
	if ( l1_d.first != 0 ) {
	  l1_temp_event = unserializeEvent(*l1_d.first);
	  result = computeL1Trigger(l1_temp_event);
	
	  response.level = 1;

	  //LOG_INFO("Received event: " << ev->id <<" Value: "<< l1_temp_event<<" L"<< ev->level<<" processing algorithm result: "<< result);

	  l1_num++;
			
	} else {
	  //LOG_INFO("Error couldn't find this piece of data in l1...");
	  continue;
	}
      }
      //L2 Triggering Decision
      //=======================
      else if( ev->level == 2 ){
	l2_d = l2_shm->find<l2_SerializedEvent>(ID);
	if ( l2_d.first != 0 ) {
	  l2_temp_event = unserializeEvent(*l2_d.first);
	  result = computeL2Trigger(l2_temp_event);

	  response.level = 2;

	  //LOG_INFO("Received event: " << ev->id <<" Value: "<< l2_temp_event<<" L"<< ev->level<<" processing algorithm result: "<< result);
	  
	  l2_num++;

	} else {
	  //LOG_INFO("Error couldn't find this piece of data in l2...");
	  continue;
		
	}

      }

		
      //Building and Enqueing Response
      //===============================
      response.event_id = ev->id;
      response.result = result;

      while( !fromCheckQ->try_send(&response, sizeof(TriggerResponse), priority) ){
	//usleep(10);
      }


      //Slowdown the code just for understand what happen
      //usleep(1000000);


      if( l1_num % 10 == 0 ) LOG_INFO(getpid()<<" / l1 / "<<l1_num);
      if( l2_num % 10 == 0 ) LOG_INFO(getpid()<<" / l2 / "<<l2_num);

    }
  } catch(interprocess_exception e){
    LOG_ERROR(e.what());
  }




  return 0;
}
