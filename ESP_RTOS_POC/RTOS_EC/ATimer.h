/**
 * @file ATimer.h
 * @class ATimer
 * @ingroup FreeRTOS_Wrapper
 *
 * The ATimer class wraps a native FreeRTOS timer handle (xTimerHandle). The application must subclass this class and
 * implements the pure virtual method ATimer::OnExpired. The ATimer::OnExpired method is called when the timer period expires.
 * This class conforms to the iFreeRTOSProtocol protocol, so the application has to call the object ATimer::Create
 * method in order to allocate the underling FreeRTOS resource before using it.
 *
 * \sa <a href="http://www.freertos.org/RTOS-software-timer.html">Software Timer documentation</a> in the FreeRTOS web site.
 *
 * @date Jun 16, 2011
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */

#ifndef CTIMER_H_
#define CTIMER_H_

#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "IFreeRTOSProtocol.h"

class ATimer: public IFreeRTOSObj {

	/**
	 * Specifies the native FreeRTOS handle managed by an instance of this class.
	 */
	xTimerHandle m_handleTimer;

	/**
	 * Default timer callback. It gets the timer identifier, that is a pointer to the
	 * ATimer application derived object, and call the OnExpired method on that object.
	 *
	 * @param xTimer handle to the expired timer.
	 */
	static void Callback(xTimerHandle xTimer);

public:

	/**
	 * The default constructor. It initializes the object without attaching it to a native FreeRTOS handle.
	 * Call the Create method to allocate a timer handle and attach it to the object.
	 */
	ATimer();

	/**
	 * Create a ATimer instance and attach it to a valid handle.
	 *
	 * @param handleTimer a valid timer handle.
	 */
	ATimer(xTimerHandle handleTimer);

	/**
	 * Delete the native FreeRTOS timer.
	 */
	virtual ~ATimer();

	/**
	 * Retrieve the native FreeRTOS timer handle attached to this object.
	 *
	 * @return the native FreeRTOS timer handle attached to this object.
	 */
	inline operator xTimerHandle() const { return m_handleTimer; }

	/**
	 * Allocate the FreeRTOS timer object. The timer identifier of the wrapped xTimerCreate function
	 * is used to pass a pointer to this object.
	 *
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerCreate.html">xTimerCreate</a> FreeRTOS API function.
	 */
	ATimer &Create(const signed char *pcTimerName, portTickType xTimerPeriod, unsigned portBASE_TYPE uxAutoReload);

	/**
	 * Application defined callback called when the timer expire.
	 */
	virtual void OnExpired() =0;

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerIsTimerActive.html">xTimerIsTimerActive</a>  FreeRTOS API function.
	 */
	inline portBASE_TYPE IsTimerActive() const;

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerStart.html">xTimerStart</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE Start(portTickType xBlockTime);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerStop.html">xTimerStop</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE Stop(portTickType xBlockTime);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerChangePeriod.html">xTimerChangePeriod</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE ChangePeriod(portTickType xNewPeriod, portTickType xBlockTime);

	/**
	 * TODO: STF - da rivedere.
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerDelete.html">xTimerDelete</a>  FreeRTOS API function.
	 */
	portBASE_TYPE Delete(portTickType xBlockTime);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerReset.html">xTimerReset</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE Reset(portTickType xBlockTime);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerStartFromISR.html">xTimerStartFromISR</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE StartFromISR(portBASE_TYPE *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerStopFromISR.html">xTimerStopFromISR</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE StopFromISR(portBASE_TYPE *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerChangePeriodFromISR.html">xTimerChangePeriodFromISR</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE ChangePeriodFromISR(portTickType xNewPeriod, portBASE_TYPE *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerResetFromISR.html">xTimerResetFromISR</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE ResetFromISR(portBASE_TYPE *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-pvTimerGetTimerID.html">pvTimerGetTimerID</a> FreeRTOS API function.
	 */
	inline void *GetTimerID() const;

	// FreeRTOS class extension.
	bool IsValid() const;
	void Attach(xGenericHandle handle);
	xGenericHandle Detach();
};

// inline method
////////////////

#if ( configUSE_TIMERS == 1 )

inline
portBASE_TYPE ATimer::IsTimerActive() const {
	return xTimerIsTimerActive(m_handleTimer);
}

inline
portBASE_TYPE ATimer::Start(portTickType xBlockTime) {
	return xTimerStart(m_handleTimer, xBlockTime);
}

inline
portBASE_TYPE ATimer::Stop(portTickType xBlockTime) {
	return xTimerStop(m_handleTimer, xBlockTime);
}

inline
portBASE_TYPE ATimer::ChangePeriod(portTickType xNewPeriod, portTickType xBlockTime) {
	return xTimerChangePeriod(m_handleTimer, xNewPeriod, xBlockTime);
}

inline
portBASE_TYPE ATimer::Reset(portTickType xBlockTime) {
	return xTimerReset(m_handleTimer, xBlockTime);
}

inline
portBASE_TYPE ATimer::StartFromISR(portBASE_TYPE *pxHigherPriorityTaskWoken) {
	return xTimerStartFromISR(m_handleTimer, pxHigherPriorityTaskWoken);
}

inline
portBASE_TYPE ATimer::ChangePeriodFromISR(portTickType xNewPeriod, portBASE_TYPE *pxHigherPriorityTaskWoken) {
	return xTimerChangePeriodFromISR(m_handleTimer, xNewPeriod, pxHigherPriorityTaskWoken);
}

inline
portBASE_TYPE ATimer::ResetFromISR(portBASE_TYPE *pxHigherPriorityTaskWoken) {
	return xTimerResetFromISR(m_handleTimer, pxHigherPriorityTaskWoken);
}

inline
void *ATimer::GetTimerID() const {
	return pvTimerGetTimerID(m_handleTimer);
}

inline
bool ATimer::IsValid() const{
	return m_handleTimer != NULL;
}

inline
xGenericHandle ATimer::Detach() {
	xTimerHandle res = m_handleTimer;
	m_handleTimer = NULL;
	return res;
}

#endif // configUSE_TIMERS

#endif /* CTIMER_H_ */
