/*******************************************************************************
*  Recive.c - 赤外線通信で送られてくるデータを受信する                         *
*                                                                              *
*    メモ：赤外線受信モジュールを接続するピン番号は"skInfraredCOM.h"で指定する *
* ============================================================================ *
*  VERSION DATE        BY                    CHANGE/COMMENT                    *
* ---------------------------------------------------------------------------- *
*  1.00    2011-07-09  きむ茶工房(きむしげ)  Create                            *
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
#include "skInfraredCOM.h"	// 赤外線通信用

#define _XTAL_FREQ 32000000	//  delay用に必要(クロック32MHzを指定)


// コンフィギュレーション１の設定
// CLKOUTﾋﾟﾝをRA4ﾋﾟﾝで使用する(CLKOUTEN_OFF)：内部ｸﾛｯｸ使用する(INTIO)
// 外部ｸﾛｯｸ監視しない(FCMEN_OFF)：外部・内部ｸﾛｯｸの切替えでの起動はなし(IESO_OFF)
// 電源電圧降下常時監視機能ON(BOREN_ON)：電源ONから64ms後にﾌﾟﾛｸﾞﾗﾑを開始する(PWRTEN_ON)
// ｳｵｯﾁﾄﾞｯｸﾞﾀｲﾏｰ無し(WDTE_OFF)：
// 外部ﾘｾｯﾄ信号は使用せずにﾃﾞｼﾞﾀﾙ入力(RA3)ﾋﾟﾝとする(MCLRE_OFF)
// ﾌﾟﾛｸﾞﾗﾑﾒﾓﾘｰを保護しない(CP_OFF)：ﾃﾞｰﾀﾒﾓﾘｰを保護しない(CPD_OFF)

#pragma config FOSC = INTOSCCLK // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = ON        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)


// コンフィギュレーション２の設定
// 動作クロックを32MHzで動作させる(PLLEN_ON)
// スタックがオーバフローやアンダーフローしたらリセットをする(STVREN_ON)
// 低電圧プログラミング機能使用しない(LVP_OFF)
// Flashﾒﾓﾘｰを保護しない(WRT_OFF)：電源電圧降下常時監視電圧(2.5V)設定(BORV_25)
///__CONFIG(PLLEN_ON & STVREN_ON & WRT_OFF & BORV_25 & LVP_OFF);
static	unsigned	int		timeCnt, cycleTime;
static	unsigned	short	startFlag;




/*******************************************************************************
*  メインの処理                                                                *
*******************************************************************************/
void main()
{
	char s[6] ;
	int ans ;
	
     OSCCON   = 0b01110000 ;  // 内部クロックは 8ＭＨｚ x 4 とする
     ANSEL      = 0b00000000 ;  // アナログは使用しない、すべてデジタルI/Oに割当
     TRISIO     = 0b00011000 ;  // RA4は入力、ピンはすべて出力に割当てる(RA3は入力専用)
     GPIO        = 0b00000000 ;  // 出力ピンの初期化(全てLOWにする)



     while(1) {
          // マイデバイスNoは10番でデータを受信する、
          // それ以外のNoで来たらデータを捨てる、でもNo=255[一斉受信]なら受信
          ans = InfraredRecive(10) ;
          if (ans != 0) {
               // このプログラム例では、受信したデータ(通知情報)は
               // デバッグモニタに出力して表示させています。
          }
     }
}
