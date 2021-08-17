#include "os.h"

cpu_t *stk_tmp;

cpu_t *PrepareStack(void *task, cpu_t *stk_p, int stk_size){
    cpu_t *stk = (cpu_t*)((int)stk_p + (stk_size - 1)*sizeof(cpu_t));

    *--stk = (cpu_t)0x01000000;
    *--stk = (cpu_t)task; // ADDRES task

    *--stk = 0; //
    *--stk = (cpu_t)0x12121212u;
    *--stk = (cpu_t)0x03030303u;
    *--stk = (cpu_t)0x02020202u;
    *--stk = (cpu_t)0x010101012;
    *--stk = (cpu_t)0;
    *--stk = (cpu_t)0xfffffffdu;
    *--stk = (cpu_t)0x11111111u;
    *--stk = (cpu_t)0x10101010u;
    *--stk = (cpu_t)0x09090909u;
    *--stk = (cpu_t)0x08080808u;
    *--stk = (cpu_t)0x07070707u;
    *--stk = (cpu_t)0x06060606u;
    *--stk = (cpu_t)0x05050505u;
    *--stk = (cpu_t)0x04040404u;

  return stk;
}


__attribute__ ((naked)) void SwitchContext(void){
  SAVE_CONTEXT();
  SAVE_SP();

  CLEAR_PENDSV();
  current_task->stk=stk_tmp;
  stk_tmp = scheduler();  
  
  RESTORE_SP();
  RESTORE_CONTEXT();

    /*// Entrada de interrupção
    OS_SAVE_ISR();

    // Manipulação de interrupções
    Clear_PendSV();

    // Saída de interrupções
    OS_EXIT_INT();
    OS_RESTORE_ISR();*/
}

void init_os_timer(void){
    uint32_t module = configCPU_CLOCK_HZ / (uint32_t)configTICK_RATE_HZ;

    *(NVIC_SYSTICK_CTRL) = 0;
    *(NVIC_SYSTICK_LOAD) = module - 1u;
    *(NVIC_SYSTICK_CTRL) = NVIC_SYSTICK_CLK | NVIC_SYSTICK_INT | NVIC_SYSTICK_ENABLE;
}

/*__attribute__ ((naked)) */void TickTimer(void){
  if(os_inc_and_compare()){
    SAVE_CONTEXT();
    SAVE_SP();
    
    current_task->stk=stk_tmp;
    stk_tmp = scheduler();
  
    RESTORE_SP();  
    RESTORE_CONTEXT();
  }
}

/*__attribute__ ((naked))*/ void SVCHandler(void){
    *(NVIC_SYSPRI3) |= NVIC_PENDSV_PRI;
    *(NVIC_SYSPRI3) |= NVIC_SYSTICK_PRI;
    RESTORE_SP();
    RESTORE_CONTEXT();
}/**/

/*uint32_t OS_CPU_SR_Save(void){
    uint32_t priority;
    __asm(                            \
            "MRS %0, PRIMASK      \n" \
            "CPSID I              \n" \
            : "=r" (priority)         \
              );
    return priority;
}

void OS_CPU_SR_Restore(uint32_t SR){
    __asm volatile("MSR PRIMASK, %0\n\t" :: "r" (SR));
}
*/
