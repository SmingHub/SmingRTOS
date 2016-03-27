/*
 * QueuedInterrupt.h
 *
 *  Created on: 27 mrt. 2016
 *      Author: Herman
 */

#ifndef SMING_CORE_QUEUEDINTERRUPT_H_
#define SMING_CORE_QUEUEDINTERRUPT_H_

#include "../include/SmingIncludes.h"
#include "QueuedDelegate.h"

typedef struct
{
   int interruptPin;
   uint32 interruptMicros;
   int interruptPinValue;
} QueuedInterruptMessage;

typedef Delegate<void(QueuedInterruptMessage)> QueuedInterruptDelegate;

class QueuedInterrupt : public QueuedDelegate<QueuedInterruptMessage>{
public:
	QueuedInterrupt(int reqPin, GPIO_INT_TYPE reqInterruptType,QueuedInterruptDelegate reqDelegate);
	virtual ~QueuedInterrupt();
private:
	int interruptPin;
	GPIO_INT_TYPE interruptType;
	void IRAM_ATTR delegatedInterrupt();
};

#endif /* SMING_CORE_QUEUEDINTERRUPT_H_ */
