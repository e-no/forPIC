#ifndef _SKINFRAREDCOM_H_
#define _SKINFRAREDCOM_H_

#ifndef _XTAL_FREQ
 // Unless already defined assume 32MHz system frequency
 // This definition is required to calibrate __delay_us() and __delay_ms()
 #define _XTAL_FREQ 32000000	//  delay�p�ɕK�v(�N���b�N32MHz���w��)
#endif

#define IRpin      RA0		// �ԊO����M���W���[���̐ڑ��s���������Ŏw�肷��


void InitCCP() ;
void InfraredSend(unsigned char toDeviceNo,unsigned char KeyCode) ;
int  InfraredRecive(unsigned char MyDeviceNo) ;
int  RemoconSW() ;

#endif
