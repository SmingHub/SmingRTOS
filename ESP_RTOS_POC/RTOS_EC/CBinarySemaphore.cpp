/**
 * CBinarySemaphore.cpp
 *
 *  Created on: Jul 13, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CBinarySemaphore.h"
#include <assert.h>

CBinarySemaphore::CBinarySemaphore() {
	// TODO Auto-generated constructor stub

}

CBinarySemaphore::~CBinarySemaphore() {
	// TODO Auto-generated destructor stub
}

CBinarySemaphore::CBinarySemaphore(xSemaphoreHandle handleSemaphore) {
	Attach(handleSemaphore);
}

bool CBinarySemaphore::Create() {
	xSemaphoreHandle handle;

	vSemaphoreCreateBinary(handle);

	if (handle != NULL)
		Attach(handle);

	return IsValid();
}
