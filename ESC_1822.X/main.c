/***********************************************
 * for PIC12F1822FN28
 * Infrared Ray sensor
 *
 *
 * made:e-no
 ************************************************/

#include <xc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define _XTAL_FREQ 8000000	//  delay�p�ɕK�v(�N���b�N32MHz���w��)

#define T0COUT     61   // �^�C�}�[�O�p�J�E���g�̏����l(256 - 195 = 61)


#define HIGH       1
#define LOW        0

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

int Count; // �^�C�}�[�̊����ݔ����񐔂��J�E���g����ϐ�
int LEDflg;

unsigned char data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
const unsigned char sig[8] = {0, 0, 1, 0, 1, 0, 0, 0};

void strOutUSART(char *str);


// ���C���̏���

int main() {

    int cnt = 0;

    OSCCON = 0b01110010; // �����N���b�N��8�l�g���Ƃ���
    ANSELAbits.ANSELA = 0b00000000; // �A�i���O�͎g�p���Ȃ��i���ׂăf�W�^��I/O�Ɋ����Ă�j
    TRISA = 0b00001100; // �s���͑S�ďo�͂Ɋ����Ă�(RA3�͓��͐�p)
    PORTA = 0b00000000; // �o�̓s���̏�����(�S��LOW�ɂ���)

    // �t�r�`�q�s�@�\�̐ݒ���s��
    RXDTSEL = 1; // 2�ԃs��(RA5)���q�w��M�s���Ƃ���
    TXCKSEL = 1; // 3�ԃs��(RA4)���s�w���M�s���Ƃ���
    TXSTA = 0b00100100; // ���M���ݒ�F�񓯊����[�h�@�W�r�b�g�E�m���p���e�B
    RCSTA = 0b10010000; // ��M���ݒ�
    SPBRG = 51; // �{�[���[�g���X�U�O�O(�������[�h)�ɐݒ�

    OPTION_REG = 0b00000110; // �����ۯ���TIMER0���g�p�A��ؽ��׶��Ēl 1:128
    TMR0 = T0COUT; // �^�C�}�[0�̏�����
    TMR0IF = 0; // �^�C�}�[0�����t���O(T0IF)��0�ɂ���
    Count = 0; // �����ݔ����̉񐔃J�E���^�[��0�ɂ���
    TMR0IE = 1; // �^�C�}�[0������(T0IE)��������
    GIE = 1; // �S�����ݏ�����������

    __delay_ms(50); // 0.0�T�b��ɊJ�n����


    RA0 =0;

    while (1) {

        Count++;    //program counter

        while (RA2);    //wait IR OFF

        while (!RA2);   //wait IR ON

        if (RA2 == 1) {
            RA5 = 1;
            cnt++;
            __delay_us(600);
        } else {
            RA5 = 0;
        }

        if (RA2 == 0) {
            __delay_us(600);
            RA5 = 1;
            cnt++;
        } else {
            RA5 = 0;
        }

        if(Count >= 10){
            if(cnt >= 8){
                RA0 = 1;
                cnt = 0;
            }
        }else{
            RA0 = 0;
            cnt = 0;
        }

        // LED�̃t���O���ON/OFF�ɂ��LED��ON/OFF���鏈��
        if (LEDflg == 0) RA1 = 0; // 5�ԃs����LOW���o�͂���(LED OFF)
        else RA1 = 1; // 5�ԃs����HIGH���o�͂���(LED ON)
    }
    return (EXIT_SUCCESS);
}


void strOutUSART(char *str) {
    while (*str) { //������̏I���(00)�܂Ōp��
        while (!PIR1bits.TXIF); //���M�I���҂�
        TXREG = *str++; //�����o�͂��|�C���^�{�P
    }
}

void interrupt InterTimer(void) {
    if (TMR0IF == 1) { // �^�C�}�[0�̊����ݔ������H
        TMR0 = T0COUT; // �^�C�}�[0�̏�����
        Count++; // �����ݔ����̉񐔂��J�E���g����
        TMR0IF = 0; // �^�C�}�[0�����t���O�����Z�b�g
        if (Count >= 40) { // �����݂�40��J�E���g����Ɩ�P�b
            Count = 0;
            // �P�b����LED�̃t���O��ON/OFF�����鏈��
            if (LEDflg == 0) LEDflg = 1;
            else LEDflg = 0;
        }
    }
}

