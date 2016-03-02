#include "espressif/esp_common.h"

#include "sming/include/sming_config.h"

//#include "sming/include/SmingCore.h"

#ifdef ENABLE_GDB
	#include "../gdbstub/gdbstub.h"
#endif

extern void init();

extern "C" void  __attribute__((weak)) user_init(void)
{
	printf("Sming RTOS Proof of Concept \r\n");
	printf("SDK version:%s\n", system_get_sdk_version());
	printf("FreeHeapSize = %d\r\n",xPortGetFreeHeapSize());
#ifdef ENABLE_GDB
	gdbstub_init();
#endif
	init();
}

// For compatibility with SDK v1.1
extern "C" void __attribute__((weak)) user_rf_pre_init(void)
{
	// RTC startup fix, author pvvx
    volatile uint32 * ptr_reg_rtc_ram = (volatile uint32 *)0x60001000;
    if((ptr_reg_rtc_ram[24] >> 16) > 4) {
        ptr_reg_rtc_ram[24] &= 0xFFFF;
        ptr_reg_rtc_ram[30] &= 0;
    }
}
