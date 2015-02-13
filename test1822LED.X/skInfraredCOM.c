/*******************************************************************************
*  skInfraredCOM - 赤外線通信を行う関数ライブラリー                            *
*                                                                              *
*    InitCCP        - CCP(PWM)機能の設定をする処理                             *
*    InfraredSend   - 赤外線ＬＥＤにデータを送信する処理                       *
*    InfraredRecive - 赤外線受信モジュールよりデータを受信する処理             *
*                                                                              *
*    メモ：CCP(PWM)機能を使用して赤外線LEDの発振周波数37.9KHzを作っています*
* ============================================================================ *
*  PIC 12F1822                                                                 *
*  MPLAB IDE(V8.63)                                                            *
*  HI-TECH C Compiler for PIC10/12/16 MCUs Version 9.80 in Lite mode           *
*******************************************************************************/
#include <pic.h>
#include <htc.h>              // delay用に必要
#include "skInfraredCOM.h"


#define ON_USEC    1690		// データ：１(LOW)の長さ
#define OFF_USEC   560		// データ：０(LOW)の長さ
#define STOP_USEC  1000		// データ：ストップ(LOW)の長さ
#define READ_USEC  1000		// リーダ部のLOWの長さ
#define HIGH_USEC  560		// データ部のHIGHの長さ

#define HIGH       1
#define LOW        0


/*******************************************************************************
*  InitCCP()                                                                   *
*    CCP(PWM)機能の設定をする処理                                              *
*                                                                              *
*    システムクロック周波数は32MHzを設定する事で、PWMの周期37.950Hzを発生する。*
*******************************************************************************/
void InitCCP()
{
     CCP1CON = 0b00001100 ;	// PWM機能(シングル)を使用する
     T2CON   = 0b00000000 ;	// TMR2プリスケーラ値を１倍に設定
     CCPR1L  = 0 ;			// デューティ値は０で初期化
     CCPR1H  = 0 ;
     TMR2    = 0 ;			// タイマー２カウンターを初期化
     PR2     = 210 ;		// PWMの周期を設定（37.950Hzで設定）
     TMR2ON  = 1 ;			// TMR2(PWM)スタート
}
////////////////////////////////////////////////////////////////////////////////
//  PalseHigh() - ＨＩＧＨのパルスを作る
//   CCP(PWM)のデューティ値を30%程与えて37.9KHzのHIGH(ON)パルスを作り、
//   HIGHパルス終了でデューティ値を0%とする。
////////////////////////////////////////////////////////////////////////////////
void PalseHigh(int cnt)
{
     int i ;

     CCPR1L = 400/4 ;    // 37.9KHzのON開始
     for (i=0 ; i < cnt ; i++ ) {
          __delay_us(HIGH_USEC) ;
     }
     CCPR1L = 0 ;        // 37.9KHzの終了
}
/*******************************************************************************
*  InfraredSend(toDeviceNo,KeyCode)                                            *
*    赤外線ＬＥＤにデータを送信する処理                                        *
*    送信相手のデバイスコードで２５５を指定すると無条件で相手は受信する        *
*                                                                              *
*    toDeviceNo : 送信相手のデバイスコード(1-250:251-255は予約)                *
*    KeyCode    : 送信するキーコード(1-255)                                    *
*******************************************************************************/
void InfraredSend(unsigned char toDeviceNo,unsigned char KeyCode)
{
	int i ;
	
     // リーダ部を送る
     PalseHigh(9) ;		// 約5040us
     __delay_us(READ_USEC) ;
	// 送信相手のデバイスコードを送る
     for (i=0 ; i<8 ; i++) {
          PalseHigh(1) ;	// 約560us
          if ((toDeviceNo >> i) & 0x1) __delay_us(ON_USEC) ;
          else                         __delay_us(OFF_USEC) ;
     }
	// キーデータ１を送る
     for (i=0 ; i<8 ; i++) {
          PalseHigh(1) ;
          if ((KeyCode >> i) & 0x01) __delay_us(ON_USEC) ;
          else                       __delay_us(OFF_USEC) ;
     }
	// キーデータ１を反転して送る
     for (i=0 ; i<8 ; i++) {
          PalseHigh(1) ;
          if ((KeyCode >> i) & 0x01) __delay_us(OFF_USEC) ;
          else                       __delay_us(ON_USEC) ;
     }
     // ストップデータを送る
     PalseHigh(1) ;
     __delay_us(STOP_USEC) ;
}
////////////////////////////////////////////////////////////////////////////////
//  DataCheck() - 受信データのチェックを行う
//   受信のデバイスコードが２５５ならMyDeviceNoをチェックしない
////////////////////////////////////////////////////////////////////////////////
int DataCheck(unsigned char MyDeviceNo,char *dt)
{
     int x1 , x2 , i ;

     // デバイスコードのチェック
     x1 = 0 ;
     for (i=0 ; i<8 ; i++) {
          if (*dt++ == 0x31) x1 = x1 | (1 << i) ;
     }
     if ((x1 != 255) && (x1 != MyDeviceNo)) return(0) ;// 受信デバイスコードが自分のコードではない
     // キーデータ１のチェック
     x1 = 0 ;
     for (i=0 ; i<8 ; i++) {
          if (*dt++ == 0x31) x1 = x1 | (1 << i) ;
     }
     // キーデータ２のチェック(キーデータ１の反転データ)
     x2 = 0 ;
     for (i=0 ; i<8 ; i++) {
          if (*dt++ == 0x30) x2 = x2 | (1 << i) ;
     }
     if (x1 != x2) return(0) ;		// キーデータの１と２が異なった
     return(x1) ;					// チェック正常、キーデータを返す
}
/*******************************************************************************
*  ans = InfraredRecive(MyDeviceNo)                                            *
*    赤外線受信モジュールよりデータを受信する処理                              *
*    受信したデータは、送信時のデータをモジュールが反転して出力する            *
*                                                                              *
*    MyDeviceNo : 自分のデバイス番号(1-250)                                    *
*    ans        : 受信データが自分へのﾃﾞｰﾀなら受信キーコード(1-255)を返す      *
*                 受信データが自分以外なら０を返す                             *
*******************************************************************************/
int InfraredRecive(unsigned char MyDeviceNo)
{
     char IRbit[26] ;			// 受信バッファ
     unsigned long t ;
     int ans , i ;

     ans = 0 ;
     t   = 0 ;
     // リーダ部のチェックを行う
     if (IRpin == LOW) {
                
          t = 0 ;
          while (IRpin == LOW) {	// HIGH(ON)になるまで繰返す
               __delay_us(10) ;	// OFFの部分をはかる
               t++ ;
              
          }
     }
     // リーダ部有りなら処理する
     if (t >= 300) {
         RA2 = 1;
          i = 0 ;
          while(IRpin == HIGH) ;	// ここまでがリーダ部(ON部分)読み飛ばす
          // データ部の読み込み
          while (1) {
               while(IRpin == LOW) ; 			// OFF部分は読み飛ばす
               t = 0 ;
               while(IRpin == HIGH) {			// ON部分の長さをはかる
                    __delay_us(10) ;
                    t++ ;
               }
               if (t >= 50) IRbit[i] = (char)0x31 ;// ON部分が長い
               else         IRbit[i] = (char)0x30 ;// ON部分が短い
               i++ ;
               if (i == 24) break ;			// ３バイト読込んだら終了
          }
          // データ有りならチェックを行う
          if (i == 24) {
               //ans = DataCheck(MyDeviceNo,IRbit) ;
          }
     }
     return( ans ) ;
}
