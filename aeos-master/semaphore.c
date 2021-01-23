/**
 * SalimOS - Salim Operating System
 * by Sistemas Operacionais Embarcados - 2019/2 
 */

#include "types.h"
#include "kernel.h"


void sem_create(t_sem *sem, int init_value)
{
  DISABLE_INTERRUPTS();
  
  sem->contador             = init_value;
  sem->bloqued_tasks        = 0;
  sem->next_task_to_unblock = 0;
  
  ENABLE_INTERRUPTS();
}

void sem_wait(t_sem *sem)
{
  DISABLE_INTERRUPTS();
  
  sem->contador--;
  if (sem->contador < 0) {
    // Bloqueia tarefa
    sem->bloqueados[sem->bloqued_tasks] = f_aptos.task_running;
    sem->bloqued_tasks = (sem->bloqued_tasks+1) % (MAX_TASKS+1);
    dispatcher(W_SEMAPHORE);
  }  
  
  ENABLE_INTERRUPTS();
}

void sem_post(t_sem *sem)
{
  DISABLE_INTERRUPTS();
  
  sem->contador++;
  if (sem->contador <= 0) {
    // Desbloqueia tarefa (ordem FIFO)
    f_aptos.APTOS[sem->bloqueados[sem->next_task_to_unblock]].task_state = READY;
    sem->next_task_to_unblock = (sem->next_task_to_unblock+1) % sem->bloqued_tasks;
    dispatcher(READY);
  }
  
  ENABLE_INTERRUPTS();
}

int sem_get_value(t_sem sem)
{
  DISABLE_INTERRUPTS();
  
  return sem.contador;
  
  ENABLE_INTERRUPTS();
}

