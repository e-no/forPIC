/*******************************************************************************
*  Recive.c - �ԊO���ʐM�ő����Ă���f�[�^����M����                         *
*                                                                              *
*    �����F�ԊO����M���W���[����ڑ�����s���ԍ���"skInfraredCOM.h"�Ŏw�肷�� *
* ============================================================================ *
*  VERSION DATE        BY                    CHANGE/COMMENT                    *
* ---------------------------------------------------------------------------- *
*  1.00    2011-07-09  ���ޒ��H�[(���ނ���)  Create                            *
* ============================================================================ *
*  PIC 12F1822                                                                 *
*  MPLAB IDE(V8.63)                                                            *
*  HI-TECH C Compiler for PIC10/12/16 MCUs Version 9.80 in Lite mode           *
*******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <xc.h>
#include <usart.h>
#include "skInfraredCOM.h"	// �ԊO���ʐM�p

#define _XTAL_FREQ 32000000	//  delay�p�ɕK�v(�N���b�N32MHz���w��)


// �R���t�B�M�����[�V�����P�̐ݒ�
// CLKOUT��݂�RA4��݂Ŏg�p����(CLKOUTEN_OFF)�F�����ۯ��g�p����(INTIO)
// �O���ۯ��Ď����Ȃ�(FCMEN_OFF)�F�O���E�����ۯ��̐ؑւ��ł̋N���͂Ȃ�(IESO_OFF)
// �d���d���~���펞�Ď��@�\ON(BOREN_ON)�F�d��ON����64ms�����۸��т��J�n����(PWRTEN_ON)
// �����ޯ����ϰ����(WDTE_OFF)�F
// �O��ؾ�ĐM���͎g�p�������޼��ٓ���(RA3)��݂Ƃ���(MCLRE_OFF)
// ��۸�����ذ��ی삵�Ȃ�(CP_OFF)�F�ް���ذ��ی삵�Ȃ�(CPD_OFF)

#pragma config FOSC = INTOSCCLK // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = ON        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)


// �R���t�B�M�����[�V�����Q�̐ݒ�
// ����N���b�N��32MHz�œ��삳����(PLLEN_ON)
// �X�^�b�N���I�[�o�t���[��A���_�[�t���[�����烊�Z�b�g������(STVREN_ON)
// ��d���v���O���~���O�@�\�g�p���Ȃ�(LVP_OFF)
// Flash��ذ��ی삵�Ȃ�(WRT_OFF)�F�d���d���~���펞�Ď��d��(2.5V)�ݒ�(BORV_25)
///__CONFIG(PLLEN_ON & STVREN_ON & WRT_OFF & BORV_25 & LVP_OFF);
static	unsigned	int		timeCnt, cycleTime;
static	unsigned	short	startFlag;




/*******************************************************************************
*  ���C���̏���                                                                *
*******************************************************************************/
void main()
{
	char s[6] ;
	int ans ;
	
     OSCCON   = 0b01110000 ;  // �����N���b�N�� 8�l�g�� x 4 �Ƃ���
     ANSEL      = 0b00000000 ;  // �A�i���O�͎g�p���Ȃ��A���ׂăf�W�^��I/O�Ɋ���
     TRISIO     = 0b00011000 ;  // RA4�͓��́A�s���͂��ׂďo�͂Ɋ����Ă�(RA3�͓��͐�p)
     GPIO        = 0b00000000 ;  // �o�̓s���̏�����(�S��LOW�ɂ���)



     while(1) {
          // �}�C�f�o�C�XNo��10�ԂŃf�[�^����M����A
          // ����ȊO��No�ŗ�����f�[�^���̂Ă�A�ł�No=255[��Ď�M]�Ȃ��M
          ans = InfraredRecive(10) ;
          if (ans != 0) {
               // ���̃v���O������ł́A��M�����f�[�^(�ʒm���)��
               // �f�o�b�O���j�^�ɏo�͂��ĕ\�������Ă��܂��B
          }
     }
}
