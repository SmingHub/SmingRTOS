/**
 * @file CMutex.h
 * @class CMutex
 * @ingroup FreeRTOS_Wrapper
 *
 * The CMutex class wraps a native FreeRTOS mutex handle (xSemaphoreHandle).
 * To create a mutex instance an object of this class and then call its CMutex::Create method
 * like showed in the following sample:
 * \code
 * CMutex g_aMutex // global mutex declaration.
 *
 * void taskControlFunc(void *pParams) // a task control loop
 * {
 * 	g_aMutex.Create();
 *
 * 	for (;;) {
 * 		while (g_aMutex.Take(portMAX_DELAY) != pdTRUE) {
 * 		// do something
 * 		}
 * }
 * \endcode
 *
 * \sa <a href="http://www.freertos.org/Inter-Task-Communication.html#Mutexes">mutex documentation</a> in the FreeRTOS web site.
 *
 * @date Jul 14, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */


#ifndef CMUTEX_H_
#define CMUTEX_H_

#include <assert.h>
#include "ASyncObject.h"

class CMutex: public ASyncObject {
public:
	/**
	 * Default constructor. It initializes the object without attaching it to a native FreeRTOS handle.
	 * Call the Create method to allocate a mutex handle and attach it to the object.
	 */
	CMutex();

	/**
	 * Create a mutex instance and attach it to a valid handle.
	 *
	 * @param handleSemaphore a valid mutex handle.
	 */
	CMutex(xSemaphoreHandle handleSemaphore);

	/**
	 * Destructor. It deletes the native FreeRTOS recursive mutex.
	 */
	virtual ~CMutex();

	/**
	 * Create a FreeRTOS mutex and attach its handle to the receiver object.
	 *
	 * \sa <a href="http://www.freertos.org/CreateMutex.html">xSemaphoreCreateMutex</a> FreeRTOS API function.
	 *
	 * @return true if the mutex is created with success, false otherwise.
	 */
	bool Create();

	/**
	 *  \sa <a href="http://www.freertos.org/a00122.html">xSemaphoreTake</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE Take(portTickType xBlockTime);

	/**
	 * \sa <a href="http://www.freertos.org/a00123.html">xSemaphoreGive</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE Give();
};


// inline methods implementation

inline
portBASE_TYPE CMutex::Take(portTickType xBlockTime) {
	assert(IsValid());

	return xSemaphoreTake(m_handleSemaphore, xBlockTime);
}

inline
portBASE_TYPE CMutex::Give() {
	assert(IsValid());

	return xSemaphoreGive(m_handleSemaphore);
}

#endif /* CMUTEX_H_ */
