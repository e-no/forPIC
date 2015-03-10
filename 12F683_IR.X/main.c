/***********************************************
 * for PIC12F1822FN28
 * Infrared Ray sensor
 *
 *
 * made:e-no
 ************************************************/

#include <xc.h>


#define _XTAL_FREQ 16000000	//  delay�p�ɕK�v(�N���b�N32MHz���w��)

#define T0COUT     61   // �^�C�}�[�O�p�J�E���g�̏����l(256 - 195 = 61)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)




// ���C���̏���

void main() {

    OSCCON = 0b11110010; // �����N���b�N��8�l�g���Ƃ���
    ANSEL = 0b00000000; // �A�i���O�͎g�p���Ȃ��i���ׂăf�W�^��I/O�Ɋ����Ă�j
    TRISIO = 0b00011000; // �s���͑S�ďo�͂Ɋ����Ă�(RA3�͓��͐�p)
    GPIO = 0b00000000; // �o�̓s���̏�����(�S��LOW�ɂ���)


       while (1) {

        if (GP4 == 1) {
            GP2 = 0;
        } else if (GP4 == 0) {
            GP2 = 1;
        } else {
            GP2 = 1;
        }
    }

    
}
