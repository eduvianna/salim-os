/**
 * SalimOS - Salim Operating System
 * by Sistemas Operacionais Embarcados - 2019/2 
 */

#define _XTAL_FREQ 4000000

#include <xc.h>
#include "kernel.h"
#include "user_tasks.h"
#include "sralloc.h"

#pragma config WDT      = OFF
#pragma config PBADEN   = OFF


__asm("GLOBAL _task_idle, _task_lcd, _task_um, _task_dois, _task_tres, _task_m1, _task_m2");


int main(void) {  
  // Configura o SO
  os_config();
    
  SRAMInitHeap();

  // Cria tarefas de usuarios
  if (create_task(1, 6, &task_lcd) == OS_ERROR) return OS_ERROR;
  if (create_task(2, 5, &task_um) == OS_ERROR) return OS_ERROR;
  if (create_task(3, 4, &task_dois) == OS_ERROR) return OS_ERROR;
  if (create_task(4, 3, &task_tres) == OS_ERROR) return OS_ERROR;
  //if (create_task(5, 2, &task_m1) == OS_ERROR) return OS_ERROR;
  //if (create_task(6, 1, &task_m2) == OS_ERROR) return OS_ERROR;

  // Inicia o sistema operacional
  start_OS();
  
  while (1);
  
  return OS_OK;
}