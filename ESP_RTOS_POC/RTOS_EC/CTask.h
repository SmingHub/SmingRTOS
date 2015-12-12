/**
 * @file CTask.h
 * @class CTask
 * @ingroup FreeRTOS_Wrapper
 *
 * The CTask class wraps a native FreeRTOS task handle (xTaskHandle). To create a task instance
 * an object of this class and then call its CTask::Create method like showed in the following sample:
 * \code
 * CTask g_aTask // global task declaration.
 *
 * void main()
 * {
 * 	g_aTask.Create(task_control_func, "Led2", configMINIMAL_STACK_SIZE, mainFLASH_TASK_PRIORITY);
 *
 * 	CFreeRTOS::StartScheduler();
 * }
 * \endcode
 *
 * Usually the application subclass this class to create an application specific task object.
 * See the CHelloWorld class for an example.
 *
 * \sa <a href="http://www.freertos.org/taskandcr.html">task documentation</a> in the FreeRTOS web site.
 *
 * @date Jul 5, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */

#ifndef CTASK_H_
#define CTASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "IFreeRTOSProtocol.h"

class CTask: public IFreeRTOSObj {

	/**
	 * Specifies the native FreeRTOS handle managed by an instance of this class.
	 */
	xTaskHandle m_handleTask;

public:
	/**
	 * The default constructor. It initializes the object without attaching it to a native FreeRTOS handle.
	 * Call the Create method to allocate a task handle and attach it to the object.
	 */
	CTask();

	/**
	 * Create a CTask instance and attach it to a valid handle.
	 *
	 * @param handleTask a valid task handle.
	 */
	CTask(xTaskHandle handleTask);

	/**
	 * Delete the native FreeRTOS task.
	 */
	virtual ~CTask();

	/**
	 * Retrieve the native FreeRTOS task handle attached to this object.
	 *
	 * @return the native FreeRTOS task handle attached to this object.
	 */
	inline operator xTaskHandle() const { return m_handleTask; }

	// FreeRTOS API Wrappers

	/**
	 * Create a FreeRTOS task and attach its handle to the receiver object.
	 *
	 * \sa <a href="http://www.freertos.org/a00125.html">xTaskCreate</a> FreeRTOS API function.
	 */
	portBASE_TYPE Create(pdTASK_CODE pvTaskCode, const portCHAR * const pcName, unsigned portSHORT usStackDepth, void *pvParameters, unsigned portBASE_TYPE uxPriority);

	/**
	 * Create a Memory Protection Unit (MPU) restricted task and attach its handle to the receiver object.
	 *
	 * \sa <a href="http://www.freertos.org/xTaskCreateRestricted.html">xTaskCreateRestricted</a> FreeRTOS API function.
	 */
	portBASE_TYPE CreateRestricted(xTaskParameters *pxTaskDefinition);

	/**
	 * \sa <a href="http://www.freertos.org/a00126.html">vTaskDelete</a>  FreeRTOS API function.
	 */
	void Delete();

	/**
	 * \sa <a href="http://www.freertos.org/a00127.html">vTaskDelay</a>  FreeRTOS API function.
	 */
	inline static void Delay(portTickType xTicksToDelay);

	/**
	 * \sa <a href="http://www.freertos.org/vtaskdelayuntil.html">vTaskDelayUntil</a>  FreeRTOS API function.
	 */
    inline static void DelayUntil(portTickType *pxPreviousWakeTime, portTickType xTimeIncrement);

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskYIELD">taskYIELD</a>  FreeRTOS API function.
     */
    inline static void Yeld() { taskYIELD(); }

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskENTER_CRITICAL">taskENTER_CRITICAL</a>  FreeRTOS API function.
     */
    inline static void EnterCritical() { taskENTER_CRITICAL(); }

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskEXIT_CRITICAL">taskEXIT_CRITICAL</a>  FreeRTOS API function.
     */
    inline static void ExitCritical() { taskEXIT_CRITICAL(); }

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskDISABLE_INTERRUPTS">taskDISABLE_INTERRUPTS</a>  FreeRTOS API function.
     */
    inline static void DisableInterrupt() { taskDISABLE_INTERRUPTS(); }

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskENABLE_INTERRUPTS">taskENABLE_INTERRUPTS</a>  FreeRTOS API function.
     */
    inline static void EnableInterrupt() { taskENABLE_INTERRUPTS(); }

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#xTaskGetCurrentTaskHandle">xTaskGetCurrentTaskHandle</a>  FreeRTOS API function.
     */
    inline static xTaskHandle GetCurrentTaskHandle();

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#xTaskGetTickCount">xTaskGetTickCount</a>  FreeRTOS API function.
     */
    inline static portTickType GetTickCount();

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#xTaskGetSchedulerState">xTaskGetSchedulerState</a>  FreeRTOS API function.
     */
    inline static portBASE_TYPE GetSchedulerState();

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#usTaskGetNumberOfTasks">uxTaskGetNumberOfTasks</a>  FreeRTOS API function.
     */
    inline static unsigned portBASE_TYPE GetNumberOfTasks();

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#vTaskList">vTaskList</a>  FreeRTOS API function.
     */
    inline static void GetTaskList(portCHAR *pcWriteBuffer);

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#vTaskGetRunTimeStats">vTaskGetRunTimeStats</a>  FreeRTOS API function.
     */
    inline static void GetRunTimeStats(portCHAR *pcWriteBuffer);

    /**
     * \sa <a href="http://www.freertos.org/a00128.html">uxTaskPriorityGet</a>  FreeRTOS API function.
     */
    inline unsigned portBASE_TYPE PriorityGet() const;

    /**
     * \sa <a href="http://www.freertos.org/a00129.html">vTaskPrioritySet</a>  FreeRTOS API function.
     */
    inline void PrioritySet(unsigned portBASE_TYPE uxNewPriority);

    /**
     * \sa <a href="http://www.freertos.org/a00130.html">vTaskSuspend</a>  FreeRTOS API function.
     */
    inline void Suspend();

    /**
     * \sa <a href="http://www.freertos.org/a00131.html">vTaskResume</a>  FreeRTOS API function.
     */
    inline void Resume();

    /**
     * \sa <a href="http://www.freertos.org/taskresumefromisr.html">xTaskResumeFromISR</a>  FreeRTOS API function.
     */
    inline portBASE_TYPE ResumeFromISR();

    /**
     * \sa <a href="http://www.freertos.org/vTaskSetApplicationTag.html">vTaskSetApplicationTaskTag</a>  FreeRTOS API function.
     */
    inline void SetApplicationTag(pdTASK_HOOK_CODE pxTagValue);

    /**
     * \sa <a href="http://www.freertos.org/xTaskCallApplicationTaskHook.html">xTaskCallApplicationTaskHook</a>  FreeRTOS API function.
     */
    inline portBASE_TYPE CallApplicationTaskHook(void *pvParameter);

    /**
     * \sa <a href="http://www.freertos.org/vTaskAllocateMPURegions.html">vTaskAllocateMPURegions</a>  FreeRTOS API function.
     */
    inline void AllocateMPURegions(const xMemoryRegion * const xRegions);

    // FreeRTOS class extension.
	inline bool IsValid() const;
	void Attach(xGenericHandle handle);
	inline xGenericHandle Detach();
};

// inline method
////////////////

inline
void CTask::Delay(portTickType xTicksToDelay) {
#if ( INCLUDE_vTaskDelay == 1 )
	vTaskDelay(xTicksToDelay);
#endif
}

inline
void CTask::DelayUntil(portTickType *pxPreviousWakeTime, portTickType xTimeIncrement) {
#if ( INCLUDE_vTaskDelayUntil == 1 )
	vTaskDelayUntil(pxPreviousWakeTime, xTimeIncrement);
#endif
}

inline
xTaskHandle CTask::GetCurrentTaskHandle() {
	return xTaskGetCurrentTaskHandle();
}

inline
portTickType CTask::GetTickCount() {
	return xTaskGetTickCount();
}

inline
portBASE_TYPE CTask::GetSchedulerState() {
#if ( INCLUDE_xTaskGetSchedulerState == 1 )
	return xTaskGetSchedulerState();
#else
	return 0;
#endif
}

inline
unsigned portBASE_TYPE CTask::GetNumberOfTasks() {
	return uxTaskGetNumberOfTasks();
}

inline
void CTask::GetTaskList(portCHAR *pcWriteBuffer) {
#if ( configUSE_TRACE_FACILITY == 1 )
	vTaskList((signed portCHAR*)pcWriteBuffer);
#endif
}

inline
void CTask::GetRunTimeStats(portCHAR *pcWriteBuffer) {
#if (configGENERATE_RUN_TIME_STATS == 1)
	vTaskGetRunTimeStats((signed portCHAR*)pcWriteBuffer);
#endif
}

inline
unsigned portBASE_TYPE CTask::PriorityGet() const {
#if ( INCLUDE_uxTaskPriorityGet == 1 )
	return uxTaskPriorityGet(m_handleTask);
#else
	return -1;
#endif
}

inline
void CTask::PrioritySet(unsigned portBASE_TYPE uxNewPriority) {
#if ( INCLUDE_vTaskPrioritySet == 1 )
	vTaskPrioritySet(m_handleTask, uxNewPriority);
#endif
}

inline
void CTask::Suspend() {
#if ( INCLUDE_vTaskSuspend == 1 )
	vTaskSuspend(m_handleTask);
#endif
}

inline
void CTask::Resume() {
#if ( INCLUDE_vTaskSuspend == 1 )
	vTaskResume(m_handleTask);
#endif
}

inline
portBASE_TYPE CTask::ResumeFromISR() {
#if ( ( INCLUDE_xTaskResumeFromISR == 1 ) && ( INCLUDE_vTaskSuspend == 1 ) )
	return xTaskResumeFromISR(m_handleTask);
#else
	return pdFALSE;
#endif
}

inline
void CTask::SetApplicationTag(pdTASK_HOOK_CODE pxTagValue) {
#if ( configUSE_APPLICATION_TASK_TAG == 1 )
	vTaskSetApplicationTaskTag(m_handleTask, pxTagValue);
#endif
}

inline
portBASE_TYPE CTask::CallApplicationTaskHook(void *pvParameter) {
#if ( configUSE_APPLICATION_TASK_TAG == 1 )
	return xTaskCallApplicationTaskHook(m_handleTask, pvParameter);
#else
	return 0;
#endif
}

inline
void CTask::AllocateMPURegions(const xMemoryRegion * const xRegions) {
#if ( portUSING_MPU_WRAPPERS == 1 )
	vTaskAllocateMPURegions(m_handleTask, xRegions);
#endif
}

inline
bool CTask::IsValid() const{
	return m_handleTask != NULL;
}

inline
xGenericHandle CTask::Detach() {
	xTaskHandle res = m_handleTask;
	m_handleTask = NULL;
	return res;
}

#endif /* CTASK_H_ */
