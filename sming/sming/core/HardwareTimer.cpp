/*
 * HWTimer.cpp
 *
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 23.11.2015 by johndoe
 * http://github.com/anakod/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 ****/

#include "HardwareTimer.h"
#include "../system/hw_timer.h"

#include "espressif/esp_common.h"


HardwareTimer::HardwareTimer()
{
	hw_timer_attach_func((void*)hardwareTimerCallback,this);
}

HardwareTimer::~HardwareTimer()
{
	hw_timer_attach_func(NULL,NULL);
	stop();
}

void IRAM_ATTR HardwareTimer::hardwareTimerCallback(void *arg)
{
	if (arg == null) return;
	HardwareTimer *ptimer = (HardwareTimer*)arg;
	ptimer->call();
}


HardwareTimer& HardwareTimer::initializeUs(uint32_t microseconds, InterruptCallback callback)
{
	setCallback(callback);
	setIntervalUs(microseconds);
	return *this;
}

bool HardwareTimer::start(bool repeating/* = true*/)
{
	this->repeating = repeating;
	stop();
	if(interval == 0 || !callback)
		return started;

	if (this->repeating == 0)
	{
		hw_timer_init_noattach(0);
	}
	else
	{
		hw_timer_init_noattach(1);
	}
	started = true;
	hw_timer_arm(interval);

	return started;
}

bool HardwareTimer::stop()
{
	if (!started) return started;

	hw_timer_stop();
	started = false;

	return started;
}

bool HardwareTimer::restart()
{
	stop();
	start(repeating);
	return started;
}

bool HardwareTimer::isStarted()
{
	return started;
}

uint32_t HardwareTimer::getIntervalUs()
{
	return interval;
}

bool HardwareTimer::setIntervalUs(uint32_t microseconds/* = 1000000*/)
{
	if(microseconds < MAX_HW_TIMER_INTERVAL_US && microseconds > MIN_HW_TIMER_INTERVAL_US)
	{
		interval = microseconds;
		if (started)
			restart();
	}
	else
	{
		stop();
	}
	return started;
}

void HardwareTimer::setCallback(InterruptCallback interrupt)
{
	callback = interrupt;

	if (!interrupt)
		stop();
}
