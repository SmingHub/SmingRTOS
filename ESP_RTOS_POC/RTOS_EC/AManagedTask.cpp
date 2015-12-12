/**
 * AManagedTask.cpp
 *
 *  Created on: Aug 3, 2010
 *      Author: Stefano Oliveri
 */

#include "AManagedTask.h"
#include "CFreeRTOS.h"

AManagedTask* AManagedTask::s_pManagedTaskListHead = NULL;

AManagedTask::AManagedTask() {
	m_pNextManagedTask = NULL;
	AddToManagedTask(this);
}

AManagedTask::~AManagedTask() {
	RemoveFromManagedTask(this);
}

portBASE_TYPE AManagedTask::Create(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority) {
	return OnCreate(pcName, usStackDepth, uxPriority) &&
			CTask::Create(AManagedTask::taskControlFunc, pcName, usStackDepth, this, uxPriority);
}

void AManagedTask::taskControlFunc(void *pParams) {
	static_cast<AManagedTask *>(pParams)->Run();
}

void AManagedTask::AddToManagedTask(AManagedTask *pTaskToAdd) {
	EnterCritical();
	// is the first element?
	if (s_pManagedTaskListHead == NULL) {
		s_pManagedTaskListHead = this;
	}
	else {
		// add the task to the head of the managed task list.
		m_pNextManagedTask = s_pManagedTaskListHead;
		s_pManagedTaskListHead = this;
	}
	ExitCritical();
}

void AManagedTask::RemoveFromManagedTask(AManagedTask *pTaskToRemove) {
	// search for the task to remove in the managed task list
	// is it the first element?
	EnterCritical();
	if (pTaskToRemove == s_pManagedTaskListHead) {
		s_pManagedTaskListHead = pTaskToRemove->m_pNextManagedTask;
		ExitCritical();
	}
	else {
		ExitCritical();
		CFreeRTOS::SuspendAllTasks();
		AManagedTask *pTask = s_pManagedTaskListHead;
		while (pTask != NULL && pTask->m_pNextManagedTask != pTaskToRemove) {
			pTask = pTask->m_pNextManagedTask;
		}
		if (pTask != NULL) { // Check to be sure that the task is in the list
			// remove the thask from the list
			pTask->m_pNextManagedTask = pTaskToRemove->m_pNextManagedTask;
		}
		CFreeRTOS::ResumeAllTasks();
	}
}

bool AManagedTask::InitHardwareForManagedTasks() {
	bool bRes = true;
	for (AManagedTask *pTask=s_pManagedTaskListHead; pTask!=NULL; pTask=pTask->m_pNextManagedTask)
		bRes &= pTask->HardwareInit();

	return bRes;
}
