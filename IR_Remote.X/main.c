/**************************************************************
        12F1822 NEW IR_Remocon Control
        File-Name IR_Remocon_V32.c
        Gucci-Tech C. H
        MPLAB 8.60 /	Hitech PICC Pro Lite mode v9.80

        Digikey Remocon
        2011/5/13 PM11:21 Coting Start

        Pin assignment:
        (1)Vdd +3.0V
        (2)RA5 'IN'  [CCP1] IR_Seki
        (3)RA4 'OUT' Debug Signal
        (4)RA3 'IN'  [MCLR] Vdd between resister 10Kohm
        (5)RA2 'IN'  [INT]
        (6)RA1 'OUT' [ICSPCLK] ON_LED
        (7)RA0 'OUT' [ICSPDAT] OFF_LED
        (8)Vss

        TMR1 Timer Data Paramater List
         Fossc=16MHz TMR1_Priscaler=1
        DaTM1_H	= 0xB9;					// 65536  - 18000	= 47536(0xB9B0)
        DaTM1_L	= 0xB0;					// 0.25uS * 18000	= 4.5msec
        DaTM1_H	= 0xDC;					// 65536  -  9000	= 56536(0xDCD8)
        DaTM1_L	= 0xD8;					// 0.25uS *  9000	= 2.25msec
        DaTM1_H	= 0xEE;					// 65536  -  4500	= 61036(0xEE6C)
        DaTM1_L	= 0x6C;					// 0.25uS *  4500	= 1.125msec

        DaTM1_H	= 0xCB;					// 65536  - 13500	= 52036(0xCB44)
        DaTM1_L	= 0x44;					// 0.25uS * 13500	= 3.375msec
        DaTM1_H	= 0xE5;					// 65536  -  6750	= 58786(0xE5A2)
        DaTM1_L	= 0xA2;					// 0.25uS *  6750	= 1.6875msec
        DaTM1_H	= 0xEA;					// 65536  -  5625	= 59911(0xEA07)
        DaTM1_L	= 0x07;					// 0.25uS *  5625	= 1.40625msec
        DaTM1_H	= 0xD8;					// 65536  - 10125	= 55411(0xD873)
        DaTM1_L	= 0x73;					// 0.25uS * 10125	= 2.53125msec

        DaTM1_H	= 0xD7;					// 65536  - 10400	= 55136(0xD760)
        DaTM1_L	= 0x60;					// 0.25uS * 10400	= 2.6msec
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include <xc.h>


#pragma	jis

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)



void IR_DatST(void);
void ActionLED(void);
void Delay_10ms(unsigned char time);
void LED_Flash(unsigned char F_Cnt);

#define MHz 000000
#define _XTAL_FREQ 16MHz

#define IR_Seki	RA5
#define ON_LED	RA1
#define OFF_LED	RA0
#define DebgSig	RA4

unsigned char IR_DatX;
//unsigned int IR_DatY;
unsigned char TMR1_EF; // TMR1 Enalbe Flag
unsigned char TMR1_DC; // TMR1 Data Counter
unsigned char IR_Dat1; // IR_Data1
unsigned char IR_Dat2; // IR_Data2
unsigned char RepeatF; // Reader-Repeat Flag
unsigned char StartF; // START Flag

// 9ms -> 3.375ms -> 2.53125ms -> 1.125ms or 2.25ms
const unsigned int DaTM1_H[5] = {0x73, 0xCB, 0xD8, 0xEE, 0xDC};
const unsigned int DaTM1_L[5] = {0x60, 0x44, 0x73, 0x6C, 0xD8};

// INT External Interrupt //

void interrupt isr(void) {
    // TMR1 Interrupt //
    if (TMR1IF) {
        if (IR_Seki == 0)
            TMR1_DC = 3; // TMR1 1.125ms Data Set
        if (IR_Seki == 1)
            TMR1_DC = 4; // TMR1 2.25ms Data Set
        TMR1H = DaTM1_H[TMR1_DC];
        TMR1L = DaTM1_L[TMR1_DC];
    }
    TMR1_EF = 1;
    TMR1IF = 0; // TMR1 Interrupt Flag Clear
}

void IR_DatST(void) {
    int j;
    unsigned char BitPos;
    IR_DatX = 0;
    BitPos = 0x80;
    for (j = 0; j < 8; j++) { // Data Code 8bit Data Read
        while (TMR1_EF == 0) { // 1.125ms or 2.25ms Check
        } // Custom Code Data Chrck
        TMR1_EF = 0;
        if (IR_Seki == 1)
            IR_DatX |= BitPos;
        BitPos = BitPos >> 1;
        DebgSig = !DebgSig; // Debug Signal ONRY
    }
    DebgSig = 0; // Debug Signal ONRY
}

//Delay 100msec --------------------------------

void Delay_10ms(unsigned char time) {
    while (time--) {
        __delay_ms(10);
    }
}

//------------------main

void main(void) {
    //Initialization
    OSCCON = 0b01111010; // PLL Disable, 16MHz internal Oscillator
    GIE = 0; // Interrupt service is disable
    ANSELA = 0; // ALL digital port
    APFCON = 0b00000001; // Rx/DT=RA1,SDO=RA0,SS=RA3,T1G=RA4,
    // TxCK=RA0,P1B=RA0,CCP1=RA5
    //  		bit 76543210
    nWPUEN = 0; // PULL-UP Enable [OPTION-Registor]
    TRISA = 0b11101100; // GPIO dirction setting
    WPUA = 0b11101100; // Pulll-up Enable Select
    PORTA = 0b11101100; // GPIO initialize
    LATA = 0b11101100;

    T1CON = 0b00000000; // Fosc/4,Prescale=1,T1OSCEN=Disable
    // T1SYNC=Yes-SYNC,TMR1ON=STOP
    // (1/(16MHz/4))*1 = 0.25uS(400kHz)
    //	TMR1H	= 0x73;					// 65536  - 36000	= 29536(0x7360)
    //	TMR1L	= 0x60;					// 0.25uS * 36000	= 9msec
    TMR1H = 0xEE; // 65536  -  4500	= 61036(0xEE6C)
    TMR1L = 0x6C; // 0.25uS *  4500	= 1.125msec
    TMR1IF = 0; // TMR1 Interrupt Flag Clear

    // Interrupt Enable //
    TMR1IE = 1; // TMR1 Interrput Enable
    PEIE = 1; // Peripheral Interrput Enable
    GIE = 1; // Global Interrupt Enable

    TMR1_DC = 0;
    TMR1_EF = 0;

    ON_LED = 1;
    Delay_10ms(20);
    ON_LED = 0;
    Delay_10ms(20);
    OFF_LED = 1;
    Delay_10ms(20);
    OFF_LED = 0;


    //---------------------------------------------------------
    while (1) {
        while (IR_Seki == 1) { // IR-Sencer Faill Check!
            __delay_ms(1);

        }
        // 9ms Pulse Witdh Check //
        while (1) {
            RA3 = 1;
            if (IR_Seki == 0) {
                __delay_ms(4);
                if (IR_Seki == 0)
                    __delay_ms(4);
                else
                    break;
            } else
                break;

            // 4.5ms Pulse Witdh Check //
            while (IR_Seki == 0) { // IR-Sencer Rise Check!
                RA3 = 0;
            }
            if (IR_Seki == 1) {
                __delay_us(3500);
                if (IR_Seki == 1)
                    RepeatF = 1;
                else
                    RepeatF = 0;
            } else
                break;

            // Cusf1 tom Code 16bit Pulse Check //
            while (IR_Seki == 1) { // IR-Senser Faill Check
                    RA3 = 1;
            }
            __delay_us(1400);
            DebgSig = 1; // Debug Onry

            if ((IR_Seki == 0) && (RepeatF == 1)) {
                // Timming Fine Adjustment //
                unsigned char i;
                i = 0;
                for (i = 0; i < 15; i++) {
                    //					DebgSig =! DebgSig;			// Debug Onry
                    while (IR_Seki == 0) { // IR-Sencer Rise Check!
                        RA3 = 0;
                    }
                    __delay_us(300);
                    while (IR_Seki == 1) { // IR-Senser Faill Check
                        RA3 = 1;
                    }
                    __delay_us(300);
                    DebgSig = !DebgSig; // Debug Onry
                }
                DebgSig = 0; // Debug Onry
            } else
                break;

            // Timming Fine Adjustment //
            //			__delay_us(10);

            TMR1ON = 1; // TMR1 Start

            // Deta Code 8bit Data Read //
            IR_DatST();
            IR_Dat1 = IR_DatX;
            // Reverse Data Code 8bit Data Read //
            IR_DatST();
            IR_Dat2 = IR_DatX;

            TMR1ON = 0; // TMR1 Stop
            ActionLED();
        }
        TMR1_DC = 0;
        TMR1H = 0xEE; // 65536  -  4500	= 61036(0xEE6C)
        TMR1L = 0x6C; // 0.25uS *  4500	= 1.125msec

        // Repeat Timming Adjustment //
        //		__delay_ms(40);							// Stop Bit
        Delay_10ms(20); // 0.2sec Wait
    }
}

// IR Data Action //

void ActionLED(void) {
    if ((IR_Dat1 == 0xD8) && (IR_Dat2 == 0x27)) {
        // 'POWER' Switch //
        ON_LED = 1; // ON LED 'ON'
        OFF_LED = 0; // OFF LED 'OFF'
    }
    if ((IR_Dat1 == 0x60) && (IR_Dat2 == 0x9F)) {
        // 'FLASH' Switch //
        ON_LED = 0; // ON LED 'OFF'
        OFF_LED = 1; // OFF LED 'ON'
    }
    if ((IR_Dat1 == 0xB8) && (IR_Dat2 == 0x47)) {
        // 'ESC' Switch //
        ON_LED = 0; // ON LED 'OFF'
        OFF_LED = 0; // OFF LED 'OFF'
    }
    if ((IR_Dat1 == 0xA0) && (IR_Dat2 == 0x5F)) {
        // '1' Switch //
        LED_Flash(1);
    }
    if ((IR_Dat1 == 0x00) && (IR_Dat2 == 0xFF)) {
        // '2' Switch //
        LED_Flash(2);
    }
    if ((IR_Dat1 == 0x10) && (IR_Dat2 == 0xEF)) {
        // '3' Switch //
        LED_Flash(3);
    }
    if ((IR_Dat1 == 0x80) && (IR_Dat2 == 0x7F)) {
        // '4' Switch //
        LED_Flash(4);
    }
    if ((IR_Dat1 == 0x20) && (IR_Dat2 == 0xDF)) {
        // '5' Switch //
        LED_Flash(5);
    }
    if ((IR_Dat1 == 0xB1) && (IR_Dat2 == 0x4E)) {
        // '6' Switch //
        LED_Flash(6);
    }
    if ((IR_Dat1 == 0x11) && (IR_Dat2 == 0xEE)) {
        // '7' Switch //
        LED_Flash(7);
    }
    if ((IR_Dat1 == 0x21) && (IR_Dat2 == 0xDE)) {
        // '8' Switch //
        LED_Flash(8);
    }
    if ((IR_Dat1 == 0x81) && (IR_Dat2 == 0x7E)) {
        // '9' Switch //
        LED_Flash(9);
    }

}

void LED_Flash(unsigned char F_Cnt) {
    unsigned char k;
    ON_LED = 0; // ON LED 'OFF'
    OFF_LED = 0; // OFF LED 'OFF'
    for (k = 0; F_Cnt > k; k++) {
        ON_LED = 1;
        Delay_10ms(20);
        ON_LED = 0;
        Delay_10ms(20);
    }
    ON_LED = 0; // ON LED 'OFF'
}

