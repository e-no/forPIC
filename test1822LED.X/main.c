/***********************************************
 * for PIC12F1822FN28
 * Infrared Ray sensor 
 *
 *
 * made:e-no
 ************************************************/

#include <xc.h>

#define _XTAL_FREQ 16000000	//  delay用に必要(クロック32MHzを指定)
#define T0COUT     61   // タイマー０用カウントの初期値(256 - 195 = 61)


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




// メインの処理

void main() {

    OSCCON = 0b11110010; // 内部クロックは8ＭＨｚとする
    ANSELAbits.ANSELA = 0b00000000; // アナログは使用しない（すべてデジタルI/Oに割当てる）
    TRISA = 0b0111000; // ピンは全て出力に割当てる(RA3は入力専用)
    PORTA = 0b00000000; // 出力ピンの初期化(全てLOWにする)


    while (1) {

        if (RA4 == 1) {
            RA2 = 0;
        } else if (RA4 == 0) {
            RA2 = 1;
        } else {
            RA2 = 1;
        }
    }

}

/*腹が減った?*/

/*
    while (RA4 == 0);

    while (1) {

        if (RA4 == 1) {
            
        } else if (RA4 == 0) {
            __delay_ms(1);

            if (RA4 == 0) {
                __delay_ms(1);

                if (RA4 == 0) {
                    RA2 = 1;
                }else{
                RA2 = 0;
                }
            }else{
                RA2 = 0;
            }
        }
    }
}
 */