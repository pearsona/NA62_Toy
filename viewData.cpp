#include "shared.hpp"

int main(int argc, char* argv[]){

  bool continuous = false;
  int wait_time = 0; //number of seconds between refresh

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
	std::sprintf(ID, std_ID_format, i);
	d = segment.find<data_type>(ID);
	std::cout<<ID<<": "<<*d.first<<"\n";
	
      }
      for(int i = 0; i < wait_time; i++) usleep(1000000); //1 second
    }while(continuous);

  


  }catch(interprocess_exception& e){

    std::cout<<e.what()<<"\n";
    exit(1);
  }


  return 1;
}
