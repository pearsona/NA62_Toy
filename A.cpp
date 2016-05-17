#include "shared.hpp"
#include "QueueReceiver.h"
#include "utils/AExecutable.h"

using namespace na62;

int main(int argc, char* argv[]){
	srand(time(NULL));
	
	int c;
	opterr = 0;
	const char* options = "lh";
	bool load = false;//, check = false;//, pull = false;
	
	const string helpMessage = "To run the full program:\n 1) to/from Data Hold: ./A with options \n\t-l for (l)oading data into the data hold\n\n 2) to/from the checking process: ./B \n\n 3) to finish (i.e. clean up): ./clean \n\n\n";
	
	while( (c = getopt(argc, argv, options)) != -1){
		switch(c){
			//Loading data into memory
		case 'l':
			load = true;
			break;
		case 'h':
		default:
			std::cout<<helpMessage;
			return 1;
		}
	}

	//create shared memory
		try{
		managed_shared_memory segment(open_or_create, "segment", DATA_HOLD_SIZE*32*sizeof(SerializedEvent));
		//Load Data Into Memory Initially
		if(load){
			//alloc_inst.allocate(DATA_HOLD_SIZE*sizeof(int));
			for(uint event_id = 1; event_id <= DATA_HOLD_SIZE; event_id++){
			//Creating an Event with random content
			Event temp_event = newEvent();
			//Serializing the event
			SerializedEvent temp_serialized_event = serializeEvent(temp_event);
			
			//Store the serialized event in the shared memory
			segment.construct<SerializedEvent>(label(event_id))(temp_serialized_event);
			}
			LOG_INFO("Loaded: "<< DATA_HOLD_SIZE <<" Serialized event into memory");
		}
	} catch(interprocess_exception& e) {
		LOG_INFO(e.what());
	}

	//Starting receiver
	QueueReceiver* receiver = new QueueReceiver();
	receiver->startThread("QueueReceiver");

	//Starting sender
	try{
		message_queue *toCheckQ = new message_queue(open_or_create, "toCheck", TO_Q_SIZE, sizeof(int));
		
		uint event_id_to_process = 1;
		while (1) {
			//Sending data to be checked
			while( !toCheckQ->try_send(&event_id_to_process, sizeof(int), priority) ){
				sleep(1);
			}
			LOG_INFO("Sended event id: "<<event_id_to_process);
			event_id_to_process++;
			//Loop back to beginning of shared memory at end
			if (event_id_to_process > DATA_HOLD_SIZE ){
				event_id_to_process = 1;
			}
		}
	} catch(interprocess_exception& e) {
		LOG_ERROR(e.what());
	}

	/*
	 * Join other threads
	 */
	AExecutable::JoinAll();
	return 0;
}
