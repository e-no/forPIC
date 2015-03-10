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

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)




// メインの処理

void main() {

    OSCCON = 0b11110010; // 内部クロックは8ＭＨｚとする
    ANSEL = 0b00000000; // アナログは使用しない（すべてデジタルI/Oに割当てる）
    TRISIO = 0b00011000; // ピンは全て出力に割当てる(RA3は入力専用)
    GPIO = 0b00000000; // 出力ピンの初期化(全てLOWにする)


       while (1) {

        if (GP4 == 1) {
            GP2 = 0;
        } else if (GP4 == 0) {
            GP2 = 1;
        } else {
            GP2 = 1;
        }
    }

    
}
