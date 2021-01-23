/**
 * SalimOS - Salim Operating System
 * by Sistemas Operacionais Embarcados - 2019/2 
 */

#include "kernel.h"
#include "user_tasks.h"
#include "timer_0.h"
#include "config.h"

t_fila_aptos f_aptos;

// System calls
OS_RETURN create_task(u_int id, u_int prior, task f)
{  
  if (f_aptos.size >= MAX_TASKS)
    return OS_ERROR;
  
  f_aptos.APTOS[f_aptos.size].task_id                = id;
  f_aptos.APTOS[f_aptos.size].task_priority          = prior;
  f_aptos.APTOS[f_aptos.size].task_ptr               = f;
  f_aptos.APTOS[f_aptos.size].task_delay_time        = 0;
  f_aptos.APTOS[f_aptos.size].task_state             = READY;
  f_aptos.APTOS[f_aptos.size].task_stack.stack_size  = 0;
  
  f_aptos.size += 1;
  
  return OS_OK;
}

u_int get_tid()
{
  return f_aptos.APTOS[f_aptos.task_running].task_id;
}

void delay_os(int time)
{
  DISABLE_INTERRUPTS();
  
  f_aptos.APTOS[f_aptos.task_running].task_delay_time = time;
  
  dispatcher(WAITING);
  
  ENABLE_INTERRUPTS();
}

short int rr_scheduler()
{
  short int next_task = f_aptos.task_running;
  
  do {
    next_task = (next_task+1) % f_aptos.size;
  } while (f_aptos.APTOS[next_task].task_state != READY);
  
  return next_task;
}

u_int rr_scheduler_1()
{
  u_int next_task_to_run = f_aptos.task_running, times = 1;
  
  do {
    next_task_to_run++;
    if (next_task_to_run < f_aptos.size) {
      if (f_aptos.APTOS[next_task_to_run].task_state == READY) {
        return next_task_to_run;
      }
    }
    else {
      next_task_to_run = 0;
      times++;
    }       
        
  } while (times < 3);
  
  return IDLE;
}

u_int prior_scheduler()
{
  // Prioridade diretamente proporcional
  u_int k, maior = 0;
  
  for (k = 1; k < f_aptos.size; k++) {
    if (f_aptos.APTOS[k].task_priority > f_aptos.APTOS[maior].task_priority)
      maior = k;
  }
  
  return maior;
}

void os_config()
{
  f_aptos.size          = 0;
  f_aptos.task_running  = 0;
  
  // Criar tarefa idle
  create_task(0, 0, &task_idle);
  
  conf_timer0();
  
  user_tasks_config();  
}

void start_OS()
{ 
  ENABLE_INTERRUPTS();
  INTCONbits.TMR0IE   = 1;
  T0CONbits.TMR0ON    = ON;
}

void task_idle() {
  while (1) {
    PORTDbits.RD3 = ~PORTDbits.RD3;
    Nop();
  }
}

void dispatcher(t_state state)
{
  DISABLE_INTERRUPTS();
  
  SAVE_CONTEXT(state);
  
#if RR_SCHEDULER
  f_aptos.task_running = rr_scheduler();
  //f_aptos.task_running = rr_scheduler_1();
#else
  fila_aptos.task_running = prior_scheduler();
#endif
  
  RESTORE_CONTEXT();
  
  ENABLE_INTERRUPTS();
}

void f_save_context(t_state state) {
  if (f_aptos.APTOS[f_aptos.task_running].task_state == RUNNING) {
    stack_size = 0;
    f_aptos.APTOS[f_aptos.task_running].task_state = state;
    f_aptos.APTOS[f_aptos.task_running].BSR_reg = BSR;
    f_aptos.APTOS[f_aptos.task_running].STATUS_reg = STATUS;
    f_aptos.APTOS[f_aptos.task_running].W_reg = WREG;
    do {      
      f_aptos.APTOS[f_aptos.task_running].task_stack.stack_data[stack_size].TOSU_reg = TOSU;\
      f_aptos.APTOS[f_aptos.task_running].task_stack.stack_data[stack_size].TOSH_reg = TOSH;\
      f_aptos.APTOS[f_aptos.task_running].task_stack.stack_data[stack_size].TOSL_reg = TOSL;\
      stack_size++;
      __asm("POP");
    } while (STKPTR);
    f_aptos.APTOS[f_aptos.task_running].task_stack.stack_size = stack_size;
  }  
}

void f_restore_context()
{
  stack_size = f_aptos.APTOS[f_aptos.task_running].task_stack.stack_size;
  f_aptos.APTOS[f_aptos.task_running].task_state = RUNNING;
  STKPTR = 0;
  if (stack_size == 0) {
    __asm("PUSH");
    TOS = f_aptos.APTOS[f_aptos.task_running].task_ptr;
    f_aptos.APTOS[f_aptos.task_running].task_stack.stack_size++;
  } else {
    WREG = f_aptos.APTOS[f_aptos.task_running].W_reg;
    BSR = f_aptos.APTOS[f_aptos.task_running].BSR_reg;
    STATUS = f_aptos.APTOS[f_aptos.task_running].STATUS_reg;
    do {
      __asm("PUSH");
      TOSL = f_aptos.APTOS[f_aptos.task_running].task_stack.stack_data[stack_size].TOSL_reg;\
      TOSH = f_aptos.APTOS[f_aptos.task_running].task_stack.stack_data[stack_size].TOSH_reg;\
      TOSU = f_aptos.APTOS[f_aptos.task_running].task_stack.stack_data[stack_size].TOSU_reg;\
      stack_size--;
    } while (stack_size >= 0);
  }
}

void delay_decrement()
{
  int i;
  for (i = 0; i < f_aptos.size; i++) {
    if (f_aptos.APTOS[i].task_state == WAITING && 
        f_aptos.APTOS[i].task_delay_time > 0) {
      f_aptos.APTOS[i].task_delay_time = f_aptos.APTOS[i].task_delay_time - 1;
      if (f_aptos.APTOS[i].task_delay_time == 0) {
        f_aptos.APTOS[i].task_state = READY;
      }
    }
  }
}
