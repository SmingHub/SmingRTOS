/**
 * @file CMessageTask.h
 * @class CMessageTask
 * @ingroup FreeRTOS_Managed
 *
 * This class implements the Queued Message Handler (QMH) design pattern.
 * \image html QMH.png "Queued Message Handler"
 * The purpose of the QMH pattern is to allow asynchronous communication between tasks through
 * a message passing paradigm.
 * A message is defined by:
 * 	- a message identifier (id) that should be unique.
 * 	- a message handler method.
 *
 * The prototype of a message handler is: \code void className::MessageHandler(unsigned long wParam) \endcode
 * A derived class should not implement a task control loop (by overriding the AManagedTask::Run method),
 * that is provided by the CMessageTask class. This procedure implements the message dispatch routine.
 * When a message is received the message loop searches the derived class's message map for an entry with the same
 * id of the received message. If such an entry is found the associated message handler is performed and the
 * message loop start processing the next message. Otherwise the message loop walk through the message map of the
 * base class. If no entry able to handle the message is found, it is discarded.
 *
 * One delegate object, implementing the IMessageTaskDelegate interface, could be associated to a CMessageTask object
 * using the method CMessageTask::SetDelegate. A delegate receive a notification from the task before and after a
 * message is dispatched.
 *
 * The MessageMacro.h file export a set of macro in order to declare and define the message map in a derived class.
 *
 * <b>Use Cases</b>
 * 	- Gate keeper
 * 	- Command Handler Task.
 *
 * <b>Example</b>
 * \code
 * //In the header file
 *
 * #define MSG_1 APP_MSG + 1
 * #define MSG_2 APP_MSG + 2
 *
 * class MyClass: public CMessageTask
 * {
 * 	DECLARE_MESSAGE_MAP(CMessageTestTask)
 *
 * 	public:
 * 	CMessageTestTask();
 * 	virtual ~CMessageTestTask();
 *
 * 	void Message1Handler(unsigned long wParam);
 * 	void Message2Handler(unsigned long wParam);
 * };
 *
 * // In the implementation file
 * BEGIN_MESSAGE_MAP(CMessageTask, MyClass)
 *   MESSAGE_MAP_ENTRY(MyClass, MSG_1, Message1Handler)
 *   MESSAGE_MAP_ENTRY(MyClass, MSG_2, Message2Handler)
 * END_MESSAGE_MAP(CMessageTestTask)
 * \endcode
 *
 * @date   Feb 28, 2011
 * @author Stefano Oliveri (STF12.net)
 *         E-mail: software@stf12.net
 */

#ifndef CMESSAGETASK_H_
#define CMESSAGETASK_H_

#include "AManagedTask.h"
#include "MessageMacro.h"
#include "Message.h"
#include "IMessageTaskDelegate.h"
#include "CQueue.h"


class CMessageTask: public AManagedTask {
public:

	enum {
		MESSAGE_QUEUE_SIZE = 5,
	};

private:

	DECLARE_MESSAGE_MAP(CMessageTask);

	/**
	 * Specify maximum amount of time the task should block waiting for an incoming message.
	 * The default value is portMAX_DELAY and the task blocks indefinitely.
	 */
	portTickType m_nTimeOut;

	/**
	 * Specifies the synchronization queue used to serialize the access at the LCD.
	 */
	CQueue m_queue;

	/**
	 * Specifies the task delegate object. It is the object that receive the notification
	 * message from the task.
	 */
	IMessageTaskDelegate *m_pDelegate;

public:

	CMessageTask();
	virtual ~CMessageTask();


	/**
	 * A subclass overriding this methods should call the base implementation.
	 * \sa AManagedTask::OnCreate
	 *
	 * @param pcName specifies the name assigned to the task for debug purpose.
	 * @param usStackDepth specifies the task stack size in words.
	 * @param uxPriority specifies the task priority.
	 *
	 */
	portBASE_TYPE OnCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority);

	/**
	 * Task control function. It implements the message loop.
	 */
	void Run();

	/**
	 * Called by the framework after the scheduler is started, but before the task
	 * enter its message loop. Use this method to perform one shot task initialization.
	 *
	 * @return <code>true</code> if success, <code>false</code> otherwise.
	 */
	virtual bool OnInit() { return true; }

	/**
	 * Called by the framework during the message loop when the timeout expire and no message are
	 * received.
	 */
	virtual void OnTimeOut() {};

	/**
	 * Set the task delegate object. This object is notified during the message dispatching loop,
	 * before and after the message is dispatched.
	 * @param pDelegate specifies the delegate object. If <code>NULL</code> the current delegate
	 * is removed.
	 */
	inline void SetDelegate(IMessageTaskDelegate *pDelegate);

	/**
	 * Set the timeout the will block waiting for new message. The default value is
	 * <code>portMAX_DELAY</code> that cause the task will wait indefinitely.
	 *
	 * @param nNewTimeOut specifies a new timeout value.
	 */
	inline void SetTimeout(portTickType nNewTimeOut);

	/**
	 * Try to post a message to the task's message queue. This method block the calling task
	 * for nTicksToWait ticks if the task's message queue is full. If nTickToWait is equal to
	 * <code>portMAX_DELAY</code> (the default value) and the <code>INCLUDE_vTaskSuspend</code> is set to 1
	 * in the FreeRTOS configuration file, the calling task block indefinitely.
	 *
	 * @param pMsg specifies the message to post.
	 * @param nTicksToWait specifies the maximum amount of time the calling task should block.
	 * @return <code>true</code> if the message was posted, <code>false</code> otherwise.
	 */
	bool PostMessage(CMessage *pMsg, portTickType nTicksToWait=portMAX_DELAY);

	/**
	 * Try post a message to the task's message queue.
	 * It is safe to use this function from within an interrupt service routine.
	 *
	 * \sa PostMessage
	 *
	 * @param pMsg specifies the message to post.
	 * @param pxHigherPriorityTaskWoken this output parameter is set to <code>pdTRUE </code>
	 * if posting the message caused a task to unblock, and the unblocked task has a priority higher
	 * than the currently running task. In this case a context switch should be requested before
	 * the interrupt is exited.
	 * @return <code>true</code> if the message was successfully posted, <code>false</code> otherwise.
	 */
	bool PostMessageFromISR(CMessage *pMsg, portBASE_TYPE * pxHigherPriorityTaskWoken);

	/**
	 * Try to send an high priority to the task's message queue. A message sent using this method
	 * bypass all others pending message, so that it is the next processed message. This method block the
	 * calling task for nTicksToWait ticks if the task's message queue is full. If nTickToWait is equal to
	 * <code>portMAX_DELAY</code> (the default value) and the <code>INCLUDE_vTaskSuspend</code> is set to 1
	 * in the FreeRTOS configuration file, the calling task block indefinitely.
	 *
	 * @param pMsg specifies the message to send.
	 * @param nTicksToWait specifies the maximum amount of time the calling task should block.
	 * @return <code>true</code> if the message was sent, <code>false</code> otherwise.
	 */
	bool SendMessage(CMessage *pMsg, portTickType nTicksToWait=portMAX_DELAY);

	/**
	 * Try to send an high priority to the task's message queue.
	 * It is safe to use this function from within an interrupt service routine.
	 *
	 * \sa SendMessage
	 *
	 * @param pMsg specifies the message to send.
	 * @param pxHigherPriorityTaskWoken specifies the maximum amount of time the calling task should block.
	 * @return <code>true</code> if the message was sent, <code>false</code> otherwise.
	 */
	bool SenfFromISR(CMessage *pMsg, portBASE_TYPE * pxHigherPriorityTaskWoken);
};

// Inline methods implementation

inline
void CMessageTask::SetDelegate(IMessageTaskDelegate *pDelegate) {
	m_pDelegate = pDelegate;
}

inline
void CMessageTask::SetTimeout(portTickType nNewTimeout) {
	m_nTimeOut = nNewTimeout;
}

#endif /* CMESSAGETASK_H_ */
