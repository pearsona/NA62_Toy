#include "utils/AExecutable.h"
#include "sharedMemory/SharedMemoryManager.h"

#include "sharedMemory/QueueReceiver.h"
#include <boost/timer/timer.hpp>

using namespace na62;


//Produce a New l1_Event
//====================
// Will be moved
Event newEvent(){
  //Generate an event with random content
  //Generating Randon content number between 1 and 1000
  return (rand() % 1000) + 1;
}



int main(int argc, char* argv[]){

	srand(time(NULL));

	SharedMemoryManager::initialize();

	//Starting Receiver
	//==================
	QueueReceiver* receiver = new QueueReceiver();
	receiver->startThread("QueueReceiver");
	uint event_id_to_process = 0;
    uint total_processed = 0;



	while (1) {
		event_id_to_process++;
		total_processed++;
    	Event temp_event = newEvent();
    	LOG_INFO("Cretaed event id: "<<event_id_to_process<<" for l1 processing");
    	bool result = SharedMemoryManager::storeL1Event(event_id_to_process, temp_event);

    	event_id_to_process %= 10 + 1;

    	LOG_INFO("Total Processed: "<<total_processed);


    	//usleep(1000000);
    }

	/*
	 * Join other threads
	 */
	AExecutable::JoinAll();
	return 0;
}
