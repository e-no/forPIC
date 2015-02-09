#include <stdio.h>
#include <string.h>
#include <xc.h>

#define T0COUT     61   // �^�C�}�[�O�p�J�E���g�̏����l(256 - 195 = 61)

int Count ;             // �^�C�}�[�̊����ݔ����񐔂��J�E���g����ϐ�
int LEDflg ;            // LED��ON/OFF��ԃt���O

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
// Flash��ذ��ی삵�Ȃ�(WRT_OFF)�F�d���d���~���펞�Ď��d��(2.5V)�ݒ�(BORV_25)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
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
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)



// �^�C�}�[�����݂̏���
void interrupt InterTimer( void )
{
     if (TMR0IF == 1) {           // �^�C�}�[0�̊����ݔ������H
          TMR0 = T0COUT ;         // �^�C�}�[0�̏�����
          Count++ ;               // �����ݔ����̉񐔂��J�E���g����
          TMR0IF = 0 ;            // �^�C�}�[0�����t���O�����Z�b�g
          if (Count >= 40) {      // �����݂�40��J�E���g����Ɩ�P�b
               Count = 0 ;
               // �P�b����LED�̃t���O��ON/OFF�����鏈��
               if (LEDflg == 0) LEDflg = 1 ;
               else             LEDflg = 0 ;
          }
     }
}
// ���C���̏���
void main()
{
     OSCCON = 0b01101010 ;     // �����N���b�N�͂S�l�g���Ƃ���
     ANSELA = 0b00000000 ;     // �A�i���O�͎g�p���Ȃ��i���ׂăf�W�^��I/O�Ɋ����Ă�j
     TRISA  = 0b00001000 ;     // �s���͑S�ďo�͂Ɋ����Ă�(RA3�͓��݂͂̂ƂȂ�)
     PORTA  = 0b00000000 ;     // �o�̓s���̏�����(�S��LOW�ɂ���)

     OPTION_REG = 0b00000110 ; // �����ۯ���TIMER0���g�p�A��ؽ��׶��Ēl 1:128
     TMR0   = T0COUT ;         // �^�C�}�[0�̏�����
     TMR0IF = 0 ;              // �^�C�}�[0�����t���O(T0IF)��0�ɂ���
     Count  = 0 ;              // �����ݔ����̉񐔃J�E���^�[��0�ɂ���
     TMR0IE = 1 ;              // �^�C�}�[0������(T0IE)��������
     GIE    = 1 ;              // �S�����ݏ�����������

     LEDflg = 0 ;              // LED�̃t���O��Ԃ�OFF�Ƃ���

     while(1) {
          // LED�̃t���O���ON/OFF�ɂ��LED��ON/OFF���鏈��
          if (LEDflg == 0) RA0 = 0 ;    // 5�ԃs����LOW���o�͂���(LED OFF)
          else             RA0 = 1 ;    // 5�ԃs����HIGH���o�͂���(LED ON)
     }
}