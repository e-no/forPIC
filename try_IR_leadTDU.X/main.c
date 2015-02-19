/* 
 * File:   main.c
 * Author: User
 *
 * Created on 2015/02/18, 14:15
 */

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 8000000	//  delay�p�ɕK�v(�N���b�N8MHz���w��)

#define T0COUT     61   // �^�C�}�[�O�p�J�E���g�̏����l(256 - 195 = 61)

#define IR_IN RA0


#byte   RA = 0x0C
#bit    IRD = RA.0
#bit    LED = RA.1

#define     ON      1
#define     OFF     0
#define     DAT     40


int Count; // �^�C�}�[�̊����ݔ����񐔂��J�E���g����ϐ�
int LEDflg; // LED��ON/OFF��ԃt���O


// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = HI        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), high trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

/*
 * 
 */
int main(int argc, char** argv) {

    long    bon,    boff,   icnt;

     OSCCON = 0b01110000; // �����N���b�N��8�l�g���Ƃ���
/**
  bit 6-3 IRCF<3:0>: �����I�V���[�^���g���I���r�b�g
            000x = 31 kHz LF
            0010 = 31.25 kHz MF
            0011 = 31.25 kHz HF(1)
            0100 = 62.5 kHz MF
            0101 = 125 kHz MF
            0110 = 250 kHz MF
            0111 = 500 kHz MF ( ���Z�b�g���̊���l)
            1000 = 125 kHz HF(1)
            1001 = 250 kHz HF(1)
            1010 = 500 kHz HF(1)
            1011 = 1MHz HF
            1100 = 2MHz HF
            1101 = 4MHz HF
            1110 = 8MHz �܂�

 */

    ANSELAbits.ANSELA = 0b00000000; // �A�i���O�͎g�p���Ȃ��i���ׂăf�W�^��I/O�Ɋ����Ă�j
    TRISA = 0b00011000; // �s���͑S�ďo�͂Ɋ����Ă�(RA3�͓��͐�p)
    PORTA = 0b00000000; // �o�̓s���̏�����(�S��LOW�ɂ���)



    LEDflg = 0; // LED�̃t���O��Ԃ�OFF�Ƃ���

    while(1){

        RA0 = 1;
        RA1 = 1;
        RA2 = 1;

        RA5 = 1;
        __delay_ms(500);


        RA0 = 0;
        RA1 = 0;
        RA2 = 0;

        RA5 = 0;
        
        __delay_ms(500);

    }//end while(1)
    return (EXIT_SUCCESS);
}

