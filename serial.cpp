#include <cstdlib>
#include <cstring>
#include <array>
#include <iostream>
#include <time.h>

struct SerialEventHeader {
	uint length;
	uint empty;
};

	//uint empty;
struct Event {
	uint event_id;
        char data[8];
};
//typedef char Event;
//typedef char l1_SerializedEvent[132];

typedef std::array< char, 128 > l1_SerializedEvent;
//typedef char [2] l1_SerializedEvent;


l1_SerializedEvent serializeL1Event(char* event, uint event_size) {
	l1_SerializedEvent seriale;
	SerialEventHeader header;
	header.length = event_size; 
        if (event_size > sizeof(l1_SerializedEvent)){
		std::cout<<"error"<<std::endl;
	}
	memcpy((char*)&seriale, (char*)&header, sizeof(header));
	memcpy(((char*) &seriale) + sizeof(header),  event, event_size);
	//memcpy(((char*) &seriale) + 32,  (char*) event, event_size);
        std::cout<< "Length "<< sizeof(event)<<std::endl;

	return seriale; // "array" being boost::array or std::array
}

char * unserializeL1Event( l1_SerializedEvent &seriale, uint &size) {

	SerialEventHeader* header;
        header = (SerialEventHeader*) &seriale;
	size = header->length;
        std::cout<< "Length "<< header->length<<std::endl;
        char * event = new char[header->length];
	memcpy(event,((char*) &seriale) + sizeof(header), header->length);
	//memcpy(event,((char*) &seriale) , header->length);

	return event; // "array" being boost::array or std::array
}

int main () {
	srand(time(0));

	std::cout<<"size in bytes: "<<sizeof(l1_SerializedEvent)<<std::endl;

	//Event event;
	//event.event_id = 4;
	//for (uint i =0 ; i< 8; i++) {
	//    event.data[i] = i;
	//}
	
	
	uint random_event_size = rand()%20 + 10;
	char * random_event = new char[random_event_size];
	for (uint i = 0; i < random_event_size; i++){
		random_event[i] = i;
		std::cout << i << " " << random_event[i] << std::endl;
	}
        std::cout<< "Length "<< random_event_size<<std::endl;
	uint unserialized_event_size;
	
	l1_SerializedEvent  serial_event;
	serial_event = serializeL1Event( random_event, random_event_size ); 
	


	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;

	char * point_to = (char*) &serial_event;
	for (uint i = 0 ; i < 20; i++) {
	    std::cout << *(point_to + i)<< std::endl;
	}
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;




	char * unserialized_event = unserializeL1Event( serial_event, unserialized_event_size ); 
	//std::cout<<unserialized_event_size;
	
	for (uint i = 0 ; i< unserialized_event_size ; i++) {
	    std::cout << i <<" "<<unserialized_event[i]<<std::endl;
	    if (unserialized_event[i] == random_event[i]){
			std::cout<<"ok";
		}else{

			std::cout<<"nok";
		}
	}
	return 1;
}
