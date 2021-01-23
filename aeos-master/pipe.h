/**
 * SalimOS - Salim Operating System
 * by Sistemas Operacionais Embarcados - 2019/2 
 */

#ifndef PIPE_H
#define	PIPE_H

#include "types.h"

#define PIPE_SIZE   2

typedef struct pipe {
    byte pipe_data[PIPE_SIZE];
    u_int pipe_size;
    u_int pipe_pos_read;
    u_int pipe_pos_write;
    u_int pipe_task_bloqued;
} t_pipe;

void pipe_create(t_pipe *p);
void pipe_read(t_pipe *p, byte* data);
void pipe_write(t_pipe *p, byte data);

#endif	/* PIPE_H */

