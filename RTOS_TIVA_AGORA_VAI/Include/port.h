#include <stdint.h>

// Registradores ARM Cortex-Mx
#define NVIC_SYSTICK_CTRL ((volatile unsigned long *) 0xe000e010)
#define NVIC_SYSTICK_LOAD ((volatile unsigned long *) 0xe000e014)
#define NVIC_INT_CTRL     ((volatile unsigned long *) 0xe000ed04)
#define NVIC_SYSPRI3      ((volatile unsigned long *) 0xe000ed20)

// Constantes
#define NVIC_SYSTICK_CLK        0x00000004
#define NVIC_SYSTICK_INT        0x00000002
#define NVIC_SYSTICK_ENABLE     0x00000001

#define configCPU_CLOCK_HZ      120000000
#define configTICK_RATE_HZ      1000 //Ticktype_t

#define NVIC_PENDSVSET  0x10000000
#define NVIC_PENDSVCLR  0x08000000

/* Definições específicas do Cortex-M */
#define PRIO_BITS                       4   // 16 níveis de prioridade
#define LOWEST_INTERRUPT_PRIORITY       0xF
#define KERNEL_INTERRUPT_PRIORITY       (LOWEST_INTERRUPT_PRIORITY << (8 - PRIO_BITS) )

// Prioridades de interrupção de Kernel
#define NVIC_PENDSV_PRI                 ( ( ( unsigned long ) KERNEL_INTERRUPT_PRIORITY ) << 16 )
#define NVIC_SYSTICK_PRI                ( ( ( unsigned long ) KERNEL_INTERRUPT_PRIORITY ) << 24 )

typedef unsigned long cpu_t;

extern cpu_t *stk_tmp;

void init_os_timer(void);
cpu_t *PrepareStack(void *task, cpu_t *stk, int stk_size);
//////////////////////////////////////////////////
#define CLEAR_PENDSV(void)  *(NVIC_INT_CTRL) = NVIC_PENDSVCLR

#define dispatcher() __asm( "cpsie i                \n"   \
                            "svc 0                  \n")
        
#define SAVE_CONTEXT() __asm( "MRS R0, PSP          \n"   \
                              "STMDB R0!, {R4-R11, R14} \n")

#define RESTORE_CONTEXT() __asm(                             \
        /* Restore r4-11 from new process stack*/            \
        "LDMIA      R0!, {R4-R11, R14}      \n"              \
        /* Load PSP with new process SP*/                    \
        "MSR        PSP, R0                 \n"              \
        "ORR        LR,LR,#0X04             \n"              \
        /*Exception return will restore remaining context */ \
        "CPSIE      I                       \n"              \
        "BX         LR                      \n"              \
     )

        
#define SAVE_SP() __asm( "LDR R1, =stk_tmp      \n"     \
                         "STR R0, [R1]          \n")
        
#define RESTORE_SP() __asm("LDR R1, =stk_tmp    \n"     \
                           "LDR R0, [R1]        \n")
        
#define yield() *(NVIC_INT_CTRL) = NVIC_PENDSVSET
        
