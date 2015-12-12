/**
 * @file Message.h
 * @ingroup FreeRTOS_Managed
 *
 * Export the message id used by the framework. The message id in the range [0, APP_MSG-1] are
 * reserved. An user defined message should start from the APP_MSG id.
 *
 * \sa CMessageTask
 *
 * @date   Feb 28, 2011
 * @author Stefano Oliveri (STF12.net)
 *         E-mail: software@stf12.net
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_


// System Messages

#define	NULL_MSG						0
#define APP_MSG							0x400


#endif /* MESSAGE_H_ */
