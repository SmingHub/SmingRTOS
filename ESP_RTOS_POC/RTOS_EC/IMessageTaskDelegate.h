/**
 * @file IMessageTaskDelegate.h
 * @class IMessageTaskDelegate
 * @ingroup FreeRTOS_Managed
 *
 * This interface is to be implemented by an object in order to receive notification
 * by the associated CMessageTask during the message loop.
 *
 * @date   Feb 28, 2011
 * @author Stefano Oliveri (STF12.net)
 *         E-mail: software@stf12.net
 */

#ifndef IMESSAGETASKDELEGATE_H_
#define IMESSAGETASKDELEGATE_H_


class IMessageTaskDelegate {
public:
	/**
	 * Called by the framework before the message is dispatched.
	 *
	 * @param msg specifies the message to be processed.
	 */
	virtual void OnHandleEvent(const CMessage &msg) =0;

	/**
	 * Called by the framework after the message was dispatched.
	 *
	 * @param msg specifies the message that was processed.
	 */
	virtual void DidHandleEvent(const CMessage &msg) =0;
};

#endif /* IDISPLAYDELEGATE_H_ */
