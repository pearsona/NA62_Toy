#include "shared.hpp"

//Prototypes
void process(data_type *d);


//Main
int main(int argc, char *argv[]){

  try{
    managed_shared_memory segment(open_only, "segment");
    
  
    //Link to Queue
    message_queue *toCheckQ = new message_queue(open_only, "toCheck");
    //message_queue *fromCheckQ = new message_queue(open_only, "fromCheck");
   

    //Try to dequeue "data" to be checked, check it, and try to enqueue it
    while (1) {
      while( !toCheckQ->try_receive((void *)name, sizeof("i_1024"), recvd_size, priority) || recvd_size != sizeof("i_1024") ) {
          usleep(1);
      }
      d = segment.find<data_type>(name);
      process(d.first);
    }

  } catch(interprocess_exception e){
    std::cout<<e.what()<<std::endl;
  }
  
  return 0;
}


void process(data_type *d){
  srand(time(NULL)); 
  *d = rand() % 2;
  usleep(100);
}
