/**
 * SalimOS - Salim Operating System
 * by Sistemas Operacionais Embarcados - 2019/2 
 */

#include "pipe.h"
#include "kernel.h"

void pipe_create(t_pipe *p)
{
  p->pipe_size            = 0;
  p->pipe_pos_read        = 0;
  p->pipe_pos_write       = 0;
  p->pipe_task_bloqued    = 0;
}

void pipe_read(t_pipe *p, byte* data)
{
  DISABLE_INTERRUPTS();
  
  if (p->pipe_size > 0) {
    *data = p->pipe_data[p->pipe_pos_read];
    p->pipe_pos_read = (p->pipe_pos_read + 1) % PIPE_SIZE;
    p->pipe_size = p->pipe_size - 1;    
    // Desbloqueia processo que eventualmente foi bloqueado no pipe
    if (p->pipe_task_bloqued > 0) {
      f_aptos.APTOS[p->pipe_task_bloqued].task_state = READY;
      p->pipe_task_bloqued = 0;
      #if PRIOR_SCHEDULER
        dispatcher(READY);
      #endif
    }
  }
  else {
    p->pipe_task_bloqued = f_aptos.task_running;
    dispatcher(WAITING);
  }
  
  ENABLE_INTERRUPTS();  
}

void pipe_write(t_pipe *p, byte data)
{
  DISABLE_INTERRUPTS();
  
  if (p->pipe_size < PIPE_SIZE) {
    p->pipe_data[p->pipe_pos_write] = data;
    p->pipe_pos_write = (p->pipe_pos_write + 1) % PIPE_SIZE;
    p->pipe_size = p->pipe_size + 1;    
    if (p->pipe_task_bloqued > 0) {
      f_aptos.APTOS[p->pipe_task_bloqued].task_state = READY;
      p->pipe_task_bloqued = 0;
      #if PRIOR_SCHEDULER
        dispatcher(READY);
      #endif
    }
  }
  else {
    p->pipe_task_bloqued = f_aptos.task_running;
    dispatcher(WAITING);
  }
  
  ENABLE_INTERRUPTS();  
}
