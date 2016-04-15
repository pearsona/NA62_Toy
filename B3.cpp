#include "shared.hpp"

int main(int argc, char *argv[]){

  try{
    srand(time(NULL)); 

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
      //std::cout << "processing: "<< *d.first <<std::endl;
      *d.first = rand() % 2;
      //std::cout << "processed: "<< *d.first <<std::endl;

      /*
      temp = (data_type *)(region.get_address() + (*offset)*sizeof(data_type));
      *temp = (data_type)(rand() % 2);*/
 

      /*      if( !fromCheckQ->try_send(temp, sizeof(data_type), priority) )
                  break; //queue is full*/
    }

  } catch(interprocess_exception e){
    std::cout<<e.what()<<std::endl;
  }
  
  return 0;
}
