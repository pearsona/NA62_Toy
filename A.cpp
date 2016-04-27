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
    managed_shared_memory segment(open_or_create, "segment", DATA_HOLD_SIZE*32*sizeof(std_data));
    //    data_allocator alloc_inst(segment.get_segment_manager());
      
    
    //create queues for communication between processes
    message_queue *toCheckQ = new message_queue(open_or_create, "toCheck", TO_Q_SIZE, sizeof(int));
    message_queue *fromCheckQ = new message_queue(open_or_create, "fromCheck", FROM_Q_SIZE, sizeof(resp_type));



    //Load Data Into Memory Initially
    if(load){
      //alloc_inst.allocate(DATA_HOLD_SIZE*sizeof(int));
      for(uint i = 1; i <= DATA_HOLD_SIZE; i++){
	//alloc_inst.allocate(sizeof(i));
	segment.construct<data_type>(label(i))(newData(i));
      }

      std::cout<<"Loaded\n";

    }


    //Continuously send data to be checked and receive data from being checked
    uint i = 1;
    resp_type *r = (resp_type *)malloc(sizeof(resp_type *));
    while (1) {
      
      //Sending data to be checked
      while( !toCheckQ->try_send(&i, sizeof(int), priority) ){
	usleep(1000);
      }
      //usleep(1000);
      i++;
      //Loop back to beginning of shared memory at end
      if( i > DATA_HOLD_SIZE ) i = 1;


      //Receiving data from being checked
      while( !fromCheckQ->try_receive((void *)r, sizeof(std_resp), recvd_size, priority) ){ usleep(100); }

      if( recvd_size != sizeof(std_resp) ) exit(1);
      //std::cout<<"A: "<<r<<"\n";
      int x = deserialize(*r);

      //If odd, then make even
      if( x < 0  ){
	char* ID = label(std::abs(x));
	d = segment.find<data_type>(ID);

	if( d.first != 0 ){
	  *d.first *= 2;
	}
	else continue; //couldn't find this piece of data...
	
      }
      
    }


  
  } catch(interprocess_exception& e) {
    std::cout<<e.what()<<std::endl;
  }
 

  return 0;
}
