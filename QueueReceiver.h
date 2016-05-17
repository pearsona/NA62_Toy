/*
 * TaskProcessor.cpp
 *
 *  Created on: May 16, 2016
 *      Author: marco
 */

#ifndef QUEUERECEIVER_H_
#define QUEUERECEIVER_H_

#include <utils/AExecutable.h>
#include <atomic>
#include <boost/interprocess/ipc/message_queue.hpp>

namespace na62 {

class QueueReceiver: public AExecutable {
public:
	QueueReceiver();
	virtual ~QueueReceiver();
	static boost::interprocess::message_queue *fromCheckQ_;
    //message_queue *fromCheckQ = new message_queue(open_or_create, "fromCheck", FROM_Q_SIZE, sizeof(l1TriggerResponse));

private:
	virtual void thread() override;
	virtual void onInterruption() override;
	std::atomic<bool> running_;
};

}

#endif
