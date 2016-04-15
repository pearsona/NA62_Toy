#include "shared.hpp"

int main(int argc, char *argv[]){

  srand(time(NULL));
  int offset = 0;

  int c;
  opterr = 0;
  const char* options = "lc::ph";
  bool load = false, check = false, pull = false;

  std::string helpMessage = "To run the full program:\n 1) to/from Data Hold: ./A3 with options \n-l for (l)oading more data into the data hold, \n-c for enqueing data to be (c)hecked (with an optional argument to start pulling from later in the data hold) (NOTE: must be entered as -c15 NOT -c 15 or -c=15 for instance), \n-p for dequeing/(p)ulling data from being checked\n\n 2) to/from the checking process: ./B3 \n\n 3) to finish (i.e. clean up): ./ab3Clean \n\n\n";

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


  try{
    //create shared memory
    managed_shared_memory segment(open_or_create, "segment", DATA_HOLD_SIZE*32*sizeof(data_type));
    //    data_allocator alloc_inst(segment.get_segment_manager());
      
    
    //create queues for communication between processes
    message_queue *toCheckQ = new message_queue(open_or_create, "toCheck", TO_Q_SIZE, sizeof("i_1024"));
    message_queue *fromCheckQ = new message_queue(open_or_create, "fromCheck", FROM_Q_SIZE, sizeof("i_1024"));




    if(load){

      //Fill Shared Memory with "data"  
      //alloc_inst.allocate(DATA_HOLD_SIZE*sizeof(int));
      for(int i = 0; i < DATA_HOLD_SIZE; i++){
	//alloc_inst.allocate(sizeof(i));
	
	std::sprintf(name, "i_%04d", i);
	//std::cout<<name<<"\n";
	segment.construct<data_type>(name)(i);
      }

      std::cout<<"Loaded\n";
      /*
      std::pair<int*, std::size_t> d = segment.find<data_type>("Integers");
      start = d.first;
      

      for(p = start; p < start + DATA_HOLD_SIZE; p++){
	std::cout<< *p <<"\n";
      }
      */

      //DATA_HOLD_SIZE *= sizeof(data_type);

    }

    if(check){
      int i = offset;
      while (1) {
	std::sprintf(name, "i_%04d", i);
	while(!toCheckQ->try_send(name, sizeof("i_1024"), priority) ){
	  usleep(1000);
	  //TODO exits after many attemps
	}
	//std::cout << "Pushing "<<i<<std::endl;
	(i  >= DATA_HOLD_SIZE/sizeof(data_type)) ? i = 0 : ++i;
	usleep(1000);
      }
      //Try to enqueue "data" to be checked
      //for(int i = 0; i < DATA_HOLD_SIZE/sizeof(data_type); i++){
      //  if(i >= offset && !toCheckQ->try_send(&i, sizeof(data_type *), priority) ) 
      //    break; //queue is full
      //}
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
