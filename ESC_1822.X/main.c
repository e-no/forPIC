/***********************************************
 * for PIC12F1822FN28
 * Infrared Ray sensor
 *
 *
 * made:e-no
 ************************************************/

#include <xc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define _XTAL_FREQ 8000000	//  delay用に必要(クロック32MHzを指定)

#define T0COUT     61   // タイマー０用カウントの初期値(256 - 195 = 61)


#define HIGH       1
#define LOW        0

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

int Count; // タイマーの割込み発生回数をカウントする変数
int LEDflg;

unsigned char data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
const unsigned char sig[8] = {0, 0, 1, 0, 1, 0, 0, 0};

void strOutUSART(char *str);


// メインの処理

int main() {

    int cnt = 0;

    OSCCON = 0b01110010; // 内部クロックは8ＭＨｚとする
    ANSELAbits.ANSELA = 0b00000000; // アナログは使用しない（すべてデジタルI/Oに割当てる）
    TRISA = 0b00001100; // ピンは全て出力に割当てる(RA3は入力専用)
    PORTA = 0b00000000; // 出力ピンの初期化(全てLOWにする)

    // ＵＳＡＲＴ機能の設定を行う
    RXDTSEL = 1; // 2番ピン(RA5)をＲＸ受信ピンとする
    TXCKSEL = 1; // 3番ピン(RA4)をＴＸ送信ピンとする
    TXSTA = 0b00100100; // 送信情報設定：非同期モード　８ビット・ノンパリティ
    RCSTA = 0b10010000; // 受信情報設定
    SPBRG = 51; // ボーレートを９６００(高速モード)に設定

    OPTION_REG = 0b00000110; // 内部ｸﾛｯｸでTIMER0を使用、ﾌﾟﾘｽｹｰﾗｶｳﾝﾄ値 1:128
    TMR0 = T0COUT; // タイマー0の初期化
    TMR0IF = 0; // タイマー0割込フラグ(T0IF)を0にする
    Count = 0; // 割込み発生の回数カウンターを0にする
    TMR0IE = 1; // タイマー0割込み(T0IE)を許可する
    GIE = 1; // 全割込み処理を許可する

    __delay_ms(50); // 0.0５秒後に開始する


    RA0 =0;

    while (1) {

        Count++;    //program counter

        while (RA2);    //wait IR OFF

        while (!RA2);   //wait IR ON

        if (RA2 == 1) {
            RA5 = 1;
            cnt++;
            __delay_us(600);
        } else {
            RA5 = 0;
        }

        if (RA2 == 0) {
            __delay_us(600);
            RA5 = 1;
            cnt++;
        } else {
            RA5 = 0;
        }

        if(Count >= 10){
            if(cnt >= 8){
                RA0 = 1;
                cnt = 0;
            }
        }else{
            RA0 = 0;
            cnt = 0;
        }

        // LEDのフラグ状態ON/OFFによりLEDをON/OFFする処理
        if (LEDflg == 0) RA1 = 0; // 5番ピンにLOWを出力する(LED OFF)
        else RA1 = 1; // 5番ピンにHIGHを出力する(LED ON)
    }
    return (EXIT_SUCCESS);
}


void strOutUSART(char *str) {
    while (*str) { //文字列の終わり(00)まで継続
        while (!PIR1bits.TXIF); //送信終了待ち
        TXREG = *str++; //文字出力しポインタ＋１
    }
}

void interrupt InterTimer(void) {
    if (TMR0IF == 1) { // タイマー0の割込み発生か？
        TMR0 = T0COUT; // タイマー0の初期化
        Count++; // 割込み発生の回数をカウントする
        TMR0IF = 0; // タイマー0割込フラグをリセット
        if (Count >= 40) { // 割込みを40回カウントすると約１秒
            Count = 0;
            // １秒毎にLEDのフラグをON/OFFさせる処理
            if (LEDflg == 0) LEDflg = 1;
            else LEDflg = 0;
        }
    }
}

