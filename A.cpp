#include <boost/timer/timer.hpp>
#include "utils/AExecutable.h"

#include "sharedMemory/SharedMemoryManager.h"
#include "sharedMemory/QueueReceiver.h"
#include "structs/SerialEvent.h"

using namespace na62;


//Produce a New l1_Event
//====================
// Will be moved
//Event newEvent(){
//Generate an event with random content
//Generating Randon content number between 1 and 1000
//  return (rand() % 1000) + 1;
//}



Event newEvent(uint event_id){
	Event event;
	event.event_id = event_id;
	event.length = rand()%20 + 10;
	event.data = new char[event.length];
	for (uint i = 0; i < event.length; i++){
		event.data[i] = i;
		//std::cout << i << " " << random_event[i] << std::endl;
	}

	return event;
}




int main(int argc, char* argv[]){
	srand(time(0));
	//srand(time(NULL));
	SharedMemoryManager::initialize();

	//Starting Receiver
	//==================
	QueueReceiver* receiver = new QueueReceiver();
	receiver->startThread("QueueReceiver");
	uint event_id_to_process = 0;
    uint total_processed = 0;

    Event event;

	while (1) {
		event_id_to_process++;
		total_processed++;
		uint size = rand() % 997 + 3;
    	//Event temp_event = newEvent(size);
    	event.event_id = event_id_to_process;



		uint random_event_size = rand()%20 + 10;
		char * random_event = new char[random_event_size];
		for (uint i = 0; i < random_event_size; i++){
			  random_event[i] = i;
			  //std::cout << i << " " << random_event[i] << std::endl;
		}


    	LOG_INFO("Cretaed event id: "<<event_id_to_process<<" for l1 processing. Size: "<<size);
    	Event new_event = newEvent(event_id_to_process);
    	while (! SharedMemoryManager::storeL1Event(new_event)) {
    		LOG_INFO("No memory left impossible insert the fragment, sleeping for a second");
    		usleep(1000000);
    	}
    	delete[] new_event.data;


    	//event_id_to_process %= 10 + 1;

    	LOG_INFO("Total Processed: "<<total_processed);


    	usleep(1000000);
    }

	/*
	 * Join other threads
	 */
	AExecutable::JoinAll();
	return 0;
}
