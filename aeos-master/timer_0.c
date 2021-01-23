/**
 * SalimOS - Salim Operating System
 * by Sistemas Operacionais Embarcados - 2019/2 
 */

#include "timer_0.h"
#include "kernel.h"
#include "config.h"

int quantum = QUANTUM;

void conf_timer0()
{
  INTCONbits.PEIE     =       1;
  INTCONbits.TMR0IF   =       0;
  T0CONbits.T0CS      =       0;
  T0CONbits.PSA       =       0;
  T0CONbits.T0PS      =       0b111;
  TMR0L               =       252;
}

// Tratador de interrupção do timer
void __interrupt() timer_zero_int()
{
  int i;
  INTCONbits.TMR0IF = 0;
  TMR0L = 252;

  // Verifica se existem tarefas em espera (delay())
  //delay_decrement();
//  for (i = 0; i < f_aptos.size; i++) {
//    if (f_aptos.APTOS[i].task_state == WAITING && 
//        f_aptos.APTOS[i].task_delay_time > 0) {
//      if (--f_aptos.APTOS[i].task_delay_time == 0) {
//        f_aptos.APTOS[i].task_state = READY;
//      }
//    }
//  }

  // Chama o despachante para a troca de contexto por estouro do timer
  // Usado no algoritmo RR
  #if RR_SCHEDULER
  quantum = quantum - 1;
  if (quantum <= 0) {
    quantum = QUANTUM;
    dispatcher(READY);
  }
  #endif  
}
