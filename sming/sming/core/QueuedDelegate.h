/*
 * QueuedDelegate.h
 *
 *  Created on: 26 mrt. 2016
 *      Author: Herman
 */

#ifndef SMING_CORE_QUEUEDDELEGATE_H_
#define SMING_CORE_QUEUEDDELEGATE_H_

#define QUEUEDDELEGATE_QUEUE_LEN 10

#include "../include/sming_global.h"
#include "Delegate.h"

template<typename ParamType>
class QueuedDelegate
{
public :

	QueuedDelegate()
	: QueuedDelegate(NULL)
	{
	}

	QueuedDelegate(Delegate<void(ParamType)> reqDelegate)
	: queuedDelegateDelegate (reqDelegate)
	{
		queuedDelegateQueue = xQueueCreate( QUEUEDDELEGATE_QUEUE_LEN, sizeof(ParamType) );
		if (!queuedDelegateQueue)
		{
			debugf("queuedDelegate Queue creation failed ");
		}
		else
		{
			debugf("queuedDelegate Queue creation ok ");
		}

		xTaskCreate( queuedDelegateFreeRTOSTask, (const signed char*)"QueuedDelegate", 1024, this, tskIDLE_PRIORITY, &queuedDelegateTask);
		if (!queuedDelegateTask)
		{
			debugf("queuedDelegate Task create failed");
		}
		else
		{
			debugf("queuedDelegate Task create ok");
		}
	}

	virtual ~QueuedDelegate() {
		vTaskDelete(queuedDelegateTask);
		vQueueDelete(queuedDelegateQueue);
	}

	virtual void setDelegate(Delegate<void(ParamType)> reqDelegate)
	{
		queuedDelegateDelegate = reqDelegate;
	}
	virtual bool sendQueue(ParamType reqParamType, bool fromISR = false)
	{
		if (fromISR)
		{
			return xQueueSendToBackFromISR ( queuedDelegateQueue, &reqParamType, NULL);
		}
		else
		{
			return xQueueSendToBack ( queuedDelegateQueue, &reqParamType, 0);
		}
	}

private :

	Delegate<void(ParamType)> queuedDelegateDelegate = NULL;

	xQueueHandle queuedDelegateQueue;
	xTaskHandle  queuedDelegateTask;

	static void queuedDelegateFreeRTOSTask(void *pvParameters)
	{
		QueuedDelegate* Self = (QueuedDelegate*) pvParameters;
		ParamType queuedDelegateParamtype;

		for (;;)
	    {
	        if (xQueueReceive(Self->queuedDelegateQueue, (void *)&queuedDelegateParamtype, (portTickType)portMAX_DELAY))
	        {
	        	if (Self->queuedDelegateDelegate)
	        	{
	        		Self->queuedDelegateDelegate(queuedDelegateParamtype);
	        	}
	        }
	    }
	    vTaskDelete(NULL);
	}
};

#endif /* SMING_CORE_QUEUEDDELEGATE_H_ */
