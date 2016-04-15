#include "shared.hpp"

int main(int argc, char* argv[]){

  bool continuous = false;
  int wait_time = 0; //number of centiseconds between refresh

  int c;
  opterr = 0;
  const char* options = "c::";
  while( (c = getopt(argc, argv, options)) != -1) {
    switch(c){
    case 'c':
      continuous = true;
      if(optarg != NULL) wait_time = atoi(optarg);
      break;
    }

  }


  try{
    managed_shared_memory segment(open_only, "segment");

    do{
      for(int i = 0; i < DATA_HOLD_SIZE; i++){
	std::sprintf(name, "i_%04d", i);
	d = segment.find<data_type>(name);
	std::cout<<name<<": "<<*d.first<<"\n";
	
      }
      for(int i = 0; i < wait_time; i++) usleep(100000); //1 centisecond
    }while(continuous);

  


  }catch(interprocess_exception& e){

    std::cout<<e.what()<<"\n";
    exit(1);
  }


  return 1;
}
