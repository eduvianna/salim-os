/**
 * SalimOS - Salim Operating System
 * by Sistemas Operacionais Embarcados - 2019/2 
 */

#ifndef KERNEL_H
#define	KERNEL_H

#include <xc.h>
#include "types.h"

//#define SAVE_CONTEXT(state) f_save_context(state);
//#define RESTORE_CONTEXT() f_restore_context();

extern t_fila_aptos f_aptos;
int stack_size = 0;

OS_RETURN create_task(u_int id, u_int prior, task f);
u_int get_tid();
void delay_os(int time);
short int rr_scheduler();
u_int prior_scheduler();
void os_config();
void start_OS();
void dispatcher(t_state state);
void f_save_context(t_state state);
void f_restore_context();
void task_idle();
void delay_decrement();

// Rotinas para tratamento de contexto
#define SAVE_CONTEXT(state) {\
  if (f_aptos.APTOS[f_aptos.task_running].task_state == RUNNING) {\
    f_aptos.APTOS[f_aptos.task_running].task_state  = state;\
    f_aptos.APTOS[f_aptos.task_running].W_reg       = WREG;\
    f_aptos.APTOS[f_aptos.task_running].STATUS_reg  = STATUS;\
    f_aptos.APTOS[f_aptos.task_running].BSR_reg     = BSR;\
    stack_size = 0;\
    while (STKPTR) {\
        f_aptos.APTOS[f_aptos.task_running].task_stack.stack_data[stack_size].TOSU_reg = TOSU;\
        f_aptos.APTOS[f_aptos.task_running].task_stack.stack_data[stack_size].TOSH_reg = TOSH;\
        f_aptos.APTOS[f_aptos.task_running].task_stack.stack_data[stack_size].TOSL_reg = TOSL;\
        stack_size++;\
        __asm("POP");\
    }\
    f_aptos.APTOS[f_aptos.task_running].task_stack.stack_size = stack_size;\
  }\
}\

#define RESTORE_CONTEXT() {\
    STKPTR = 0;\
    if (f_aptos.APTOS[f_aptos.task_running].task_stack.stack_size == 0) {\
        __asm("PUSH");\
        TOS = f_aptos.APTOS[f_aptos.task_running].task_ptr;\
    }\
    else {\
        WREG        = f_aptos.APTOS[f_aptos.task_running].W_reg;\
        STATUS      = f_aptos.APTOS[f_aptos.task_running].STATUS_reg;\
        BSR         = f_aptos.APTOS[f_aptos.task_running].BSR_reg;\
        stack_size  = f_aptos.APTOS[f_aptos.task_running].task_stack.stack_size-1;\    
        while (stack_size) {\
            __asm("PUSH");\
            TOSL = f_aptos.APTOS[f_aptos.task_running].task_stack.stack_data[stack_size].TOSL_reg;\
            TOSH = f_aptos.APTOS[f_aptos.task_running].task_stack.stack_data[stack_size].TOSH_reg;\
            TOSU = f_aptos.APTOS[f_aptos.task_running].task_stack.stack_data[stack_size].TOSU_reg;\
            stack_size--;\    
        }\
    }\
    f_aptos.APTOS[f_aptos.task_running].task_state = RUNNING;\
}\
// Fim das rotinas para tratamento de contexto

#endif	/* KERNEL_H */
