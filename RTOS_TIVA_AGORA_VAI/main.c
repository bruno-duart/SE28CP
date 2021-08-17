
#include "Include/os.h"
#include "Include/tasks.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"


#ifdef __cplusplus
 extern "C"
#endif
void MCU_init(void); /* Device initialization function declaration */
                                

cpu_t stk1[128];
cpu_t stk2[128];
cpu_t stk3[128];
cpu_t stk4[128];

TCB_t tcb1, tcb2, tcb3, tcb4;


int main(void) {
   MAP_SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);

   // Run from the PLL at 120 MHz.
   (void)MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
               SYSCTL_OSC_MAIN |
               SYSCTL_USE_PLL |
               SYSCTL_CFG_VCO_480), 120000000);

   // Enable stacking for interrupt handlers.  This allows floating-point
   // instructions to be used within interrupt handlers, but at the expense of
   // extra stack usage.
  /* MAP_FPUEnable();
   MAP_FPULazyStackingEnable();*/

  /* include your code here */
  InstallTask(&tcb1, task1, 2, stk1, sizeof(stk1));
  InstallTask(&tcb2, task2, 3, stk2, sizeof(stk2));
  InstallTask(&tcb3, task3, 4, stk3, sizeof(stk3));
  InstallTask(&tcb4, task4, 1, stk4, sizeof(stk4));
  
  start_os();

  for(;;) {
    /* __RESET_WATCHDOG(); by default, COP is disabled with device init. When enabling, also reset the watchdog. */
  } /* loop forever */
  /* please make sure that you never leave main */
  return 0;
}
