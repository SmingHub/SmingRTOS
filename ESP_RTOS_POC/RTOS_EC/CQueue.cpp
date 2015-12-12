/**
 * CQueue.cpp
 *
 *  Created on: Jul 12, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CQueue.h"


CQueue::CQueue() {
	m_handleQueue = NULL;
}

CQueue::~CQueue() {
	if (IsValid())
		Delete();
}

CQueue::CQueue(xQueueHandle handleQueue) {
	Attach(handleQueue);
}

CQueue &CQueue::Create(unsigned portBASE_TYPE uxQueueLength, unsigned portBASE_TYPE uxItemSize) {
	assert(!IsValid());

	xQueueHandle handle;

	handle = xQueueCreate(uxQueueLength, uxItemSize);
	if (handle != NULL)
		Attach(handle);

	return *this;
}

void CQueue::Attach(xGenericHandle handle) {
	assert(handle != NULL);

	if (IsValid())
		Delete();

	m_handleQueue = handle;
}

void CQueue::Delete() {
	if (m_handleQueue != NULL) {
		vQueueDelete(m_handleQueue);
		m_handleQueue = NULL;
	}
}

