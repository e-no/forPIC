#ifndef _SKINFRAREDCOM_H_
#define _SKINFRAREDCOM_H_

#ifndef _XTAL_FREQ
 // Unless already defined assume 32MHz system frequency
 // This definition is required to calibrate __delay_us() and __delay_ms()
 #define _XTAL_FREQ 32000000	//  delay用に必要(クロック32MHzを指定)
#endif

#define IRpin      RA0		// 赤外線受信モジュールの接続ピンをここで指定する


void InitCCP() ;
void InfraredSend(unsigned char toDeviceNo,unsigned char KeyCode) ;
int  InfraredRecive(unsigned char MyDeviceNo) ;
int  RemoconSW() ;

#endif
