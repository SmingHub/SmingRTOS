/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/anakod/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 ****/

#ifndef _SMING_CORE_Timer_H_
#define _SMING_CORE_Timer_H_


#include "Interrupts.h"
#include "Delegate.h"
#include "../wiring/WiringFrameworkDependencies.h"
#include "../include/sming_global.h"
#include "espressif/esp_timer.h"
#include "QueuedDelegate.h"

// According to documentation maximum value of interval for ms
// timer after doing system_timer_reinit is 268435ms.
// If we do some testing we find that higher values works, 
// the actual limit seems to be about twice as high
// but we use the documented value anyway to be on the safe side.
#define MAX_OS_TIMER_INTERVAL_US 268435000

typedef Delegate<void()> TimerDelegate;

typedef struct
{
   uint32 timerMicros;
} QueuedTimerMessage;

typedef Delegate<void(QueuedTimerMessage)> QueuedTimerDelegate;

class Timer
{
public:
	Timer();
	Timer(bool reqQueued);
	~Timer();

	// It return value for Method Chaining (return "this" reference)
	// http://en.wikipedia.org/wiki/Method_chaining
	// We provide both versions: Delegate and classic c-style callback function for performance reason (for high-frequency timers)
	// Usually only Delegate needed

	Timer& initializeMs(uint32_t milliseconds, TimerDelegate delegateFunction, bool reqQueued); // Init in Milliseconds.
	Timer& initializeMs(uint32_t milliseconds, TimerDelegate delegateFunction = NULL); // Init in Milliseconds.

	void start(bool repeating = true);
	void startOnce() { start(false); }
	void stop();
	void restart();
	bool isStarted();

	bool isQueued = false;

	uint64_t getIntervalUs();
	uint32_t getIntervalMs();

    void setIntervalMs(uint32_t milliseconds = 1000000);
    void setCallback(TimerDelegate delegateFunction);
    void setCallback(TimerDelegate delegateFunction, bool reqQueued);
    void setQueued(bool reqQueued);

protected:
    static void IRAM_ATTR processing(void *arg);

private:
    os_timer_t timer;
    uint64_t interval = 0;
    TimerDelegate delegate_func = nullptr;
    bool repeating = false;
    bool started = false;
    
    // Because of the limitation in Espressif SDK a workaround
    // was added to allow for longer timer intervals.
    void setIntervalUs(uint64_t microseconds = 1000000);
    uint16_t long_intvl_cntr = 0;
    uint16_t long_intvl_cntr_lim = 0;

    QueuedDelegate<QueuedTimerMessage> * qd;
	void queueHandler(QueuedTimerMessage queuedTimerMessage);
};

#endif /* _SMING_CORE_Timer_H_ */
