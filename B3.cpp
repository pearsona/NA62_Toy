#include "shared.hpp"

//Prototypes
void process(data_type *d, data_type randomness);


//Main
int main(int argc, char *argv[]){

  srand(time(NULL)); 

  try{
    managed_shared_memory segment(open_only, "segment");
    
  
    //Link to Queue
    message_queue *toCheckQ = new message_queue(open_only, "toCheck");
    //message_queue *fromCheckQ = new message_queue(open_only, "fromCheck");
   

    //Try to dequeue "data" to be checked, check it, and try to enqueue it
    while (1) {
      while( !toCheckQ->try_receive((void *)ID, sizeof(std_ID), recvd_size, priority) || recvd_size != sizeof(std_ID) ) {
          usleep(1);
      }
      d = segment.find<data_type>(ID);
      process(d.first, rand() % 2);
    }

  } catch(interprocess_exception e){
    std::cout<<e.what()<<std::endl;
  }
  
  return 0;
}


void process(data_type *d, data_type randomness){
  *d = randomness;
}
