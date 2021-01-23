/**
 * SalimOS - Salim Operating System
 * by Sistemas Operacionais Embarcados - 2019/2 
 */

#ifndef TIMER_0_H
#define	TIMER_0_H

#include <pic18f4520.h>
#include <xc.h>

// Configura TIMER ZERO
void conf_timer0();

// Tratador de interrupção do timer
void __interrupt() timer_zero_int();

#endif	/* TIMER_0_H */

