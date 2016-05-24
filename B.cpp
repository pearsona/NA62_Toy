#include <time.h>
#include <cstdlib>

#include "SharedMemoryManager.h"

#include "structs/TriggerMessager.h"

/*
 * L1 trigger function
 */
static bool computeL1Trigger(Event event) {
   if (event % 2 == 0) {
       return 1; //even
   }
   return 0; //odd
}

/*
 * L2 trigger function
 */
static bool computeL2Trigger(Event event) {
  if( event % 3 == 0 ) {
    return 1; //divisible by 3
  }
  return 0; //divisible by 3
}

//Main
int main(int argc, char *argv[]){

  srand(time(NULL));
  na62::SharedMemoryManager::initialize();

  //Benchmarking Variables
  //=======================
	uint l1_num = 0, l2_num = 0;

    //Dequeue data, decide whether to L1/L2 trigger on it, and enqueue result
    //========================================================================
	TriggerMessager trigger_message;

    Event fetched_event;

    while (1) {

    	if (na62::SharedMemoryManager::getNextEvent(fetched_event, trigger_message)) {
			if(trigger_message.level == 1) {
				trigger_message.trigger_result  = computeL1Trigger(fetched_event);

				//LOG_INFO("Received event: " << trigger_message.id <<" Value: "<< l1_temp_event<<" L"<< ev->level<<" processing algorithm result: "<< result);
				LOG_INFO("Received event: " << trigger_message.id);
				l1_num++;

			}

			if(trigger_message.level == 2) {
				trigger_message.trigger_result = computeL2Trigger(fetched_event);

				l2_num++;
			}
			na62::SharedMemoryManager::pushTriggerResponseQueue(trigger_message);

			//Slowdown the code just for understand what happen
			//usleep(1000000);


			if( l1_num % 10 == 0 ) LOG_INFO(getpid()<<" / l1 / "<<l1_num);
			if( l2_num % 10 == 0 ) LOG_INFO(getpid()<<" / l2 / "<<l2_num);


    	} else {
    		//maybe sleep for a while
    		usleep(1000000);
    		continue;
    	}

    }





  return 0;
}
