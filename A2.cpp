#include "shared.hpp"

int main(int argc, char *argv[]){

  srand(time(NULL));
  int offset = 0;

  int c;
  opterr = 0;
  const char* options = "lc::ph";
  bool load = false, check = false, pull = false;

  std::string helpMessage = "To run the full program:\n 1) to/from Data Hold: ./A2 with options \n-l for (l)oading more data into the data hold, \n-c for enqueing data to be (c)hecked (with an optional argument to start pulling from later in the data hold) (NOTE: must be entered as -c15 NOT -c 15 or -c=15 for instance), \n-p for dequeing/(p)ulling data from being checked\n\n 2) to/from the checking process: ./B2 \n\n 3) to finish (i.e. clean up): ./ab2Clean \n\n\n";

  while( (c = getopt(argc, argv, options)) != -1 || argc == 1){
    switch(c){
      //Loading data into memory
    case 'l':
      load = true;
      break;

      //Enqueing data to be checked
    case 'c':
      check = true;
      if(optarg != NULL) offset = atoi(optarg);
      break;

      //Dequeuing data from being checked
    case 'p':
      pull = true;
      break;

      //User either didn't enter arguments or raised the help flag
    case 'h':
    default:
      std::cout<<helpMessage;
      return 1;
    }
  }

  std::cout<<offset<<"\n";

  try{

    //create shared memory and associated mapped region
    shared_memory_object shm(open_or_create, "shm", read_write);
    shm.truncate(DATA_HOLD_SIZE);
    mapped_region region(shm, read_write);
      
    //create queues for communication between processes
    message_queue *toCheckQ = new message_queue(open_or_create, "toCheck", TO_Q_SIZE, sizeof(data_type *));
    message_queue *fromCheckQ = new message_queue(open_or_create, "fromCheck", FROM_Q_SIZE, sizeof(data_type *));

  
    if(load){
      //Fill Shared Memory with "data"  
      int i = 0; 
      for(data_type* p = (data_type *)region.get_address(); p < (data_type *)(region.get_address() + DATA_HOLD_SIZE); p++){

	*p = (data_type)(rand() % 2);
      }
    }

    if(check){
      //Try to enqueue "data" to be checked
      for(int i = 0; i < DATA_HOLD_SIZE/sizeof(data_type); i++){
	if(i >= offset && !toCheckQ->try_send(&i, sizeof(data_type *), priority) ) 
	  break; //queue is full
      }
    }
  
    
    if(pull){
      //Try to dequeue "data" from being checked
      for(int i = 0; i < DATA_HOLD_SIZE/sizeof(data_type); i++){
	if( !fromCheckQ->try_receive((void *)temp, sizeof(data_type), recvd_size, priority) || recvd_size != sizeof(data_type) )
	  break; //the queue is empty or the data is corrupted

	std::cout<<*temp<<"\n";
      
	/*
	  data_type *p = (data_type *)malloc(sizeof(data_type *)); *p = *temp;
	  if( !dataQ->try_send((void *)p, sizeof(data_type), priority) )
	  break; //queue is full
	*/
      }
    }

  } catch(interprocess_exception& e) {
    std::cout<<e.what()<<std::endl;
  }
 


  return 0;
}
