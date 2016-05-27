#include <cstdlib>
#include <cstring>
#include <array>
#include <iostream>

struct Event {
	uint event_id;
	char data[8];
};

//typedef char Event;
//typedef char l1_SerializedEvent[132];

typedef std::array< char, 12 > l1_SerializedEvent;
//typedef char [2] l1_SerializedEvent;

l1_SerializedEvent serializeL1Event( Event &event ) {
	l1_SerializedEvent seriale;
	memcpy(&seriale, &event.event_id, 12);
	//memcpy(&seriale + 4, &event.data, 104);
	return seriale; // "array" being boost::array or std::array
}

Event unserializeL1Event( l1_SerializedEvent &seriale ) {
	Event event;

        //event.event_id = reinterpret_cast<uint>(seriale);
	memcpy(&event, &seriale, 12);
	//memcpy(&event.data, &seriale + 4, 104);
	return event; // "array" being boost::array or std::array
}

int main () {

Event event;
event.event_id = 4;
for (uint i =0 ; i< 8; i++) {
    event.data[i] = i;
}

//char * point_to = (char*) &event.event_id;
//for (uint i = 0 ; i < 4; i++) {
//    std::cout << std::hex <<*(point_to + i);
//    if (i%2 == 0) std::cout<<" ";
//}
//std::cout<<std::endl;


l1_SerializedEvent  serial_event;
serial_event = serializeL1Event( event ); 

//point_to = (char*) &serial_event;
//for (uint i = 0 ; i < 4; i++) {
//    std::cout << std::hex <<*(point_to + i);
//    if (i%2 == 0) std::cout<<" ";
//}
//std::cout<<std::endl;

//std::cout<<sizeof(uint);
Event unserialized;
unserialized = unserializeL1Event( serial_event ); 
std::cout<<unserialized.event_id;

for (uint i =0 ; i< 8; i++) {
    std::cout << i <<" "<<event.data[i]<<std::endl;
}



return 1;
}
