/**
 * ATimer.cpp
 *
 * @date Jun 16, 2011
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */

#include "ATimer.h"
#include <assert.h>

#if ( configUSE_TIMERS == 1 )

ATimer::ATimer() {
	m_handleTimer = NULL;

}

ATimer::ATimer(xTimerHandle handleTimer) {
	Attach(handleTimer);
}

ATimer::~ATimer() {
	if ( IsValid() )
		Delete(portMAX_DELAY);
}

void ATimer::Callback(xTimerHandle xTimer) {

	static_cast<ATimer *>(pvTimerGetTimerID(xTimer))->OnExpired();
}

ATimer &ATimer::Create(const signed char *pcTimerName, portTickType xTimerPeriod, unsigned portBASE_TYPE uxAutoReload){//, void * pvTimerID, tmrTIMER_CALLBACK pxCallbackFunction) {
	assert(!IsValid());

	xTimerHandle handle;
	handle = xTimerCreate(pcTimerName, xTimerPeriod, uxAutoReload, this, ATimer::Callback);
	if (handle != NULL)
		Attach(handle);

	return *this;
}

void ATimer::Attach(xGenericHandle handle) {
	assert(handle != NULL);

	if (IsValid()) {
		Delete(portMAX_DELAY);
	}
	m_handleTimer = handle;
}

portBASE_TYPE ATimer::Delete(portTickType xBlockTime) {
	portBASE_TYPE res = pdTRUE;
	if (m_handleTimer != NULL) {
		res = xTimerDelete(m_handleTimer, xBlockTime);
		if (res != pdTRUE)
			m_handleTimer = NULL;
	}

	return res;
}

#endif // configUSE_TIMERS
