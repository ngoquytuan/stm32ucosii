/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name		: os_cpu_c.c 
* Author		: Librae 
* Date			: 08/10/2010
* Description	: uCOS-II STM32 On the C language part of the porting code,
* Includes task stack initialization code and hook functions, etc.

******************************************************************************/

#define  OS_CPU_GLOBALS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include "uCOS_II.H"
#include "os_cpu.h"
#include <os_cfg_r.h>

/******************************************************************************************************************
*                                    INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              p_arg         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_xxx).
*
* Returns    : Always returns the location of the new top-of-stack once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : 1) Interrupts are enabled when your task starts executing.
*              2) All tasks run in Thread mode, using process stack.
*
******************************************************************************************************************/

OS_STK *OSTaskStkInit (void  (*task)(void  *parg), void  *parg, OS_STK  *ptos, INT16U  opt)
{
    OS_STK *stk;


    (void)opt;                                        /*  'opt' is not used, prevent warning   */
                                                      
                                                                            
    stk = ptos;                                       /*  Load stack pointer          */
                                                                                       

                                                      /*  Registers stacked as if     */
                                                      /*  auto-saved on exception     */             
	                                                

    *(stk) = (INT32U)0x01000000L;                     /*  xPSR                        */ 
    *(--stk) = (INT32U)task;                          /*  Entry Point of the task     */
    *(--stk) = (INT32U)0xFFFFFFFEL;                   /*  R14 (LR)  (init value will  */
                                                      /*  cause fault if ever used)   */                                                                           
    *(--stk) = (INT32U)0x12121212L;                   /*  R12                         */
    *(--stk) = (INT32U)0x03030303L;                   /*  R3                          */
    *(--stk) = (INT32U)0x02020202L;                   /*  R2                          */
    *(--stk) = (INT32U)0x01010101L;                   /*  R1                          */
    *(--stk) = (INT32U)parg;                          /*  R0 : argument  */
                                                      /*  Remaining registers saved on*/
                                                      /*  process stack               */   
    *(--stk) = (INT32U)0x11111111L;                   /*  R11                         */
    *(--stk) = (INT32U)0x10101010L;                   /*  R10                         */
    *(--stk) = (INT32U)0x09090909L;                   /*  R9                          */
    *(--stk) = (INT32U)0x08080808L;                   /*  R8                          */
    *(--stk) = (INT32U)0x07070707L;                   /*  R7                          */
    *(--stk) = (INT32U)0x06060606L;                   /*  R6                          */
    *(--stk) = (INT32U)0x05050505L;                   /*  R5                          */
    *(--stk) = (INT32U)0x04040404L;                   /*  R4                          */

    return(stk);
}

 /*********************************************************************************************************
   *                                            TASK RETURN HOOK
   *
   * Description: This function is called if a task accidentally returns.  In other words, a task should
   *              either be an infinite loop or delete itself when done.
   *
   * Arguments  : p_tcb        Pointer to the task control block of the task that is returning.
   *
   * Note(s)    : None.
   *********************************************************************************************************
   */
   
   void  OSTaskReturnHook (OS_TCB  *p_tcb)
   {
   #if OS_CFG_APP_HOOKS_EN > 0u
       if (OS_AppTaskReturnHookPtr != (OS_APP_HOOK_TCB)0) {
           (*OS_AppTaskReturnHookPtr)(p_tcb);
       }
   #else
       (void)p_tcb;                                            /* Prevent compiler warning                               */
   #endif
   }
	
/* 以下为一些钩子函数，全部为空函数。具体说明请看相关资料 */

#if OS_CPU_HOOKS_EN
/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookBegin (void)
{
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookEnd (void)
{
}
#endif


/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}




/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskDelHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
void OSTaskSwHook (void)
{
}

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/
void OSTaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OSTCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSTCBInitHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                                           /* Prevent Compiler warning                 */
}
#endif


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
void OSTimeTickHook (void)
{
}


/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION >= 251
void OSTaskIdleHook (void)
{
}

#endif

#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
