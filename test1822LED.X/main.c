#include <stdio.h>
#include <string.h>
#include <xc.h>

#define T0COUT     61   // タイマー０用カウントの初期値(256 - 195 = 61)

int Count ;             // タイマーの割込み発生回数をカウントする変数
int LEDflg ;            // LEDのON/OFF状態フラグ

// コンフィギュレーション１の設定
// CLKOUTﾋﾟﾝをRA4ﾋﾟﾝで使用する(CLKOUTEN_OFF)：内部ｸﾛｯｸ使用する(INTIO)
// 外部ｸﾛｯｸ監視しない(FCMEN_OFF)：外部・内部ｸﾛｯｸの切替えでの起動はなし(IESO_OFF)
// 電源電圧降下常時監視機能ON(BOREN_ON)：電源ONから64ms後にﾌﾟﾛｸﾞﾗﾑを開始する(PWRTEN_ON)
// ｳｵｯﾁﾄﾞｯｸﾞﾀｲﾏｰ無し(WDTE_OFF)：
// 外部ﾘｾｯﾄ信号は使用せずにﾃﾞｼﾞﾀﾙ入力(RA3)ﾋﾟﾝとする(MCLRE_OFF)
// ﾌﾟﾛｸﾞﾗﾑﾒﾓﾘｰを保護しない(CP_OFF)：ﾃﾞｰﾀﾒﾓﾘｰを保護しない(CPD_OFF)
// コンフィギュレーション２の設定
// 動作クロックを32MHzでは動作させない(PLLEN_OFF)
// スタックがオーバフローやアンダーフローしたらリセットをする(STVREN_ON)
// 低電圧プログラミング機能使用しない(LVP_OFF)
// Flashﾒﾓﾘｰを保護しない(WRT_OFF)：電源電圧降下常時監視電圧(2.5V)設定(BORV_25)

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



// タイマー割込みの処理
void interrupt InterTimer( void )
{
     if (TMR0IF == 1) {           // タイマー0の割込み発生か？
          TMR0 = T0COUT ;         // タイマー0の初期化
          Count++ ;               // 割込み発生の回数をカウントする
          TMR0IF = 0 ;            // タイマー0割込フラグをリセット
          if (Count >= 40) {      // 割込みを40回カウントすると約１秒
               Count = 0 ;
               // １秒毎にLEDのフラグをON/OFFさせる処理
               if (LEDflg == 0) LEDflg = 1 ;
               else             LEDflg = 0 ;
          }
     }
}
// メインの処理
void main()
{
     OSCCON = 0b01101010 ;     // 内部クロックは４ＭＨｚとする
     ANSELA = 0b00000000 ;     // アナログは使用しない（すべてデジタルI/Oに割当てる）
     TRISA  = 0b00001000 ;     // ピンは全て出力に割当てる(RA3は入力のみとなる)
     PORTA  = 0b00000000 ;     // 出力ピンの初期化(全てLOWにする)

     OPTION_REG = 0b00000110 ; // 内部ｸﾛｯｸでTIMER0を使用、ﾌﾟﾘｽｹｰﾗｶｳﾝﾄ値 1:128
     TMR0   = T0COUT ;         // タイマー0の初期化
     TMR0IF = 0 ;              // タイマー0割込フラグ(T0IF)を0にする
     Count  = 0 ;              // 割込み発生の回数カウンターを0にする
     TMR0IE = 1 ;              // タイマー0割込み(T0IE)を許可する
     GIE    = 1 ;              // 全割込み処理を許可する

     LEDflg = 0 ;              // LEDのフラグ状態をOFFとする

     while(1) {
          // LEDのフラグ状態ON/OFFによりLEDをON/OFFする処理
          if (LEDflg == 0) RA0 = 0 ;    // 5番ピンにLOWを出力する(LED OFF)
          else             RA0 = 1 ;    // 5番ピンにHIGHを出力する(LED ON)
     }
}