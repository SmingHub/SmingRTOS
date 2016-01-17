/*
 * HWTimer.h
 *
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 23.11.2015 by johndoe
 * http://github.com/anakod/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 ****/

#ifndef _SMING_CORE_HWTIMER_H_
#define _SMING_CORE_HWTIMER_H_

#include "Interrupts.h"
#include "Delegate.h"

#include "../include/sming_global.h"

#define MAX_HW_TIMER_INTERVAL_US 0x7fffff
#define MIN_HW_TIMER_INTERVAL_US 0x32

typedef Delegate<void()> TimerDelegate;

class HardwareTimer
{
public:
	HardwareTimer();
	virtual ~HardwareTimer();

	HardwareTimer& IRAM_ATTR initializeUs(uint32_t microseconds, InterruptCallback callback = NULL); // Init in Microseconds.

	bool IRAM_ATTR start(bool repeating = true);
	bool __forceinline IRAM_ATTR startOnce() { return start(false); }
	bool IRAM_ATTR stop();
	bool IRAM_ATTR restart();
	bool isStarted();

	uint32_t getIntervalUs();
    bool IRAM_ATTR setIntervalUs(uint32_t microseconds = 1000000);
    void IRAM_ATTR setCallback(InterruptCallback callback);


private:
    uint32_t interval = 0;
    InterruptCallback callback = nullptr;
    bool repeating = false;
    bool started = false;

    static void IRAM_ATTR hardwareTimerCallback(void *arg);

    void __forceinline IRAM_ATTR call() {
    	if (callback) {
    		callback();
    	}
	}


};

#endif /* _SMING_CORE_HWTIMER_H_ */
