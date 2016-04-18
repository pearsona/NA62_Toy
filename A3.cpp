#include "shared.hpp"


int main(int argc, char* argv[]){

  srand(time(NULL));

  int c;
  opterr = 0;
  const char* options = "lc::ph";
  bool load = false, check = false;//, pull = false;

  const string helpMessage = "To run the full program:\n 1) to/from Data Hold: ./A3 with options \n\t-l for (l)oading more data into the data hold, \n\t-c for enqueing data to be (c)hecked\n\n 2) to/from the checking process: ./B3 \n\n 3) to finish (i.e. clean up): ./ab3Clean \n\n\n";

  while( (c = getopt(argc, argv, options)) != -1 || argc == 1){
    switch(c){
      //Loading data into memory
    case 'l':
      load = true;
      break;

      //Enqueing data to be checked
    case 'c':
      check = true;
      break;

      //Dequeuing data from being checked
      /*case 'p':
      pull = true;
      break;*/

      //User either didn't enter arguments or raised the help flag
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
    message_queue *toCheckQ = new message_queue(open_or_create, "toCheck", TO_Q_SIZE, sizeof(std_ID));
    //    message_queue *fromCheckQ = new message_queue(open_or_create, "fromCheck", FROM_Q_SIZE, sizeof(std_ID));



    //Load Data Into Memory Initially
    if(load){
      //alloc_inst.allocate(DATA_HOLD_SIZE*sizeof(int));
      for(uint i = 0; i < DATA_HOLD_SIZE; i++){
	//alloc_inst.allocate(sizeof(i));
	segment.construct<data_type>(label(i))(newData(i));
      }

      std::cout<<"Loaded\n";

    }


    //Continuously Send Data to be Checked
    if(check){
      uint i = 0;
      while (1) {
	while( !toCheckQ->try_send(&i, sizeof(std_ID), priority) ){
	  usleep(1000);
	}
	usleep(1000);

	i++;
	//Loop back to beginning of shared memory at end
	if( i >= DATA_HOLD_SIZE ) i = 0;
      }
    }
  
  } catch(interprocess_exception& e) {
    std::cout<<e.what()<<std::endl;
  }
 

  return 0;
}
