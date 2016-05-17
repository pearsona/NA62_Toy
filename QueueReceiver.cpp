/*
 * QueueReceiver.cpp
 *
 *  Created on: May 16, 2016
 *      Author: marco
 */

#include "QueueReceiver.h"
#include "structs/TriggerResponse.h"
#include <boost/timer/timer.hpp>


namespace na62 {

boost::interprocess::message_queue *QueueReceiver::fromCheckQ_;

QueueReceiver::QueueReceiver(){
	fromCheckQ_ = new boost::interprocess::message_queue(boost::interprocess::open_or_create, "fromCheck", 15, sizeof(TriggerResponse));
	running_ = true;
}

QueueReceiver::~QueueReceiver(){}

void QueueReceiver::thread() {
		//All Those variable can became part of the class
		TriggerResponse *response = (TriggerResponse *)malloc(sizeof(TriggerResponse *));
		std::size_t recvd_size;
		uint priority = 0;

		while (running_) {
			//Receiving response from trigger processor
			try{
				if (fromCheckQ_->try_receive((void *)response, sizeof(response), recvd_size, priority)) {
					if (recvd_size != sizeof(response)) {
						LOG_ERROR("Error");
						continue;
					}

					LOG_INFO("Received event id:"<< response->event_id << " Result: "<< response->l1_result);
					
					if (response->l1_result) {
						//good event send L1 request
						LOG_INFO(" -> Sending L1 request for event id: "<< response->event_id);
					} else {
						//Bad event
						LOG_INFO(" -> Discard event id: "<< response->event_id);
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
