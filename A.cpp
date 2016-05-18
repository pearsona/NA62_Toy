#include "shared.hpp"

//#include "QueueReceiver.h"

using namespace na62;

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

  LOG_INFO(sizeof(SerializedEvent)<<" "<<sizeof(EventID));

  //create shared memory
  try{
    managed_shared_memory l1_shm(open_or_create, "l1_shm", L1_MEM_SIZE*32*sizeof(SerializedEvent));

    //Load Data Into Memory Initially
    if(load){
      //alloc_inst.allocate(L1_MEM_SIZE*sizeof(int));
      for(uint event_id = 1; event_id <= L1_MEM_SIZE; event_id++){
	//Creating an Event with random content
	Event temp_event = newEvent();
	//Serializing the event
	SerializedEvent temp_serialized_event = serializeEvent(temp_event);
			
	//Store the serialized event in the shared memory
	l1_shm.construct<SerializedEvent>(label(event_id))(temp_serialized_event);
      }
      LOG_INFO("Loaded: "<< L1_MEM_SIZE <<" Serialized event into memory");
    }

    //Starting receiver
    QueueReceiver* receiver = new QueueReceiver();
    receiver->startThread("QueueReceiver");

    //Starting sender

    message_queue *toCheckQ = new message_queue(open_or_create, "toCheck", TO_Q_SIZE, sizeof(EventID));
		
    uint event_id_to_process = 1;
    while (1) {
      //Sending data to be checked
      EventID *ev;
      ev->id = event_id_to_process;
      ev->level = 1;
      
      //Check to make sure event is still waiting for L1 processing
      char *ID = label(event_id_to_process);
      d = l1_shm->find<SerializedEvent>(ID);
      if( !*d.first ) continue;

      while( !toCheckQ->try_send(ev, sizeof(EventID), priority) ){
	sleep(1);
      }
      LOG_INFO("Sended event id: "<<ev->id<<" for l"<<ev->level<<" processing");

      event_id_to_process++;
      //Loop back to beginning of shared memory at end
      if (event_id_to_process > L1_MEM_SIZE ){
	event_id_to_process = 1;
      }
    }
  } catch(interprocess_exception& e) {
    LOG_ERROR(e.what());
  }


  /*
   * Join other threads
   */
  AExecutable::JoinAll();
  return 0;
}
