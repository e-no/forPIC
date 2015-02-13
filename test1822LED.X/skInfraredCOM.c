/*******************************************************************************
*  skInfraredCOM - �ԊO���ʐM���s���֐����C�u�����[                            *
*                                                                              *
*    InitCCP        - CCP(PWM)�@�\�̐ݒ�����鏈��                             *
*    InfraredSend   - �ԊO���k�d�c�Ƀf�[�^�𑗐M���鏈��                       *
*    InfraredRecive - �ԊO����M���W���[�����f�[�^����M���鏈��             *
*                                                                              *
*    �����FCCP(PWM)�@�\���g�p���ĐԊO��LED�̔��U���g��37.9KHz������Ă��܂�*
* ============================================================================ *
*  PIC 12F1822                                                                 *
*  MPLAB IDE(V8.63)                                                            *
*  HI-TECH C Compiler for PIC10/12/16 MCUs Version 9.80 in Lite mode           *
*******************************************************************************/
#include <pic.h>
#include <htc.h>              // delay�p�ɕK�v
#include "skInfraredCOM.h"


#define ON_USEC    1690		// �f�[�^�F�P(LOW)�̒���
#define OFF_USEC   560		// �f�[�^�F�O(LOW)�̒���
#define STOP_USEC  1000		// �f�[�^�F�X�g�b�v(LOW)�̒���
#define READ_USEC  1000		// ���[�_����LOW�̒���
#define HIGH_USEC  560		// �f�[�^����HIGH�̒���

#define HIGH       1
#define LOW        0


/*******************************************************************************
*  InitCCP()                                                                   *
*    CCP(PWM)�@�\�̐ݒ�����鏈��                                              *
*                                                                              *
*    �V�X�e���N���b�N���g����32MHz��ݒ肷�鎖�ŁAPWM�̎���37.950Hz�𔭐�����B*
*******************************************************************************/
void InitCCP()
{
     CCP1CON = 0b00001100 ;	// PWM�@�\(�V���O��)���g�p����
     T2CON   = 0b00000000 ;	// TMR2�v���X�P�[���l���P�{�ɐݒ�
     CCPR1L  = 0 ;			// �f���[�e�B�l�͂O�ŏ�����
     CCPR1H  = 0 ;
     TMR2    = 0 ;			// �^�C�}�[�Q�J�E���^�[��������
     PR2     = 210 ;		// PWM�̎�����ݒ�i37.950Hz�Őݒ�j
     TMR2ON  = 1 ;			// TMR2(PWM)�X�^�[�g
}
////////////////////////////////////////////////////////////////////////////////
//  PalseHigh() - �g�h�f�g�̃p���X�����
//   CCP(PWM)�̃f���[�e�B�l��30%���^����37.9KHz��HIGH(ON)�p���X�����A
//   HIGH�p���X�I���Ńf���[�e�B�l��0%�Ƃ���B
////////////////////////////////////////////////////////////////////////////////
void PalseHigh(int cnt)
{
     int i ;

     CCPR1L = 400/4 ;    // 37.9KHz��ON�J�n
     for (i=0 ; i < cnt ; i++ ) {
          __delay_us(HIGH_USEC) ;
     }
     CCPR1L = 0 ;        // 37.9KHz�̏I��
}
/*******************************************************************************
*  InfraredSend(toDeviceNo,KeyCode)                                            *
*    �ԊO���k�d�c�Ƀf�[�^�𑗐M���鏈��                                        *
*    ���M����̃f�o�C�X�R�[�h�łQ�T�T���w�肷��Ɩ������ő���͎�M����        *
*                                                                              *
*    toDeviceNo : ���M����̃f�o�C�X�R�[�h(1-250:251-255�͗\��)                *
*    KeyCode    : ���M����L�[�R�[�h(1-255)                                    *
*******************************************************************************/
void InfraredSend(unsigned char toDeviceNo,unsigned char KeyCode)
{
	int i ;
	
     // ���[�_���𑗂�
     PalseHigh(9) ;		// ��5040us
     __delay_us(READ_USEC) ;
	// ���M����̃f�o�C�X�R�[�h�𑗂�
     for (i=0 ; i<8 ; i++) {
          PalseHigh(1) ;	// ��560us
          if ((toDeviceNo >> i) & 0x1) __delay_us(ON_USEC) ;
          else                         __delay_us(OFF_USEC) ;
     }
	// �L�[�f�[�^�P�𑗂�
     for (i=0 ; i<8 ; i++) {
          PalseHigh(1) ;
          if ((KeyCode >> i) & 0x01) __delay_us(ON_USEC) ;
          else                       __delay_us(OFF_USEC) ;
     }
	// �L�[�f�[�^�P�𔽓]���đ���
     for (i=0 ; i<8 ; i++) {
          PalseHigh(1) ;
          if ((KeyCode >> i) & 0x01) __delay_us(OFF_USEC) ;
          else                       __delay_us(ON_USEC) ;
     }
     // �X�g�b�v�f�[�^�𑗂�
     PalseHigh(1) ;
     __delay_us(STOP_USEC) ;
}
////////////////////////////////////////////////////////////////////////////////
//  DataCheck() - ��M�f�[�^�̃`�F�b�N���s��
//   ��M�̃f�o�C�X�R�[�h���Q�T�T�Ȃ�MyDeviceNo���`�F�b�N���Ȃ�
////////////////////////////////////////////////////////////////////////////////
int DataCheck(unsigned char MyDeviceNo,char *dt)
{
     int x1 , x2 , i ;

     // �f�o�C�X�R�[�h�̃`�F�b�N
     x1 = 0 ;
     for (i=0 ; i<8 ; i++) {
          if (*dt++ == 0x31) x1 = x1 | (1 << i) ;
     }
     if ((x1 != 255) && (x1 != MyDeviceNo)) return(0) ;// ��M�f�o�C�X�R�[�h�������̃R�[�h�ł͂Ȃ�
     // �L�[�f�[�^�P�̃`�F�b�N
     x1 = 0 ;
     for (i=0 ; i<8 ; i++) {
          if (*dt++ == 0x31) x1 = x1 | (1 << i) ;
     }
     // �L�[�f�[�^�Q�̃`�F�b�N(�L�[�f�[�^�P�̔��]�f�[�^)
     x2 = 0 ;
     for (i=0 ; i<8 ; i++) {
          if (*dt++ == 0x30) x2 = x2 | (1 << i) ;
     }
     if (x1 != x2) return(0) ;		// �L�[�f�[�^�̂P�ƂQ���قȂ���
     return(x1) ;					// �`�F�b�N����A�L�[�f�[�^��Ԃ�
}
/*******************************************************************************
*  ans = InfraredRecive(MyDeviceNo)                                            *
*    �ԊO����M���W���[�����f�[�^����M���鏈��                              *
*    ��M�����f�[�^�́A���M���̃f�[�^�����W���[�������]���ďo�͂���            *
*                                                                              *
*    MyDeviceNo : �����̃f�o�C�X�ԍ�(1-250)                                    *
*    ans        : ��M�f�[�^�������ւ��ް��Ȃ��M�L�[�R�[�h(1-255)��Ԃ�      *
*                 ��M�f�[�^�������ȊO�Ȃ�O��Ԃ�                             *
*******************************************************************************/
int InfraredRecive(unsigned char MyDeviceNo)
{
     char IRbit[26] ;			// ��M�o�b�t�@
     unsigned long t ;
     int ans , i ;

     ans = 0 ;
     t   = 0 ;
     // ���[�_���̃`�F�b�N���s��
     if (IRpin == LOW) {
                
          t = 0 ;
          while (IRpin == LOW) {	// HIGH(ON)�ɂȂ�܂ŌJ�Ԃ�
               __delay_us(10) ;	// OFF�̕������͂���
               t++ ;
              
          }
     }
     // ���[�_���L��Ȃ珈������
     if (t >= 300) {
         RA2 = 1;
          i = 0 ;
          while(IRpin == HIGH) ;	// �����܂ł����[�_��(ON����)�ǂݔ�΂�
          // �f�[�^���̓ǂݍ���
          while (1) {
               while(IRpin == LOW) ; 			// OFF�����͓ǂݔ�΂�
               t = 0 ;
               while(IRpin == HIGH) {			// ON�����̒������͂���
                    __delay_us(10) ;
                    t++ ;
               }
               if (t >= 50) IRbit[i] = (char)0x31 ;// ON����������
               else         IRbit[i] = (char)0x30 ;// ON�������Z��
               i++ ;
               if (i == 24) break ;			// �R�o�C�g�Ǎ��񂾂�I��
          }
          // �f�[�^�L��Ȃ�`�F�b�N���s��
          if (i == 24) {
               //ans = DataCheck(MyDeviceNo,IRbit) ;
          }
     }
     return( ans ) ;
}
