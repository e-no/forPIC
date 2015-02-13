/* 
 * File:   main.c
 * Author: User
 *
 * Created on 2015/02/10, 19:36
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <xc.h>

#define _XTAL_FREQ  8000000    // delay用(クロック８MHzで動作時)

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
// Flashﾒﾓﾘｰを保護しない(WRT_OFF)：電源電圧降下常時監視電圧(2.5V)設定(BORV_HI)


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

void Wait(unsigned int num);
void strOutUSART(char *str);


// メインの処理
void main()
{
     int i;
     char str[];

     OSCCON = 0b01110010 ;     // 内部クロックは8ＭＨｚとする
     ANSELA = 0b00000000 ;     // アナログは使用しない（すべてデジタルI/Oに割当てる）
     TRISA  = 0b00001000 ;     // ピンは全て出力に割当てる(RA3は入力専用)
     PORTA  = 0b00000000 ;     // 出力ピンの初期化(全てLOWにする)
     // ＵＳＡＲＴ機能の設定を行う
     RXDTSEL = 1 ;             // 2番ピン(RA5)をＲＸ受信ピンとする
     TXCKSEL = 1 ;             // 3番ピン(RA4)をＴＸ送信ピンとする
     TXSTA  = 0b00100100 ;     // 送信情報設定：非同期モード　８ビット・ノンパリティ
     RCSTA  = 0b10010000 ;     // 受信情報設定
     SPBRG  = 51 ;             // ボーレートを９６００(高速モード)に設定

     Wait(500) ;               // ５秒後に開始する
     i = 0x30 ;

     while(1) {
          while(TXIF==0) ;     // 送信可能になるまで待つ    *1)

          i = 2099;

          sprintf(str,"value: %d \n\r",i);

          strOutUSART(str);
          Wait(100) ;          // １秒後に処理を繰り返す
     }
}

// 指定した時間(num x 10ms)だけウエイトを行う処理関数
void Wait(unsigned int num)
{
     int i ;

     // numで指定した回数だけ繰り返す
     for (i=0 ; i < num ; i++) {
          __delay_ms(10) ;     // 10msプログラムの一時停止
     }
}

void strOutUSART(char *str){
    while(*str){                 //文字列の終わり(00)まで継続
        while (!PIR1bits.TXIF);  //送信終了待ち
        TXREG = *str++;          //文字出力しポインタ＋１
    }
}
