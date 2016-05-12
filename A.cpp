#include "shared.hpp"

int main(int argc, char* argv[]){

  srand(time(NULL));
  
  int c;
  opterr = 0;
  const char* options = "lh";
  bool load = false;//, check = false;//, pull = false;

  const string helpMessage = "To run the full program:\n 1) to/from Data Hold: ./A with options \n\t-l for (l)oading data into the data hold\n\n 2) to/from the checking process: ./B \n\n 3) to finish (i.e. clean up): ./clean \n\n\n";

  while( (c = getopt(argc, argv, options)) != -1){
    switch(c){
      //Loading data into memory
    case 'l':
      load = true;
      break;
  
    case 'h':
    default:
      std::cout<<helpMessage;
      return 1;
    }
  }

  try{
    //create shared memory
    managed_shared_memory segment(open_or_create, "segment", DATA_HOLD_SIZE*32*sizeof(SerializedEvent));
  
    //create queues for communication between processes
    message_queue *toCheckQ = new message_queue(open_or_create, "toCheck", TO_Q_SIZE, sizeof(int));
    message_queue *fromCheckQ = new message_queue(open_or_create, "fromCheck", FROM_Q_SIZE, sizeof(l1TriggerResponse));

    //Load Data Into Memory Initially
    if(load){
      //alloc_inst.allocate(DATA_HOLD_SIZE*sizeof(int));
      for(uint event_id = 1; event_id <= DATA_HOLD_SIZE; event_id++){
	//Creating an Event with random content
	Event temp_event = newEvent();
	//Serializing the event
	SerializedEvent temp_serialized_event = serializeEvent(temp_event);

	//Store the serialized event in the shared memory
	segment.construct<SerializedEvent>(label(event_id))(temp_serialized_event);
      }

      std::cout<<"Loaded: "<< DATA_HOLD_SIZE <<" Serialized event into memory\n";
    }

    //Continuously send data to be checked and receive data from being checked
    uint event_id_to_process = 1;
    l1TriggerResponse *response = (l1TriggerResponse *)malloc(sizeof(l1TriggerResponse *));
    while (1) {
      
      //Sending data to be checked
      while( !toCheckQ->try_send(&event_id_to_process, sizeof(int), priority) ){
	usleep(1000);
      }
      std::cout<<"Sended event id: "<<event_id_to_process<<"\n";
      event_id_to_process++;
      //Loop back to beginning of shared memory at end
      if( event_id_to_process > DATA_HOLD_SIZE ) event_id_to_process = 1;


      //Receiving data from being checked
      while( !fromCheckQ->try_receive((void *)response, sizeof(std_resp), recvd_size, priority) ){ 
         usleep(1000); 
      }

      if( recvd_size != sizeof(std_resp) ) exit(1);

      std::cout<<"Received: "<< response->event_id << " Result: "<< response->l1_result <<"\n";

      if (response->l1_result) {
	//good event send L1 request
	std::cout<<"Sending L1 request for event id: "<< response->event_id <<"\n";
      } else {
        //Bad event 
	std::cout<<"Discard event id: "<< response->event_id <<"\n";
      }   
    }  
  } catch(interprocess_exception& e) {
    std::cout<<e.what()<<std::endl;
  }
  return 0;
}
