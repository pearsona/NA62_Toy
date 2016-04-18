#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>

int main(int argc, char *argv[]){

  using namespace boost::interprocess;

  try{
    shared_memory_object::remove("shm");
    
    //message_queue::remove("data");
    message_queue::remove("toCheck");
    message_queue::remove("fromCheck");

  } catch(interprocess_exception& e){
    std::cout<<e.what()<<std::endl;
  }

  return 1;
}
