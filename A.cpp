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
	event.length = rand()%2000 + 200;
	event.data = new char[event.length];
	for (uint i = 0; i < event.length; i++){
		event.data[i] = i % 256;
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



	uint total_processed = 0;

	while (1) {
		Event new_event = newEvent(total_processed++);
		LOG_INFO("Created event id: "<<total_processed<<" for l1 processing. Size: "<<new_event.length);


		while (! SharedMemoryManager::storeL1Event(new_event)) {
			LOG_INFO("No memory left impossible insert the fragment, sleeping for a second");
			usleep(10);
		}
		delete[] new_event.data;

		LOG_INFO("Total Processed: "<<total_processed<<"\n\n");

	//	usleep(100000);
	}

	/*
	 * Join other threads
	 */
	AExecutable::JoinAll();
	return 0;
}
