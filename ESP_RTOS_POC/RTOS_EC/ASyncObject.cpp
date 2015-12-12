/**
 * ASyncObject.cpp
 *
 *  Created on: Jul 13, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "ASyncObject.h"
#include <assert.h>

ASyncObject::ASyncObject() {
	m_handleSemaphore = NULL;
}

ASyncObject::~ASyncObject() {
	if (IsValid())
		Delete();
}

void ASyncObject::Delete() {
	if (m_handleSemaphore != NULL) {
		vQueueDelete(m_handleSemaphore);
		m_handleSemaphore = NULL;
	}
}

void ASyncObject::Attach(xGenericHandle handle) {
	assert(handle != NULL);

	if (IsValid()) {
		Delete();
	}
	m_handleSemaphore = handle;
}
