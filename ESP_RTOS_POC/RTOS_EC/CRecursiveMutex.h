/**
 * @file CRecursiveMutex.h
 * @class CRecursiveMutex
 * @ingroup FreeRTOS_Wrapper
 *
 * The CRecursiveMutex class wraps a native FreeRTOS recursive mutex handle (xSemaphoreHandle).
 * To create a recursive mutex instance an object of this class and then call its CRecursiveMutex::Create method
 * like showed in the following sample:
 * \code
 * CRecursiveMutex g_aRecursiveMutex // global mutex declaration.
 *
 * void taskControlFunc(void *pParams)
 * {
 * 	g_aRecursiveMutex.Create();
 *
 * 	for (;;) {
 * 		while (g_aRecursiveMutex.Take(portMAX_DELAY) != pdTRUE) {
 * 		// do something
 * 		}
 * }
 * \endcode
 *
 * \sa <a href="http://www.freertos.org/Inter-Task-Communication.html#Recursive">recursive mutex documentation</a> in the FreeRTOS web site.
 *
 * @date Jul 14, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */


#ifndef CRECURSIVEMUTEX_H_
#define CRECURSIVEMUTEX_H_

#include <assert.h>
#include "ASyncObject.h"

class CRecursiveMutex: public ASyncObject {
public:
	/**
	 * Default constructor. It initializes the object without attaching it to a native FreeRTOS handle.
	 * Call the Create method to allocate a mutex handle and attach it to the object.
	 */
	CRecursiveMutex();

	/**
	 * Create a recursive mutex instance and attach it to a valid handle.
	 * @param handleSemaphore a valid mutex handle.
	 */
	CRecursiveMutex(xSemaphoreHandle handleSemaphore);

	/**
	 * Delete the native FreeRTOS recursive mutex.
	 */
	virtual ~CRecursiveMutex();

	/**
	 *
	 * @return true if the mutex was created with success, false otherwise.
	 * \sa <a href="http://www.freertos.org/xSemaphoreCreateRecursiveMutex.html">xSemaphoreCreateRecursiveMutex</a> FreeRTOS API function.
	 */
	bool Create();

	/**
	 * \sa <a href="http://www.freertos.org/xSemaphoreTakeRecursive.html">xSemaphoreTakeRecursive</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE Take(portTickType xBlockTime);

	/**
	 * \sa <a href="http://www.freertos.org/xSemaphoreGiveRecursive.html">xSemaphoreGiveRecursive</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE Give();
};


// inline methods implementation

inline
portBASE_TYPE CRecursiveMutex::Take(portTickType xBlockTime) {
#if (configUSE_RECURSIVE_MUTEXES == 1)
	assert(IsValid());

	return xSemaphoreTakeRecursive(m_handleSemaphore, xBlockTime);
#else
	return  pdFALSE;
#endif
}

inline
portBASE_TYPE CRecursiveMutex::Give() {
#if (configUSE_RECURSIVE_MUTEXES == 1)
	assert(IsValid());

	return xSemaphoreGiveRecursive(m_handleSemaphore);
#else
	return  pdFALSE;
#endif
}


#endif /* CRECURSIVEMUTEX_H_ */
