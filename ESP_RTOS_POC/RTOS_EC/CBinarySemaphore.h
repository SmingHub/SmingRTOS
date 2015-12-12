/**
 * @file CBinarySemaphore.h
 * @class CBinarySemaphore
 * @ingroup FreeRTOS_Wrapper
 *
 * The CBinarySemaphore class wraps a native FreeRTOS binary semaphore handle (xSemaphoreHandle).
 * To create a binary semaphore instance an object of this class and then call its CBinarySemaphore::Create method
 * like showed in the following sample:
 * \code
 * CBinarySemaphore g_aSemaphore // global semaphore declaration.
 *
 * void taskControlFunc(void *pParams) // a task control loop
 * {
 * 	g_aSemaphore.Create();
 *
 * 	for (;;) {
 * 		while (g_aSemaphore.Take(portMAX_DELAY) != pdTRUE) {
 * 		// do something
 * 		}
 * }
 * \endcode
 *
 * \sa <a href="http://www.freertos.org/Inter-Task-Communication.html#Binary">binary semaphore documentation</a> in the FreeRTOS web site.

 *
 * @date   Jul 13, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */


#ifndef CBINARYSEMAPHORE_H_
#define CBINARYSEMAPHORE_H_

#include <assert.h>
#include "ASyncObject.h"

class CBinarySemaphore: public ASyncObject {
public:

	/**
	 * Default constructor. It initializes the object without attaching it to a native FreeRTOS handle.
	 * Call the Create method to allocate a binary semaphore handle and attach it to the object.
	 */
	CBinarySemaphore();

	/**
	 * Create a binary semaphore instance and attach it to a valid handle.
	 *
	 * @param handleSemaphore a valid binary semaphore handle.
	 */
	CBinarySemaphore(xSemaphoreHandle handleSemaphore);

	/**
	 * Destructor. It deletes the native FreeRTOS counting semaphore.
	 */
	virtual ~CBinarySemaphore();

	/**
	 * Create a FreeRTOS binary semaphore and attach its handle to the receiver object.
	 *
	 * @return true if the counting semaphore is created with success, false otherwise.
	 *
	 * \sa <a href="http://www.freertos.org/a00121.html">vSemaphoreCreateBinary</a> FreeRTOS API function.
	 */
	bool Create();

	/**
	 * \sa <a href="http://www.freertos.org/a00122.html">xSemaphoreTake</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE Take(portTickType xBlockTime);

	/**
	 * \sa <a href="http://www.freertos.org/a00123.html">xSemaphoreGive</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE Give();

	/**
	 * \sa <a href="http://www.freertos.org/a00124.html">xSemaphoreGiveFromISR</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE GiveFromISR(signed portBASE_TYPE *pxHigherPriorityTaskWoken);
};


// inline methods implementation

inline
portBASE_TYPE CBinarySemaphore::Take(portTickType xBlockTime) {
	assert(IsValid());

	return xSemaphoreTake(m_handleSemaphore, xBlockTime);
}

inline
portBASE_TYPE CBinarySemaphore::Give() {
	assert(IsValid());

	return xSemaphoreGive(m_handleSemaphore);
}

inline
portBASE_TYPE CBinarySemaphore::GiveFromISR(signed portBASE_TYPE *pxHigherPriorityTaskWoken) {
	assert(IsValid());

	return xSemaphoreGiveFromISR(m_handleSemaphore, pxHigherPriorityTaskWoken);
}

#endif /* CBINARYSEMAPHORE_H_ */

