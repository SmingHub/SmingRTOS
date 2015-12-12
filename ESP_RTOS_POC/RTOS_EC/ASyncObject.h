/**
 * @file   ASyncObject.h
 * @class  ASyncObject
 * @ingroup FreeRTOS_Wrapper
 *
 * Abstract base class for all task synchronization object.
 *
 * @date   Jul 13, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */

#ifndef ASEMAPHORE_H_
#define ASEMAPHORE_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "IFreeRTOSProtocol.h"

class ASyncObject: public IFreeRTOSObj {
protected:
    xSemaphoreHandle m_handleSemaphore;

public:
	ASyncObject();
	virtual ~ASyncObject();

	inline operator xSemaphoreHandle() const { return m_handleSemaphore; }

	void Delete();
	inline bool IsValid()const;
	void Attach(xGenericHandle handle);
	inline xGenericHandle Detach();

	virtual portBASE_TYPE Take(portTickType xBlockTime) =0;
	virtual portBASE_TYPE Give() =0;
};

// inline method

inline
bool ASyncObject::IsValid() const {
	return m_handleSemaphore != NULL;
}

inline
xGenericHandle ASyncObject::Detach() {
	xSemaphoreHandle res = m_handleSemaphore;
	m_handleSemaphore = NULL;
	return res;
}

#endif /* ASEMAPHORE_H_ */

