/**
 * CTask.cpp
 *
 *  Created on: Jul 5, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CTask.h"
#include <assert.h>


CTask::CTask() {
	m_handleTask = NULL;
}

CTask::CTask(xTaskHandle handleTask) {
	Attach(handleTask);
}

CTask::~CTask() {
	if (IsValid())
		Delete();
}

void CTask::Attach(xGenericHandle handle) {
	assert(handle != NULL);

	if (IsValid()) {
		Delete();
	}
	m_handleTask = handle;
}

portBASE_TYPE CTask::Create(pdTASK_CODE pvTaskCode, const portCHAR * const pcName, unsigned portSHORT usStackDepth, void *pvParameters, unsigned portBASE_TYPE uxPriority)
{
	portBASE_TYPE res;
	xTaskHandle handle;
	res = xTaskCreate(pvTaskCode, (const signed char*)pcName, usStackDepth, pvParameters, uxPriority, &handle);
	if (res == pdTRUE)
		Attach(handle);

	return res;
}

portBASE_TYPE CTask::CreateRestricted(xTaskParameters *pxTaskDefinition)
{
	portBASE_TYPE res = pdFALSE;
#if( portUSING_MPU_WRAPPERS == 1 )
	xTaskHandle handle;
	res = xTaskCreateRestricted(pxTaskDefinition, &handle);
	if (res == pdTRUE)
		Attach(handle);
#endif

	return res;
}


void CTask::Delete() {
	if (m_handleTask != NULL) {
#if ( INCLUDE_vTaskDelete == 1 )
		xTaskHandle handleTaskToDelete = m_handleTask;
		m_handleTask = NULL;
		vTaskDelete(handleTaskToDelete);
#else
		vTaskSuspend(m_handleTask);
#endif
		m_handleTask = NULL;
	}
}
