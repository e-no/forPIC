/* 
 * File:   main.c
 * Author: User
 *
 * Created on 2015/02/10, 19:36
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <xc.h>

#define _XTAL_FREQ  8000000    // delay�p(�N���b�N�WMHz�œ��쎞)

// �R���t�B�M�����[�V�����P�̐ݒ�
// CLKOUT��݂�RA4��݂Ŏg�p����(CLKOUTEN_OFF)�F�����ۯ��g�p����(INTIO)
// �O���ۯ��Ď����Ȃ�(FCMEN_OFF)�F�O���E�����ۯ��̐ؑւ��ł̋N���͂Ȃ�(IESO_OFF)
// �d���d���~���펞�Ď��@�\ON(BOREN_ON)�F�d��ON����64ms�����۸��т��J�n����(PWRTEN_ON)
// �����ޯ����ϰ����(WDTE_OFF)�F
// �O��ؾ�ĐM���͎g�p�������޼��ٓ���(RA3)��݂Ƃ���(MCLRE_OFF)
// ��۸�����ذ��ی삵�Ȃ�(CP_OFF)�F�ް���ذ��ی삵�Ȃ�(CPD_OFF)

// �R���t�B�M�����[�V�����Q�̐ݒ�
// ����N���b�N��32MHz�ł͓��삳���Ȃ�(PLLEN_OFF)
// �X�^�b�N���I�[�o�t���[��A���_�[�t���[�����烊�Z�b�g������(STVREN_ON)
// ��d���v���O���~���O�@�\�g�p���Ȃ�(LVP_OFF)
// Flash��ذ��ی삵�Ȃ�(WRT_OFF)�F�d���d���~���펞�Ď��d��(2.5V)�ݒ�(BORV_HI)


// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

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

void Wait(unsigned int num);
void strOutUSART(char *str);


// ���C���̏���
void main()
{
     int i;
     char str[];

     OSCCON = 0b01110010 ;     // �����N���b�N��8�l�g���Ƃ���
     ANSELA = 0b00000000 ;     // �A�i���O�͎g�p���Ȃ��i���ׂăf�W�^��I/O�Ɋ����Ă�j
     TRISA  = 0b00001000 ;     // �s���͑S�ďo�͂Ɋ����Ă�(RA3�͓��͐�p)
     PORTA  = 0b00000000 ;     // �o�̓s���̏�����(�S��LOW�ɂ���)
     // �t�r�`�q�s�@�\�̐ݒ���s��
     RXDTSEL = 1 ;             // 2�ԃs��(RA5)���q�w��M�s���Ƃ���
     TXCKSEL = 1 ;             // 3�ԃs��(RA4)���s�w���M�s���Ƃ���
     TXSTA  = 0b00100100 ;     // ���M���ݒ�F�񓯊����[�h�@�W�r�b�g�E�m���p���e�B
     RCSTA  = 0b10010000 ;     // ��M���ݒ�
     SPBRG  = 51 ;             // �{�[���[�g���X�U�O�O(�������[�h)�ɐݒ�

     Wait(500) ;               // �T�b��ɊJ�n����
     i = 0x30 ;

     while(1) {
          while(TXIF==0) ;     // ���M�\�ɂȂ�܂ő҂�    *1)

          i = 2099;

          sprintf(str,"value: %d \n\r",i);

          strOutUSART(str);
          Wait(100) ;          // �P�b��ɏ������J��Ԃ�
     }
}

// �w�肵������(num x 10ms)�����E�G�C�g���s�������֐�
void Wait(unsigned int num)
{
     int i ;

     // num�Ŏw�肵���񐔂����J��Ԃ�
     for (i=0 ; i < num ; i++) {
          __delay_ms(10) ;     // 10ms�v���O�����̈ꎞ��~
     }
}

void strOutUSART(char *str){
    while(*str){                 //������̏I���(00)�܂Ōp��
        while (!PIR1bits.TXIF);  //���M�I���҂�
        TXREG = *str++;          //�����o�͂��|�C���^�{�P
    }
}
