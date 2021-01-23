#ifndef ADC_H_
#define ADC_H_

// =============================================== //
// BIBLIOTECA PARA USO DE ADC NO PIC 18F4550       //
//                                                 //
// ESCOLA SENAI "A. JACOB LAFER                    //
// CURSO T�CNICO DE ELETROELETR�NICA               //
// DISCIPLINA: DESET                               //
// DATA: 19/09/2016                                //
// REVIS�O: A                                      //
// =============================================== //

// -----------------------------------------------------
// Rotina de inicializa��o do m�dulo ADC
// -----------------------------------------------------
void Inicializa_ADC(void){
 TRISAbits.TRISA0 = 1;    // Configura RA0 (AN0) como entrada
 ADCON0 = 0;           // Configura refer�ncia de tens�o (Vdd e Vss) e habilita AN0 (entrada do ADC)
 ADCON1 = 0x0E;        // Configura o pino como entrada anal�gica (apenas o pino AN0)
 ADCON2 = 0xD9;        // Configura o tempo de aquisi��o (vide datasheet)
 ADCON0bits.ADON = 1;  // Liga m�dulo ADC
}
// -----------------------------------------------------

// -----------------------------------------------------
// Rotina de leitura do ADC
// -----------------------------------------------------
unsigned int LerADC(void){
 unsigned int temp;

ADCON0bits.GO = 1;               // Inicia aquisi��o
 while(ADCON0bits.DONE);         // Aguarda o fim da aquisi��o (quando bit DONE = 0)
 temp = ADRESH;                  // Parte alta do valor lido
 temp = ((temp << 8)| ADRESL );  // Junta parte alta com parte baixa em um valor de 16 bits

return temp;
}
// -----------------------------------------------------
#endif