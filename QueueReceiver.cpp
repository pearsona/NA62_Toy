/*
 * QueueReceiver.cpp
 *
 *  Created on: May 16, 2016
 *      Author: marco
 */

//#include "QueueReceiver.h"

#include "shared.hpp"

namespace na62 {

  boost::interprocess::message_queue *QueueReceiver::fromCheckQ_;
  boost::interprocess::message_queue *QueueReceiver::toCheckQ_;
  boost::interprocess::managed_shared_memory *QueueReceiver::l1_shm;
  boost::interprocess::managed_shared_memory *QueueReceiver::l2_shm;

  QueueReceiver::QueueReceiver(){
    fromCheckQ_ = new boost::interprocess::message_queue(boost::interprocess::open_or_create, "fromCheck", 15, sizeof(TriggerResponse));
    toCheckQ_ = new boost::interprocess::message_queue(boost::interprocess::open_or_create, "toCheck", 15, sizeof(EventID));
    l1_shm = new boost::interprocess::managed_shared_memory(boost::interprocess::open_or_create, "l1_shm", L1_MEM_SIZE*32*sizeof(SerializedEvent));
    l2_shm = new boost::interprocess::managed_shared_memory(boost::interprocess::open_or_create, "l2_shm", L2_MEM_SIZE*32*sizeof(SerializedEvent));
    running_ = true;
  }

  QueueReceiver::~QueueReceiver(){}

  void QueueReceiver::thread() {
    //All Those variable can became part of the class
    TriggerResponse *response = (TriggerResponse *)malloc(sizeof(TriggerResponse *));
    //std::size_t recvd_size;
    //uint priority = 0;

    while (running_) {

      //Receiving response from trigger processor
      try{
	if (fromCheckQ_->try_receive((void *)response, sizeof(TriggerResponse), recvd_size, priority)) {
	  if (recvd_size != sizeof(TriggerResponse)) {
	    LOG_ERROR("Error");
	    continue;
	  }

	  LOG_INFO("Received l"<<response->level<<" event id:"<< response->event_id << " Result: "<< response->result);
	  char *ID = label(response->event_id);

	  if (response->result) {
	    if (response->level == 1){
	      //good event send L1 request
	      LOG_INFO(" -> Sending L1 request for event id: "<< response->event_id);
	      
	      d = l1_shm->find<SerializedEvent>(ID);
	      l1_shm->destroy<SerializedEvent>(ID);
	      l2_shm->construct<SerializedEvent>(ID)(*d.first);

	      //Sending data to be checked for l2 processing
	      EventID *ev = new EventID;
	      ev->id = response->event_id;
	      ev->level = 2;
	      while( !toCheckQ_->try_send(ev, sizeof(EventID), priority) ){
		sleep(1);
	      
	      LOG_INFO("Sended event id: "<<ev->id<<" for l"<<ev->level<<" processing");
	      }
	    }
	    else if (response->level == 2){
	      LOG_INFO(" -> Sending L2 request for event id: "<< response->event_id);  
	    }

	  } else {
	    //Bad event
	    LOG_INFO(" -> Discard event id: "<< response->event_id << "from L"<< response->level << " triggering");

	    if( !l1_shm->destroy<SerializedEvent>(ID) )
	      l2_shm->destroy<SerializedEvent>(ID);
	  }
	} else {
	  boost::this_thread::sleep(boost::posix_time::microsec(50));
	}
      } catch(boost::interprocess::interprocess_exception& e) {
	LOG_ERROR(e.what());
      }
    }
  }

  void QueueReceiver::onInterruption() {
    running_ = false;
  }

}
