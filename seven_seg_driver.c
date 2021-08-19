//Serial Communication
//and 7 Segment Displays
//Main.c
#include "msp432p401r.h"
#include "SSEG.h"
#include "SysTick.h"
#include <stdlib.h>
#include <stdio.h>


#define P4OUT4    (*((volatile uint8_t *) 0x40004C23)) // Used For Output
#define P4DIR4    (*((volatile uint8_t *) 0x40004C25))
#define P4SEL04   (*((volatile uint8_t *) 0x40004C2B))
#define P4SEL14   (*((volatile uint8_t *) 0x40004C2D))

#define P5IN5     (*((volatile uint8_t *) 0x40004C40)) // Used For Input
#define P5OUT5     (*((volatile uint8_t *) 0x40004C42)) // Used For Input
#define P5DIR5    (*((volatile uint8_t *) 0x40004C44))
#define P5SEL05   (*((volatile uint8_t *) 0x40004C4A))
#define P5SEL15   (*((volatile uint8_t *) 0x40004C4C))



void main() {
	
    SysTick_Init();
    SSEG_Init();

    int w = 0xABCD;

    w = 0x9970; // Number to be Displayed
    SSEG_Out(w);
         while(1){

             int count = 0;
             char str[50] = "Please enter a number between 0 - 9999: \n";
             printf(str,"%s \n");

             scanf("%X", &w);

             SSEG_Out(w);

          if(w < 0x10000){
             while (count < 300){
             P4OUT4 = 0x01;
             SSEG_Shift_Out(0);
             P4OUT4 = 0x02;
             SSEG_Shift_Out(1);
             P4OUT4 = 0x04;
             SSEG_Shift_Out(2);
             P4OUT4 = 0x08;
             SSEG_Shift_Out(3);
             count = count + 1;
             }
             count = 0;
            }

         }
}


//SSEG.c

#include <stdint.h>
#include "SysTick.h"
#include "msp432p401r.h"
#include "SSEG.h"

// Global Variables
char out_num;


void DisableInterrupts();			// Disable interrupts
void EnableInterrupts();			// Enable interrupts
long StartCritical ();				// previous I bit, disable interrupts
void EndCritical(long sr);			// restore I bit to previous value
void WaitForInterrupt();			// low power mode


// Array Holding Backward Serial Value Of 7 Segment Display
volatile int arrr[17][8] = {
                  {0,0,1,1,1,1,1,1}, // 0
                  {0,0,0,0,0,1,1,0}, // 1
                  {0,1,0,1,1,0,1,1}, // 2
                  {0,1,0,0,1,1,1,1}, // 3
                  {0,1,1,0,0,1,1,0}, // 4

                  {0,1,1,0,1,1,0,1}, // 5
                  {0,1,1,1,1,1,0,1}, // 6
                  {0,0,0,0,0,1,1,1}, // 7
                  {0,1,1,1,1,1,1,1}, // 8
                  {0,1,1,0,0,1,1,1}, // 9

                  {0,1,1,1,0,1,1,1}, // A
                  {0,1,1,1,1,1,0,0}, // b
                  {0,0,1,1,1,0,0,1}, // C
                  {0,1,0,1,1,1,1,0}, // d
                  {0,1,1,1,1,0,0,1}, // E

                  {0,1,1,1,0,0,0,1},  // F
                  {0,0,0,0,0,0,0,0}  // Turn Off

};


#define P4OUT4    (*((volatile uint8_t *) 0x40004C23)) // Used For Output
#define P4DIR4    (*((volatile uint8_t *) 0x40004C25))
#define P4SEL04   (*((volatile uint8_t *) 0x40004C2B))
#define P4SEL14   (*((volatile uint8_t *) 0x40004C2D))

#define P5IN5     (*((volatile uint8_t *) 0x40004C40)) // Used For Input
#define P5OUT5     (*((volatile uint8_t *) 0x40004C42)) // Used For Input
#define P5DIR5    (*((volatile uint8_t *) 0x40004C44))
#define P5SEL05   (*((volatile uint8_t *) 0x40004C4A))
#define P5SEL15   (*((volatile uint8_t *) 0x40004C4C))

#define xlocation   (*((volatile uint8_t *) 0x20000700))
int *ptr;
/*
 * SSEG_Init Function
 * Initialize 7-segment display
 * Inputs: none
 * Outputs: none
 */
void SSEG_Init() {
            DisableInterrupts();
             //SSEG INIT START
             // Port 4 = Output
             // Turn On Ports:
             // 4.3  4.2  4.1  4.0
             P4SEL04 &= 0xF0; // F0 = 1111 0000
             P4SEL14 &= 0xF0;
             P4DIR4  |= 0x0F; // 0F = 0000 1111

             // Turn on the First Digit
             P4OUT4 = 0x0F;

             // Port 5 = Output
             // Turn On Ports:
             // 5.2  5.1  5.0
             // Used for Mux
             P5SEL05 &= 0xF8; // F8 = 1111 1000
             P5SEL15 &= 0xF8; // F8 = 1111 1000
             P5DIR5  |= 0x07; // 07 = 0000 0111

             // Use Port 5.4 and 5.5 for button switches
             P5SEL05 &= 0xCF; // F8 = 1100 1111
             P5SEL15 &= 0xCF; // F8 = 1100 1111
             P5DIR5  &= 0xCF; // 07 = 0011 0000
             // END

             P5IES &= 0xCF;
             P5IFG &= 0xCF;
             P5IE |= 0x30;
             NVIC->IP[9] = (NVIC->IP[9]&0x00FFFFFF)|0x40000000;
             NVIC->ISER[1] = 0x00000080;


             int n=0;
             ptr = &xlocation; // Give Location of Stack Numbers to be Modified

             while(n < 8){
                          P5OUT5 &= 0xFD; // Turn Reset Signal Low
                          if(arrr[0][n] == 1){
                              P5OUT5 &= 0xFB;} // Send Serial Bit 1
                          else{
                              P5OUT5 |= 0x4; } // Send No Bit 0
                          P5OUT5 |= 0x01; // Turn on Clock Signal
                          n = n + 1; // Increment
                          P5OUT5 &= 0xFE; // Turn Off Clock Signal
                          P5OUT5 |= 0x02; // Turn On Reset
                                  }



           EnableInterrupts();
           return;
}

/*
 * SSEG_Out Function
 * Output a number to a single digit of the 7-segment display
 * Inputs: a number between 0 and 15
 * Outputs: none
 */
void SSEG_Out(int num) {

int m = num;
int powr = 1;
int i;
int j;


for (i = 0; i < 4; i = i+1){
    for(j=0; j < 3 - i; j = j +1)
                       {
                       m   = m / 16;
                       powr = powr * 16;
                       }

             m = m % 16; // Second Extraction
             ptr[3-i] = m;
             m = m * powr; // Multiply to use as Modulus
             num = num % m; // Get Remaining Numbers
             m = num;
             powr = 1;
}

return;
}





/*
 * Port 5 ISR
 * Uses P5IV to solve critical section/race
 */
void PORT5_IRQHandler() {

    int buttons = P5IN5;
    int button1 = P5IN5;
    int button2 = P5IN5;
    buttons &= 0x30;
    button1 &= 0x20; //
    button2 &= 0x10;

   int status = P5IV;
    if(buttons == 0x30){}//Do Nothing
    else if(button1 == 0x20){
        if(ptr[0] == 9 && ptr[1] == 9 && ptr[2] == 9 && ptr[3] == 9){ptr[0] = 0; ptr[1] = 0; ptr[2] = 0; ptr[3] = 0;}
        else if(ptr[0] == 9 && ptr[1] == 9 && ptr[2] == 9){ptr[0] = 0; ptr[1] = 0; ptr[2] = 0; ptr[3] = ptr[3] + 1;}
        else if(ptr[0] == 9 && ptr[1] == 9){ptr[0] = 0; ptr[1] = 0;ptr[2] = ptr[2] + 1;}
        else if(ptr[0] == 9){ptr[0] = 0; ptr[1] = ptr[1] + 1;}
        else{ptr[0] = ptr[0] + 1;}}
    else if(button2 == 0x10){
        if(ptr[0] == 0 && ptr[1] == 0 && ptr[2] == 0 && ptr[3] == 0){ptr[0] = 9; ptr[1] = 9; ptr[2] = 9; ptr[3] = 9;}
        else if(ptr[0] == 0 && ptr[1] == 0 && ptr[2] == 0){ptr[0] = 9; ptr[1] = 9; ptr[2] = 9; ptr[3] = ptr[3] - 1;}
        else if(ptr[0] == 0 && ptr[1] == 0){ptr[0] = 9; ptr[1] = 9; ptr[2] = ptr[2] - 1;}
        else if(ptr[0] == 0){ptr[0] = 9; ptr[1] = ptr[1] - 1;}
        else{ptr[0] = ptr[0] - 1;}}




           P5IFG &= 0xCF;
           SysTick_Wait(60000);
}


/*
 * SSEG_Shift_Out Function
 * Shifts data out serially
 * Inputs: 8-bit data
 * Outputs: none
 */
void SSEG_Shift_Out(char b){

int status;
int n = 0;
status = P5IV;
if (status == 0){
while(n < 8){
              P5OUT5 &= 0xFD; // Turn Reset Signal Low
              if(arrr[ptr[3-b]][n] == 1){
                  P5OUT5 &= 0xFB;} // Send Serial Bit 1
              else{
                  P5OUT5 |= 0x4; } // Send No Bit 0
              P5OUT5 |= 0x01; // Turn on Clock Signal
              n = n + 1; // Increment
              P5OUT5 &= 0xFE; // Turn Off Clock Signal
              P5OUT5 |= 0x02; // Turn On Reset
                      }
}

SysTick_Wait(20000);
}

/*
 * SSEG_Disp_Num Function
 * Separate the input number into 4 single digit
 * Inputs: num between 0 and 9999
 * Outputs: none
 */
void SSEG_Disp_Num(int num){


int m = num;
int powr = 1;
int i;
int j;


for (i = 0; i < 4; i = i+1){
    for(j=0; j < 3 - i; j = j +1)
                       {
                       m   = m / 10;
                       powr = powr * 10;
                       }

             m = m % 10; // Second Extraction
             ptr[3-i] = m;
             m = m * powr; // Multiply to use as Modulus
             num = num % m; // Get Remaining Numbers
             m = num;
             powr = 1;
}

return;}



/*
 * SSEG_Off Function
 * Turns off all 7-seg digits
 * Inputs: none
 * Outputs: none
 */
void SSEG_Off(){
    ptr[0] = 16;
    ptr[1] = 16;
    ptr[2] = 16;
    ptr[3] = 16;
    return;
}
