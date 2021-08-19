#include <stdint.h>
#include "SysTick.h"
#include "msp432p401r.h"

extern void asm_main();
extern void systickinit();



struct State {
    uint32_t Out;
    uint32_t XOut;
    uint32_t Time;  // 10 ms units
    const struct State *Next[26];
};

typedef const struct State STyp;

#define SENSOR  (*((volatile uint8_t *) 0x40004C40))
#define LIGHT   (*((volatile uint8_t *) 0x40004C23))
#define XSENSOR (*((volatile uint8_t *) 0x40004C00)) // Key for Cross walk button P1.1
#define XLIGHT  (*((volatile uint8_t *) 0x40004C03)) // Key for Cross walk Light P.2


#define SYSCS    (*((volatile uint8_t *) 0xE000E010))
#define SYSRV    (*((volatile uint8_t *) 0xE000E014))
#define SYSCV    (*((volatile uint8_t *) 0xE000E018))
#define SYSCAL   (*((volatile uint8_t *) 0xE000E01C))


#define P1IN1     (*((volatile uint8_t *) 0x40004C00))
#define P1OUT1    (*((volatile uint8_t *) 0x40004C02))
#define P1REN1    (*((volatile uint8_t *) 0x40004C06))
#define P1DIR1    (*((volatile uint8_t *) 0x40004C04))
#define P1SEL01   (*((volatile uint8_t *) 0x40004C0A))
#define P1SEL11   (*((volatile uint8_t *) 0x40004C0C))


#define P2OUT2    (*((volatile uint8_t *) 0x40004C03))
#define P2DIR2    (*((volatile uint8_t *) 0x40004C05))
#define P2SEL02   (*((volatile uint8_t *) 0x40004C0B))
#define P2SEL12   (*((volatile uint8_t *) 0x40004C0D))


#define P4IN4     (*((volatile uint8_t *) 0x40004C21))
#define P4OUT4    (*((volatile uint8_t *) 0x40004C23))
#define P4DIR4    (*((volatile uint8_t *) 0x40004C25))
#define P4SEL04   (*((volatile uint8_t *) 0x40004C2B))
#define P4SEL14   (*((volatile uint8_t *) 0x40004C2D))


#define P5IN5     (*((volatile uint8_t *) 0x40004C40))
#define P5DIR5    (*((volatile uint8_t *) 0x40004C44))
#define P5SEL05   (*((volatile uint8_t *) 0x40004C4A))
#define P5SEL15   (*((volatile uint8_t *) 0x40004C4C))

#define goS    &FSM[0]
#define waitS  &FSM[1]
#define goW    &FSM[2]
#define waitW  &FSM[3]
#define goXS   &FSM[4]
#define CDownS &FSM[5]
#define goXW   &FSM[6]
#define CDownW &FSM[7]
#define XwaitS &FSM[8] // These Two States are there to check if the button is pressed for 2 seconds
#define XwaitW &FSM[9]

#define BlinkS1 &FSM[10]
#define BlinkS2 &FSM[11]
#define BlinkS3 &FSM[12]
#define BlinkS4 &FSM[13]
#define BlinkS5 &FSM[14]
#define BlinkS6 &FSM[15]
#define BlinkS7 &FSM[16]
#define BlinkS8 &FSM[17]

#define BlinkW1 &FSM[18]
#define BlinkW2 &FSM[19]
#define BlinkW3 &FSM[20]
#define BlinkW4 &FSM[21]
#define BlinkW5 &FSM[22]
#define BlinkW6 &FSM[23]
#define BlinkW7 &FSM[24]
#define BlinkW8 &FSM[25]

STyp FSM[26] = {
                // For Switching Through Roads and CrossWalk
                {0x21,0x1, 100,{goS,waitS,goS,waitS,XwaitS,XwaitS,XwaitS,XwaitS}},
                {0x22,0x1, 50,{goW,goW,goW,goW,goXS,goXS,goXS,goXS}},
                {0x0C,0x1, 100,{goW,goW,waitW,waitW,XwaitW,XwaitW,XwaitW,XwaitW}},
                {0x14,0x1, 50,{goS,goS,goS,goS,goXW,goXW,goXW,goXW}},
                {0x24,0x2, 100,{BlinkS1,BlinkS1,BlinkS1,BlinkS1,BlinkS1,BlinkS1,BlinkS1,BlinkS1}},
                {0x24,0x1, 0,{goS,goW,goS,goW,goS,goW,goS,goW}},
                {0x24,0x2, 100,{BlinkW1,BlinkW1,BlinkW1,BlinkW1,BlinkW1,BlinkW1,BlinkW1,BlinkW1}},
                {0x24,0x1, 0,{goW,goW,goS,goS,goW,goW,goS,goS}},

                // Delay For CrossWalk to Check for 2 Seconds
                {0x21,0x1, 200,{goS,goS,goS,goS,waitS,waitS,waitS,waitS}},
                {0x0C,0x1, 200,{goW,goW,goW,goW,waitW,waitW,waitW,waitW}},

                // Blink South
                {0x24,0x1, 90,{BlinkS2,BlinkS2,BlinkS2,BlinkS2,BlinkS2,BlinkS2,BlinkS2,BlinkS2}},
                {0x24,0x0, 90,{BlinkS3,BlinkS3,BlinkS3,BlinkS3,BlinkS3,BlinkS3,BlinkS3,BlinkS3}},
                {0x24,0x1, 90,{BlinkS4,BlinkS4,BlinkS4,BlinkS4,BlinkS4,BlinkS4,BlinkS4,BlinkS4}},
                {0x24,0x0, 90,{BlinkS5,BlinkS5,BlinkS5,BlinkS5,BlinkS5,BlinkS5,BlinkS5,BlinkS5}},
                {0x24,0x1, 90,{BlinkS6,BlinkS6,BlinkS6,BlinkS6,BlinkS6,BlinkS6,BlinkS6,BlinkS6}},
                {0x24,0x0, 90,{BlinkS7,BlinkS7,BlinkS7,BlinkS7,BlinkS7,BlinkS7,BlinkS7,BlinkS7}},
                {0x24,0x1, 90,{BlinkS8,BlinkS8,BlinkS8,BlinkS8,BlinkS8,BlinkS8,BlinkS8,BlinkS8}},
                {0x24,0x0, 90,{CDownS,CDownS,CDownS,CDownS,CDownS,CDownS,CDownS,CDownS}},
                // Blink West
                {0x24,0x1, 90,{BlinkW2,BlinkW2,BlinkW2,BlinkW2,BlinkW2,BlinkW2,BlinkW2,BlinkW2}},
                {0x24,0x0, 90,{BlinkW3,BlinkW3,BlinkW3,BlinkW3,BlinkW3,BlinkW3,BlinkW3,BlinkW3}},
                {0x24,0x1, 90,{BlinkW4,BlinkW4,BlinkW4,BlinkW4,BlinkW4,BlinkW4,BlinkW4,BlinkW4}},
                {0x24,0x0, 90,{BlinkW5,BlinkW5,BlinkW5,BlinkW5,BlinkW5,BlinkW5,BlinkW5,BlinkW5}},
                {0x24,0x1, 90,{BlinkW6,BlinkW6,BlinkW6,BlinkW6,BlinkW6,BlinkW6,BlinkW6,BlinkW6}},
                {0x24,0x0, 90,{BlinkW7,BlinkW7,BlinkW7,BlinkW7,BlinkW7,BlinkW7,BlinkW7,BlinkW7}},
                {0x24,0x1, 90,{BlinkW8,BlinkW8,BlinkW8,BlinkW8,BlinkW8,BlinkW8,BlinkW8,BlinkW8}},
                {0x24,0x0, 90,{CDownW,CDownW,CDownW,CDownW,CDownW,CDownW,CDownW,CDownW}}
};





void main(void){
    STyp    *Pt;
    uint32_t Input;
    uint32_t XInput;

   // Systick Initialization

    SYSCS  &= ~0x1;
    SYSRV  |= 0xFF;
    SYSCV  |= 0x77;
    SYSCS |= 0x5;
    SYSCS &= ~0x2;

    // Port Initialization
    P1SEL01 &= 0x0;
    P1SEL11 &= 0x0;
    P1DIR1  |= 0x1;
    P1OUT1  |= 0x12;
    P1REN1  |= 0x12;

    P2SEL02 &= 0xF8;
    P2SEL12 &= 0xF8;
    P2DIR2  |= 0x7;

    P4SEL04 &= 0xC0;
    P4SEL14 &= 0xC0;
    P4DIR4  |= 0x3F;

    P5SEL05 &= 0xFC;
    P5SEL15 &= 0xFC;
    P5DIR5  |= 0xFC;


    Pt = goS; // start state
    while(1) {
        LIGHT  = Pt->Out;  // set lights
        XLIGHT = Pt ->XOut;
        SysTick_Wait10ms(Pt->Time);

    Input  = SENSOR;  // Read External Buttons
    Input  &= ~0xFC;  // Select Bits

    XInput = ~XSENSOR; // Read Cross Walk Button
    XInput &= 0x10;
    XInput = XInput >> 2; // Shift the Bit to the Right

    Input |= XInput;

    Pt = Pt->Next[Input];

    }
}
