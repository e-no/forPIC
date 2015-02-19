/* 
 * File:   main.c
 * Author: User
 *
 * Created on 2015/02/18, 14:15
 */

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 8000000	//  delay用に必要(クロック8MHzを指定)

#define T0COUT     61   // タイマー０用カウントの初期値(256 - 195 = 61)

#define IR_IN RA0


#byte   RA = 0x0C
#bit    IRD = RA.0
#bit    LED = RA.1

#define     ON      1
#define     OFF     0
#define     DAT     40


int Count; // タイマーの割込み発生回数をカウントする変数
int LEDflg; // LEDのON/OFF状態フラグ


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

/*
 * 
 */
int main(int argc, char** argv) {

    long    bon,    boff,   icnt;

     OSCCON = 0b01110000; // 内部クロックは8ＭＨｚとする
/**
  bit 6-3 IRCF<3:0>: 内部オシレータ周波数選択ビット
            000x = 31 kHz LF
            0010 = 31.25 kHz MF
            0011 = 31.25 kHz HF(1)
            0100 = 62.5 kHz MF
            0101 = 125 kHz MF
            0110 = 250 kHz MF
            0111 = 500 kHz MF ( リセット時の既定値)
            1000 = 125 kHz HF(1)
            1001 = 250 kHz HF(1)
            1010 = 500 kHz HF(1)
            1011 = 1MHz HF
            1100 = 2MHz HF
            1101 = 4MHz HF
            1110 = 8MHz また

 */

    ANSELAbits.ANSELA = 0b00000000; // アナログは使用しない（すべてデジタルI/Oに割当てる）
    TRISA = 0b00011000; // ピンは全て出力に割当てる(RA3は入力専用)
    PORTA = 0b00000000; // 出力ピンの初期化(全てLOWにする)



    LEDflg = 0; // LEDのフラグ状態をOFFとする

    while(1){

        RA0 = 1;
        RA1 = 1;
        RA2 = 1;

        RA5 = 1;
        __delay_ms(500);


        RA0 = 0;
        RA1 = 0;
        RA2 = 0;

        RA5 = 0;
        
        __delay_ms(500);

    }//end while(1)
    return (EXIT_SUCCESS);
}

