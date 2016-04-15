#include "shared.hpp"

int main(int argc, char *argv[]){

  try{

    srand(time(NULL)); 

    shared_memory_object shm(open_only, "shm", read_write);
    mapped_region region(shm, read_write);
    
  
    //Link to Queue
    message_queue *toCheckQ = new message_queue(open_only, "toCheck");
    message_queue *fromCheckQ = new message_queue(open_only, "fromCheck");
   

    //Try to dequeue "data" to be checked, check it, and try to enqueue it
    int *offset = (int *)malloc(sizeof(int *));
    while (1) {
      while( !toCheckQ->try_receive((void *)offset, sizeof(data_type *), recvd_size, priority) || recvd_size != sizeof(data_type *) ) {
          usleep(1);
      }
        //break; //the queue is empty or the data is corrupted

      temp = (data_type *)(region.get_address() + (*offset)*sizeof(data_type));
      *temp = (data_type)(rand() % 2);
      std::cout << "processing: "<< *temp <<std::endl;

      /*      if( !fromCheckQ->try_send(temp, sizeof(data_type), priority) )
                  break; //queue is full*/
    }

  } catch(interprocess_exception e){
    std::cout<<e.what()<<std::endl;
  }
  
  return 0;
}
