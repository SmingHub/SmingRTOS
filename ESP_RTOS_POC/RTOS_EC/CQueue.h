/**
 * @file CQueue.h
 * @class CQueue
 * @ingroup FreeRTOS_Wrapper
 *
 * The CQueue class wraps a native FreeRTOS queue handle (xQueueHandle).
 * To create a queue instance an object of this class and then call its CQueue::Create method
 * like showed in the following sample:
 * \code
 * CQueue g_aQueue // global task declaration.
 *
 * void taskControlFunc(void *pParams)
 * {
 * 	unsigned char *Message;
 * 	g_aQueue.Create(nQueueSize, sizeof(queueItemT));
 *
 * 	for (;;) {
 * 		g_aQueue.Receive(&Message, portMAX_DELAY ) != pdTRUE)
 * 		// do something
 * }
 * \endcode
 *
 * See the CLcdTask for an example.
 *
 * \sa <a href="http://www.freertos.org/Inter-Task-Communication.html">queue documentation</a> in the FreeRTOS web site.
 *
 * @date Jul 12, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */


#ifndef CQUEUE_H_
#define CQUEUE_H_

#include <assert.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "IFreeRTOSProtocol.h"


class CQueue: public IFreeRTOSObj {
	/**
	 * Specifies the native FreeRTOS handle managed by an instance of this class.
	 */
	xQueueHandle m_handleQueue;

public:
	/**
	 * The default constructor. It initializes the object without attaching it to a native FreeRTOS handle.
	 * Call the Create method to allocate a queue handle and attach it to the object.
	 */
	CQueue();

	/**
	 * Create a CQueue instance and attach it to a valid handle.
	 *
	 * @param handleQueue a valid queue handle.
	 */
	CQueue(xQueueHandle handleQueue);

	/**
	 * Delete the native FreeRTOS queue.
	 */
	virtual ~CQueue();

	/**
	 * Retrieve the native FreeRTOS queue handle attached to this object.
	 *
	 * @return the native FreeRTOS queue handle attached to this object.
	 */
	inline operator xQueueHandle() const { return m_handleQueue; }

	inline bool IsValid() const;
	void Attach(xGenericHandle handle);
	inline xGenericHandle Detach();

	// FreeRTOS API Wrappers

	/**
	 * \sa <a href="http://www.freertos.org/a00116.html">xQueueCreate</a> FreeRTOS API function.
	 */
	CQueue &Create(unsigned portBASE_TYPE uxQueueLength, unsigned portBASE_TYPE uxItemSize);

	/**
	 * \sa <a href="http://www.freertos.org/a00018.html#vQueueDelete">vQueueDelete</a> FreeRTOS API function.
	 */
	void Delete();

	/**
	 * \sa <a href="http://www.freertos.org/a00018.html#ucQueueMessagesWaiting">uxQueueMessagesWaiting</a> FreeRTOS API function.
	 */
	inline unsigned portBASE_TYPE MessagesWaiting();

	/**
	 * \sa <a href="http://www.freertos.org/a00117.html">xQueueSend</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE Send(const void * pvItemToQueue, portTickType xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/xQueueSendToBack.html">SendToBack</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE SendToBack(const void * pvItemToQueue, portTickType xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/xQueueSendToFront.html">xQueueSendToFront</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE SendToFront(const void * pvItemToQueue, portTickType xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/a00118.html">xQueueReceive</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE Receive(void *pvBuffer, portTickType xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/xQueuePeek.html">xQueuePeek</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE Peek(void *pvBuffer, portTickType xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/a00119.html">xQueueSendFromISR</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE SendFromISR(const void * pvItemToQueue, portBASE_TYPE *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/xQueueSendToBackFromISR.html">xQueueSendToBackFromISR</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE SendToBackFromISR(const void * pvItemToQueue, portBASE_TYPE *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/xQueueSendToFrontFromISR.html">xQueueSendToFrontFromISR</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE SendToFrontFromISR(const void * pvItemToQueue, portBASE_TYPE *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/a00120.html">xQueueReceiveFromISR</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE ReceiveFromISR(void *pvBuffer, portBASE_TYPE *pxTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/vQueueAddToRegistry.html">vQueueAddToRegistry</a> FreeRTOS API function.
	 */
	inline void AddToRegistry(signed portCHAR *pcQueueName);

};

// inline method implementation

inline
portBASE_TYPE CQueue::Send(const void * pvItemToQueue, portTickType xTicksToWait) {
	return xQueueSend(m_handleQueue, pvItemToQueue, xTicksToWait);
}

inline
portBASE_TYPE CQueue::SendFromISR(const void * pvItemToQueue, portBASE_TYPE *pxHigherPriorityTaskWoken) {
	return xQueueSendFromISR(m_handleQueue, pvItemToQueue, pxHigherPriorityTaskWoken);
}

inline
unsigned portBASE_TYPE CQueue::MessagesWaiting() {
	assert(IsValid());

	return uxQueueMessagesWaiting(m_handleQueue);
}

inline
portBASE_TYPE CQueue::SendToBack(const void * pvItemToQueue, portTickType xTicksToWait) {
	assert(IsValid());

	return xQueueSendToBack(m_handleQueue, pvItemToQueue, xTicksToWait);
}

inline
portBASE_TYPE CQueue::SendToFront(const void * pvItemToQueue, portTickType xTicksToWait) {
	assert(IsValid());

	return xQueueSendToFront(m_handleQueue, pvItemToQueue, xTicksToWait);
}

inline
portBASE_TYPE CQueue::Receive(void *pvBuffer, portTickType xTicksToWait) {
	assert(IsValid());

	return xQueueReceive(m_handleQueue, pvBuffer, xTicksToWait);
}

inline
portBASE_TYPE CQueue::Peek(void *pvBuffer, portTickType xTicksToWait) {
	assert(IsValid());

	return xQueuePeek(m_handleQueue, pvBuffer, xTicksToWait);
}

inline
portBASE_TYPE CQueue::SendToBackFromISR(const void * pvItemToQueue, portBASE_TYPE *pxHigherPriorityTaskWoken) {
	assert(IsValid());

	return xQueueSendToBackFromISR(m_handleQueue, pvItemToQueue, pxHigherPriorityTaskWoken);
}

inline
portBASE_TYPE CQueue::SendToFrontFromISR(const void * pvItemToQueue, portBASE_TYPE *pxHigherPriorityTaskWoken) {
	assert(IsValid());

	return xQueueSendToFrontFromISR(m_handleQueue, pvItemToQueue, pxHigherPriorityTaskWoken);
}

inline
portBASE_TYPE CQueue::ReceiveFromISR(void *pvBuffer, portBASE_TYPE *pxTaskWoken) {
	assert(IsValid());

	return xQueueReceiveFromISR(m_handleQueue, pvBuffer, pxTaskWoken);
}

inline
void CQueue::AddToRegistry(signed portCHAR *pcQueueName) {
#if (configQUEUE_REGISTRY_SIZE > 0)
	assert(IsValid());

	vQueueAddToRegistry(m_handleQueue, pcQueueName);
#endif
}

inline
bool CQueue::IsValid() const{
	return m_handleQueue != NULL;
}

inline
xGenericHandle CQueue::Detach() {
	xQueueHandle res = m_handleQueue;
	m_handleQueue = NULL;
	return res;
}

#endif /* CQUEUE_H_ */

