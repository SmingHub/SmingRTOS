/*
 * QueuedInterrupt.cpp
 *
 *  Created on: 27 mrt. 2016
 *      Author: Herman
 */

#include <sming/core/QueuedInterrupt.h>

QueuedInterrupt::QueuedInterrupt(int reqPin, GPIO_INT_TYPE reqInterruptType,QueuedInterruptDelegate reqDelegate)
: QueuedDelegate(reqDelegate), interruptPin(reqPin), interruptType(reqInterruptType)
{
	attachInterrupt(interruptPin,Delegate<void()>(&QueuedInterrupt::delegatedInterrupt,this), (GPIO_INT_TYPE)GPIO_PIN_INTR_ANYEDGE);
}

QueuedInterrupt::~QueuedInterrupt() {
	detachInterrupt(interruptPin);
}

void IRAM_ATTR QueuedInterrupt::delegatedInterrupt()
{
	int interruptMicros = micros();
	int interruptPinValue = digitalRead(interruptPin);
	QueuedInterruptMessage queuedInterruptMessage;

	queuedInterruptMessage.interruptPin = interruptPin;
	queuedInterruptMessage.interruptMicros = interruptMicros;
	queuedInterruptMessage.interruptPinValue = interruptPinValue;
	sendQueue(queuedInterruptMessage, true);
}

