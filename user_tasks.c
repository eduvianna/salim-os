/**
 * SalimOS - Salim Operating System
 * by Sistemas Operacionais Embarcados - 2019/2 
 */

#include <pic18f4520.h>
#include "user_tasks.h"
#include "kernel.h"
#include "sralloc.h"
#include "semaphore.h"
#include "pipe.h"
#include "config_hardware.h"
#include <stdio.h>
#include "adc.h"
#include <xc.h>
#include "seven_seg.h"

byte *buffer;
t_sem sem1, sem2, sem3, sem4, sem5;
t_pipe msg1, msg2;
int BATTERY = 99;

// Função obrigatória
void user_tasks_config()  {  
    // Configuração das portas de comunicação que as tarefas
    // de usuário irão manipular
    TRISDbits.RD0 = 0;
    TRISDbits.RD4 = 0;
    TRISDbits.RD2 = 0;
    
    //buttons init
    TRISDbits.RD6 = 0;
    TRISDbits.RD7 = 0;
    // Idle
    TRISDbits.RD3 = 0;
    TRISBbits.RB0 = 0;
    TRISBbits.RB1 = 0;
    
    // Inicialização das portas para output
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD7 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB7 = 1;
    
    // Motores
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    
    //seven segments
    seven_seg_init();
  
    buffer = SRAMalloc(10);

    sem_create(&sem1, 1);
    sem_create(&sem2, 0);
    sem_create(&sem3, 0);
    sem_create(&sem4, 0);
    pipe_create(&msg1);
    pipe_create(&msg2);
}

// Funções de usuários
void task_um() {
    while (1) {
        sem_wait(&sem2);
        for(int i=0; i>=1000; i++) {
            Nop();
        }
        BATTERY = BATTERY - 1;
        PORTDbits.RD0 = ~PORTDbits.RD0;
        sem_post(&sem3);
    }
}

void task_dois() {
    while (1) {
        sem_wait(&sem3);
        for(int i=0; i>=5000; i++) {
            Nop();
        }
        BATTERY = BATTERY - 1;
        PORTDbits.RD2 = ~PORTDbits.RD2;
        sem_post(&sem4);
    }
}

void task_disp() {
    while (1) {
        //int battery_voltage = (int) read_adc();
        //int tensao = (int) (tensaoBateria*5.0 / 1024.0);
        //float battery_charge = tensao * 20.0;
        sem_wait(&sem1);
        /*if(BATTERY < 50 && BATTERY > 25) {
            pipe_write(&msg1, '0');
        }
        else if(BATTERY < 25) {
            pipe_write(&msg1, '1');
        }*/
        seven_seg_set(BATTERY);
        sem_post(&sem2);
    }
}



void task_m1() {
    byte dado;
    while (1) {
        sem_wait(&sem4);
        /*
        pipe_read(&msg1, dado); 
        if(dado == '0' || dado == '1') {
            PORTDbits.RD4 = 1;
            BATTERY = BATTERY + 1;
        }
        */
        if(BATTERY < 50 && BATTERY > 25){
            PORTDbits.RD4 = 1;
            BATTERY = BATTERY + 1;
        }
        else {
            PORTDbits.RD4 = 0;
        }
        sem_post(&sem5);
    }
}

void task_m2(){
    byte dado;
    while (1) {
        sem_wait(&sem5);
        /*
        pipe_read(&msg1, dado); 
        if(dado == '0' || dado == '1') {
            PORTDbits.RD5 = 1;
            BATTERY = BATTERY + 1;
        }
        */
        if(BATTERY < 25){
            PORTDbits.RD5 = 1;
            BATTERY = BATTERY + 1;
        }
        else {
            PORTDbits.RD5 = 0;
        }
        sem_post(&sem1);
    }
}




