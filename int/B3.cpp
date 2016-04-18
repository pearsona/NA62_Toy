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
    //char* ID = (char*)malloc(sizeof(std_ID));
    int *i = (int *)malloc(sizeof(int *));
    while (1) {
      while( !toCheckQ->try_receive((void *)i, sizeof(std_ID), recvd_size, priority) || recvd_size != sizeof(std_ID) ) {
          usleep(10);
      }
      d = segment.find<data_type>(label(*i));
      if(d.first != 0) process(d.first, rand() % 2);
      else std::cout<<"B3: "<<label(*i)<<"\n";
    }

  } catch(interprocess_exception e){
    std::cout<<e.what()<<std::endl;
  }
  
  return 0;
}


void process(data_type *d, data_type randomness){
  *d = randomness;
}
