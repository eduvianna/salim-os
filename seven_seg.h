#ifndef SEVEN_SEG_H
#define SEVEN_SEG_H

#include <xc.h>


void seven_seg_init(void);
void seven_seg_set(int number);
void update_ports(int number, int display);

#endif

