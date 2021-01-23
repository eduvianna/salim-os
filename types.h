/**
 * SalimOS - Salim Operating System
 * by Sistemas Operacionais Embarcados - 2019/2 
 */


#ifndef TYPES_H
#define	TYPES_H

#include "config.h"

#define  ON                         1
#define  OFF                        2
#define  MAX_STACK_SIZE             32
#define  OS_ERROR                   0
#define  OS_OK                      1    
#define  IDLE                       0
#define  ENABLE_INTERRUPTS()        INTCONbits.GIE = 1;
#define  DISABLE_INTERRUPTS()       INTCONbits.GIE = 0;

typedef unsigned int u_int;
typedef u_int OS_RETURN;
typedef unsigned char byte;
typedef void(*task)(void);
typedef enum {READY = 1, RUNNING, WAITING, W_SEMAPHORE, FINISHED} t_state;

typedef struct stack_data {
    byte TOSU_reg;
    byte TOSL_reg;
    byte TOSH_reg;    
} t_stack_data;

typedef struct stack {
    int stack_size;
    t_stack_data stack_data[MAX_STACK_SIZE+1];
} t_stack;

typedef struct tcb {
    u_int task_id;
    u_int task_priority;
    t_state task_state;
    t_stack task_stack;
    byte W_reg;
    byte STATUS_reg;
    byte BSR_reg;
    task task_ptr;
    int task_delay_time;
} t_tcb;

typedef struct fila_aptos {
    t_tcb APTOS[MAX_TASKS];
    int size;
    u_int task_running;
} t_fila_aptos;

typedef struct sem {
    u_int bloqueados[MAX_TASKS];
    int bloqued_tasks;
    int next_task_to_unblock;
    int contador;
} t_sem;


#endif	/* TYPES_H */

