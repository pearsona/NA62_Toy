/*
 * QueueReceiver.cpp
 *
 *  Created on: May 16, 2016
 *      Author: marco
 */

#include "shared.hpp"

namespace na62 {

  message_queue *QueueReceiver::fromCheckQ_;
  message_queue *QueueReceiver::toCheckQ_;
  managed_shared_memory *QueueReceiver::l1_shm;
  managed_shared_memory *QueueReceiver::l2_shm;


  //Constructor and Destructor
  //===========================

  QueueReceiver::QueueReceiver(){
    fromCheckQ_ = new message_queue(open_or_create, "fromCheck", 15, sizeof(TriggerResponse));
    toCheckQ_ = new message_queue(open_or_create, "toCheck", 15, sizeof(EventID));
    l1_shm = new managed_shared_memory(open_or_create, "l1_shm", L1_MEM_SIZE*32*sizeof(SerializedEvent));
    l2_shm = new managed_shared_memory(open_or_create, "l2_shm", L2_MEM_SIZE*32*sizeof(SerializedEvent));
    running_ = true;
  }

  QueueReceiver::~QueueReceiver(){}



  //Threading
  //==========

  void QueueReceiver::thread() {
    //All Those variable can became part of the class
    TriggerResponse *response = new TriggerResponse;

    while (running_) {

      //Receiving Response from Trigger Processor and Acting on It
      //===========================================================

      try{
	
	//Receiving Response
	//=================
      
	//QUESTION: Should this be a while loop instead of an if statement?
	if (fromCheckQ_->try_receive((void *)response, sizeof(TriggerResponse), recvd_size, priority)) {
	  if (recvd_size != sizeof(TriggerResponse)) {
	    LOG_ERROR("Error");
	    continue;
	  }

	  LOG_INFO("Received l"<<response->level<<" event id:"<< response->event_id << " Result: "<< response->result);
	  
	  char *ID = label(response->event_id);

	  
	  //Act on Result
	  //==============

	  //Event is Good
	  //==============
	  if (response->result) {

	    //L1 Processing
	    //==============
	    if (response->level == 1){
	      LOG_INFO(" -> Sending L1 request for event id: "<< response->event_id);
	      
	      //Find and Destroy Event from L1 SHM and store in L2 SHM
	      //======================================================
	      l1_d = l1_shm->find<SerializedEvent>(ID);
	      
	      if( l1_d.first ){
		l1_shm->destroy<SerializedEvent>(ID);
		l2_shm->construct<SerializedEvent>(ID)(*l1_d.first);
	      }
	      else continue;


	      //Sending Event to be Checked for L2 processing
	      //============================================
	      EventID *ev = new EventID;
	      ev->id = response->event_id;
	      ev->level = 2;

	      while( !toCheckQ_->try_send(ev, sizeof(EventID), priority) ){
		sleep(1);
	      
		LOG_INFO("Sended event id: "<<ev->id<<" for l"<<ev->level<<" processing");
	      }
	    }
	    //L2 Processing
	    //==============
	    else if (response->level == 2){
	      LOG_INFO(" -> Sending L2 request for event id: "<< response->event_id);  
	    }

	  } 
	  //Event is Bad
	  //=============
	  else {
	    LOG_INFO(" -> Discard event id: "<< response->event_id << "from L"<< response->level << " triggering");
	    
	    //Delete Event from Memory
	    //=========================
	    if( !l1_shm->destroy<SerializedEvent>(ID) )
	      l2_shm->destroy<SerializedEvent>(ID);

	  }

	} else {
	  boost::this_thread::sleep(boost::posix_time::microsec(50));
	}
      } catch(interprocess_exception& e) {
	LOG_ERROR(e.what());
      }
    }
  }

  void QueueReceiver::onInterruption() {
    running_ = false;
  }

}
