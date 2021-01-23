/*
 * File:   seven_seg.c
 * Author: luan
 *
 * Created on May 30, 2019, 3:07 PM
 */
#include <xc.h>

void seven_seg_init(void) {
    // seta os tris maluku
    TRISC = 0b00000000;
    PORTC = 0b00000000;
}

void update_ports(int number, int display) {
    if(display == 0) {
        switch(number) {
            case 0:  PORTC = 0b00000000; break;
            case 1:  PORTC = 0b00010000; break;
            case 2:  PORTC = 0b00100000; break;
            case 3:  PORTC = 0b00110000; break;
            case 4:  PORTC = 0b01000000; break;
            case 5:  PORTC = 0b01010000; break;
            case 6:  PORTC = 0b01100000; break;
            case 7:  PORTC = 0b01110000; break;
            case 8:  PORTC = 0b10000000; break;
            case 9:  PORTC = 0b10010000; break;
            default: PORTC = 0b00000000;
        }
    } else {
        switch(number) {
            case 0:  PORTC = 0b00000000; break;
            case 1:  PORTC = 0b00000001; break;
            case 2:  PORTC = 0b00000010; break;
            case 3:  PORTC = 0b00000011; break;
            case 4:  PORTC = 0b00000100; break;
            case 5:  PORTC = 0b00000101; break;
            case 6:  PORTC = 0b00000110; break;
            case 7:  PORTC = 0b00000111; break;
            case 8:  PORTC = 0b00001000; break;
            case 9:  PORTC = 0b00001001; break;
            default: PORTC = 0b00000000;
        }
    }
}

void seven_seg_set(int number) { // entrada 7 por exemplo
    if(number > 9) {
        int last    = number % 10;
        int first   = number / 10;
        update_ports(last, 0);
        PORTC = PORTC & 0b11110000 | first;
    } else {
        update_ports(number, 1);
    }
}
