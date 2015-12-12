/**
 * CFreeRTOS.cpp
 *
 *  Created on: Jul 14, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CFreeRTOS.h"
#include "AManagedTask.h"

CFreeRTOS::CFreeRTOS() {
	// TODO Auto-generated constructor stub

}

CFreeRTOS::~CFreeRTOS() {
	// TODO Auto-generated destructor stub
}

bool CFreeRTOS::InitHardwareForManagedTasks() {
	return AManagedTask::InitHardwareForManagedTasks();
}
