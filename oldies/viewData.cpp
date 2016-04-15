#include "shared.hpp"

int main(int argc, char *argv[]){


  try{
    shared_memory_object shm(open_only, "shm", read_only);
    mapped_region region(shm, read_only);

    int i = 0;
    for(data_type* p = (data_type *)region.get_address(); p < (data_type *)(region.get_address() + DATA_HOLD_SIZE); p++) std::cout<<i++<<": "<<(data_type)(*p)<<"\n";

  } catch(interprocess_exception& e){
    std::cout<<e.what()<<"\n";
  }
  

  return 0;
}
