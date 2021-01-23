/**
 * SalimOS - Salim Operating System
 * by Sistemas Operacionais Embarcados - 2019/2 
 */

#ifndef SEMAPHORE_H
#define	SEMAPHORE_H

#include "types.h"

void sem_create(t_sem *sem, int init_value);
void sem_wait(t_sem *sem);
void sem_post(t_sem *sem);
int sem_get_value(t_sem sem);


#endif	/* SEMAPHORE_H */

