#include "shared.hpp"

using namespace na62;


int main(int argc, char* argv[]){

  srand(time(NULL));


  //Read Command Line Arguments
  //============================

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




  //Create & Fill Shared Memory, Start Receiver, and Enqueue Data
  //==============================================================

  try{

    //Create & Fill (if -l option given on command line) Shared Memory
    //=================================================================
    managed_shared_memory *l1_shm = new managed_shared_memory(open_or_create, "l1_shm", L1_MEM_SIZE*32*sizeof(SerializedEvent));

    if( load ){
      for( uint event_id = 1; event_id <= L1_MEM_SIZE; event_id++ ){

	Event temp_event = newEvent();
	SerializedEvent temp_serialized_event = serializeEvent(temp_event);
	l1_shm->construct<SerializedEvent>(label(event_id))(temp_serialized_event);

      }
      LOG_INFO("Loaded: "<< L1_MEM_SIZE <<" Serialized event into memory");
    }



    //Starting Receiver
    //==================
    QueueReceiver* receiver = new QueueReceiver();
    receiver->startThread("QueueReceiver");



    //Enqueing Data
    //==============
    message_queue *toCheckQ = new message_queue(open_or_create, "toCheck", TO_Q_SIZE, sizeof(EventID));
		
    uint event_id_to_process = 1;
    uint num_discarded = 0;
    EventID *ev = new EventID;
    
    while (1) {
    
      ev->id = event_id_to_process;
      ev->level = 1;

      
      //Check to Make Sure Event is Still Waiting for L1 Processing
      //============================================================
      char *ID = label(event_id_to_process);
      l1_d = l1_shm->find<SerializedEvent>(ID);
      if( !l1_d.first )
	if( num_discarded++ >= L1_MEM_SIZE ) break;
	else continue;


      //Enqueue Data
      //=============
      while( !toCheckQ->try_send(ev, sizeof(EventID), priority) ){
	sleep(1);
      }
      LOG_INFO("Sended event id: "<<ev->id<<" for l"<<ev->level<<" processing");

      event_id_to_process = (event_id_to_process + 1) % L1_MEM_SIZE;

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
