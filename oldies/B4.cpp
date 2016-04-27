#include "shared.hpp"

//Prototypes
void process(data_type *d, uint randomness);


//Main
int main(int argc, char *argv[]){

  srand(time(NULL)); 

  try{
    managed_shared_memory segment(open_only, "segment");
    
  
    //Link to Queue
    message_queue *toCheckQ = new message_queue(open_only, "toCheck");
    //message_queue *fromCheckQ = new message_queue(open_only, "fromCheck");
   

    //Try to dequeue "data" to be checked, check it, and try to enqueue it
    int *i = (int *)malloc(sizeof(int *));
    while (1) {
      while( !toCheckQ->try_receive((void *)i, sizeof(std_ID), recvd_size, priority) ) {
	usleep(1);
      }

      if( recvd_size != sizeof(std_ID) ) exit(1);
      
      d = segment.find<data_type>(label(*i));
      if( d.first != 0 ) process(d.first, rand());
      else continue; //couldn't find this piece of data...

    }

  } catch(interprocess_exception e){
    std::cout<<e.what()<<std::endl;
  }
  
  return 0;
}


void process(data_type *d, uint randomness){
  //*d = randomness;
  //*d = (randomness % 2) ? "true" : "false";
  string zero =  boost::lexical_cast<boost::container::string>(0);
  string one =  boost::lexical_cast<boost::container::string>(1);
  *d = (randomness % 2) ? one + one : zero + zero;
}
