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
#include "lcd.h"
#include "config_hardware.h"
#include "delay.h"
#include "adc.h"
#include <stdio.h>
#include <xc.h>

byte *buffer;
t_sem sem1, sem2, sem3, sem4;
t_pipe msg1, msg2;

// Função obrigatória
void user_tasks_config()  {  
    // Configuração das portas de comunicação que as tarefas
    // de usuário irão manipular
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    // Idle
    TRISDbits.TRISD3 = 0;
    
    // Inicialização das portas para output
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD7 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;
    
    // Motores
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
  
    buffer = SRAMalloc(10);

    sem_create(&sem1, 1);
    sem_create(&sem2, 0);
    sem_create(&sem3, 0);
    sem_create(&sem4, 0);
    pipe_create(&msg1);
    pipe_create(&msg2);
    Inicializa_ADC();
    Lcd_Init();
}

// Funções de usuários
void task_um() {
    byte dado;
    while (1) {
        //delay_os(300);
        // Reading battery charge state: 0: high, 1:low, 2:very low
        // Writing the state to motors
        sem_wait(&sem2);
        //pipe_read(&msg1, &dado);
        
        PORTDbits.RD0 = ~PORTDbits.RD0;
        
      // pipe_write(&msg2, dado);
        sem_post(&sem2);
    }
}

void task_dois() {
    byte dado;
    while (1) {
        //delay_os(100);
    //    pipe_read(&msg1, &dado);
        // Writing the state to motors
        sem_wait(&sem2);
        PORTDbits.RD1 = ~PORTDbits.RD1;
      
        //      pipe_write(&msg2, dado);
        sem_post(&sem2);
    }
}

void task_tres() {
    byte dado;
    while (1) {
        //delay_os(200);
//        pipe_read(&msg1, &dado);
        // Writing the state to motors
        sem_wait(&sem2);
        PORTDbits.RD2 = ~PORTDbits.RD2;

        //pipe_write(&msg2, dado);
        sem_post(&sem1);
  }
}

void task_lcd() {
    unsigned char i;
    unsigned int tensaoBateria;
    float tensao, percentualBateria;
    unsigned char s[16];
  
    while(1) {
        
        MsDelay(500);
        
        // Motor 1
        sem_wait(&sem1);
            
        tensaoBateria = LerADC();
        tensao = tensaoBateria*5.0/1024.0;
        percentualBateria = tensao * 20.0;

        Lcd_Clear();
        Lcd_Set_Cursor(1,5);
        Lcd_Write_String("BATTERY %");
        Lcd_Set_Cursor(2,5);

        sprintf(s, "%04.2f", percentualBateria);
        Lcd_Write_String(s);
        if(percentualBateria < 50 && percentualBateria > 25){
            pipe_write(&msg1, '0');
            sem_post(&sem3);
        }
        // Motor 2
        if(percentualBateria < 25 && percentualBateria > 10){
            pipe_write(&msg1, '1');
            sem_post(&sem3);
        }
        if(percentualBateria < 10){
            pipe_write(&msg1, '2');
            sem_post(&sem3);
        }
        PORTCbits.RC0 = 1;
        sem_post(&sem2);
        //delay_os(500);
    }
}



void task_m1(){
    byte dado;
    while (1) {
        sem_wait(&sem3);
     
        pipe_read(&msg1, &dado);
        if (dado == '0' || dado == '2'){
            PORTCbits.RC0 = 0;
            sem_post(&sem1);
        }
        sem_post(&sem3);
        //delay_os(100);
    }
}

void task_m2(){
    byte dado;
    while (1) {
        sem_wait(&sem3);
     
        pipe_read(&msg2, &dado);
        if (dado == '1' || dado == '2'){
            PORTCbits.RC1 = 0;
            sem_post(&sem1);
        }
        sem_post(&sem3);
        //delay_os(100);
    }
}




