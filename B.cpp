#include "shared.hpp"

//Main
int main(int argc, char *argv[]){

  srand(time(NULL)); 

  try{
    managed_shared_memory segment(open_only, "segment");    
  
    //Link to Queue
    message_queue *toCheckQ = new message_queue(open_only, "toCheck");
    message_queue *fromCheckQ = new message_queue(open_only, "fromCheck");
   
    //Try to dequeue "data" to be checked, check it, and try to enqueue it
    int *event_id_to_process = (int *)malloc(sizeof(int *));
    while (1) {
      while( !toCheckQ->try_receive((void *)event_id_to_process, sizeof(int), recvd_size, priority) ){
         usleep(1); 
      }
      
      if( recvd_size != sizeof(int) ) exit(1);
 
      char* ID = label(*event_id_to_process);
      d = segment.find<SerializedEvent>(ID);
      if( d.first != 0 ){	
	
	Event temp_event = unserializeEvent(* d.first);
	bool result = computeL1Trigger(temp_event);
	std::cout<<"Received event: " << *event_id_to_process <<" Value: "<< temp_event<<" Processing algorithm result: "<< result<<"\n";
        l1TriggerResponse response;
	response.event_id = *event_id_to_process;
	response.l1_result = result;
	
	while( !fromCheckQ->try_send(&response, sizeof(response), priority) ){ 
           usleep(10); 
        }
      }else{
	 std::cout<<"Error couldn't find this piece of data..."<<std::endl;
         continue; 
      }
      //Slowdown the code just for understand what happen
      usleep(1000000);
    }

  } catch(interprocess_exception e){
    std::cout<<e.what()<<std::endl;
  }
  
  return 0;
}
