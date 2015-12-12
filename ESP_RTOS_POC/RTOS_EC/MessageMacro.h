/**
 * @file MessageMacro.h
 *
 * This file exports the macro used by CMessageTask derived classes.
 *
 * @date   Feb 28, 2011
 * @author Stefano Oliveri (STF12.net)
 *         E-mail: software@stf12.net
 */

#ifndef MESSAGEMACRO_H_
#define MESSAGEMACRO_H_


/**
 * @class CMessage
 * @ingroup FreeRTOS_Managed
 *
 * \sa CMessageTask
 *
 */
class CMessage {
public:
	/**
	 * Specifies the message id. It should me unique in teh application domain.
	 */
	unsigned short m_nId;

	/**
	 * Specifies additional parameters passed to the message handler by the task message loop routine.
	 */
	unsigned long wParam;
};

/**
 * \def DECLARE_MESSAGE_MAP(className)
 *
 * Use this macro at the beginning of a CMessageTask derived class declaration to
 * add the class message map declaration.
 *
 * <b>Example</b>
 * \code
 * class MyClass: public CMessagTask
 * {
 * 		DECLARE_MESSAGE_MAP(MyClass);
 * 		// class declaration...
 * };
 * \endcode
 */
#define DECLARE_MESSAGE_MAP(className) \
	typedef struct {\
		unsigned short id;\
		void (className::*MessageHandler)(unsigned long wParam);\
	} message_map_entry_t;\
	public:virtual void DispatchMessage(const CMessage &msg);\
	private:static const message_map_entry_t s_message_map[];

/**
 * \def BEGIN_MESSAGE_MAP(baseClass, className)
 *
 * Use this macro in the definition file of a CMessageTask derived class to
 * start the class message amp definition.
 *
 * <b>Example</b>
 * \code
 * // CMyClass.cpp file
 *
 * BEGIN_MESSAGE_MAP(CMyClass, CMessageTask)
 * 	MESSAGE_MAP_ENTRY(CMyClass, MSG_ID1, Message1Handler)
 * END_MESSAGE_MAP(CMyClass)
 * \endcode
 */
#define BEGIN_MESSAGE_MAP(baseClass, className) \
	void className::DispatchMessage(const CMessage &msg) {\
		for (int i=0; s_message_map[i].id!=NULL_MSG; ++i) {\
			if (s_message_map[i].id == msg.m_nId) {\
				(this->*s_message_map[i].MessageHandler)(msg.wParam);\
				return;\
			}\
		}\
		return baseClass::DispatchMessage(msg);\
	}\
	const className::message_map_entry_t className::s_message_map[] = {


/**
 * \def MESSAGE_MAP_ENTRY(className, messageId, messageHandler)
 *
 * Use this macro in the definition file of a CMessageTask derived class to
 * add a message map entry in the class message map.
 *
 * <b>Example</b>
 * \sa BEGIN_MESSAGE_MAP(baseClass, className)
 */
#define MESSAGE_MAP_ENTRY(className, messageId, messageHandler) \
	{ messageId, &className::messageHandler },

/**
 * \def END_MESSAGE_MAP(className)
 *
 * Use this macro in the definition file of a CMessageTask derived class to
 * end the class message map definition.#
 *
 * <b>Example</b>
 * \sa BEGIN_MESSAGE_MAP(baseClass, className)
 */
#define END_MESSAGE_MAP(className) \
	{ NULL_MSG, NULL } \
	};


#endif /* MESSAGEMACRO_H_ */
