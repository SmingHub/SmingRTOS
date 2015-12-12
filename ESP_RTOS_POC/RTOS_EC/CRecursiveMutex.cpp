/**
 * CRecursiveMutex.cpp
 *
 *  Created on: Jul 14, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CRecursiveMutex.h"

CRecursiveMutex::CRecursiveMutex() {
	// TODO Auto-generated constructor stub

}

CRecursiveMutex::CRecursiveMutex(xSemaphoreHandle handleSemaphore) {
	Attach(handleSemaphore);
}

CRecursiveMutex::~CRecursiveMutex() {
	// TODO Auto-generated destructor stub
}

bool CRecursiveMutex::Create() {
#if (configUSE_RECURSIVE_MUTEXES == 1 && configUSE_MUTEXES ==1)
	xSemaphoreHandle handle = xSemaphoreCreateRecursiveMutex();
	if (handle != NULL)
		Attach(handle);
#endif

	return IsValid();
}

