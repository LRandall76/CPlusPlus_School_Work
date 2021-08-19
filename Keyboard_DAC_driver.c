//Main.c
#include <stdint.h>
#include "DAC.h"
#include "Piano.h"
#include "Sound.h"
#include "msp432p401r.h"
#include "SysTickInts.h"
#define P5IN5     (*((volatile uint8_t *) 0x40004C40))

void main(void) {
    uint8_t  counter = 0; // Used For Indexing through Sine Wave 0-31
    uint8_t  wave = 0;  // 0 to 15 DAC Output
    uint8_t  note = 0;  // Note To Be Played Should be 1-3 if 0 then no Output

    DAC_Init();
    Piano_Init();
    Sound_Init();
    SysTick_Init(0);


    while(1) {

        note = Piano_In();

        wave = Sound_play(note, counter);

        DAC_Out(wave);

        // Adjust Counter For Sound Wave
        counter = counter + 1;
        if (counter > 31)
        {counter = 0;}

     }
}








//DAC.c
/*
 * DAC.c
 *
 *  Created on: Oct 12, 2020
 *      Author: retribuXion
 */
#include <stdint.h>

#define P4OUT4    (*((volatile uint8_t *) 0x40004C23)) // Used For Output
#define P4DIR4    (*((volatile uint8_t *) 0x40004C25))
#define P4SEL04   (*((volatile uint8_t *) 0x40004C2B))
#define P4SEL14   (*((volatile uint8_t *) 0x40004C2D))


void DAC_Init() {

      // Port 4 = Output
      // Turn On Ports:
      // 4.3  4.2  4.1  4.0
      P4SEL04 &= 0xF0; // F0 = 1111 0000
      P4SEL14 &= 0xF0;
      P4DIR4  |= 0x0F; // 0F = 0000 1111


}

void DAC_Out(uint8_t Data){

    P4OUT4 = Data; // Simply Reads 4 Bit Input and Outputs it
    // Writes to Resister Outputs
}

DAC.h
/*
 * DAC.h
 *
 *  Created on: Oct 12, 2020
 *      Author: retribuXion
 */

#ifndef DAC_H_
#define DAC_H_


void DAC_Init();
void DAC_Out(uint8_t Data);


#endif /* DAC_H_ */

//Piano.c


/*
 * Piano.c
 *
 *  Created on: Oct 12, 2020
 *      Author: retribuXion
 */
#include <stdint.h>

#define P5IN5     (*((volatile uint8_t *) 0x40004C40)) // Used For Input
#define P5DIR5    (*((volatile uint8_t *) 0x40004C44))
#define P5SEL05   (*((volatile uint8_t *) 0x40004C4A))
#define P5SEL15   (*((volatile uint8_t *) 0x40004C4C))

void Piano_Init() {
      // Port 5 = Input
      // 5.0  5.1  5.2
      P5SEL05 &= 0xF8;  // F8 = 1111 1000
      P5SEL15 &= 0xF8;
      P5DIR5  &= 0xF8;
}



uint8_t Piano_In(){ // This Function Translates the Button Inputs
                                // to One of the Three Notes
    uint8_t note = 0;
    uint8_t temp;


    // Read Data into Temporary Variable to be Bit Cleared
    temp = P5IN5;
    temp &= 0x01;

    // If Temp is not 0 that means that bit is high
    if (temp){
        note = 0x01;
        return note;
    }


    // Read Data into Temporary Variable to be Bit Cleared
    temp = P5IN5;
    temp &= 0x02;

    // If Temp is not 0 that means that bit is high
    if (temp){
        note = 0x02;
        return note;
        }


    // Read Data into Temporary Variable to be Bit Cleared
    temp = P5IN5;
    temp &= 0x04;

    // If Temp is not 0 that means that bit is high
    if (temp){
        note = 0x03;
        return note;
        }


    // If we get here no note is pressed
    // or an unexpected result has occurred so just return note = 0
    return note;


}

//Piano.h

/*
 * Piano.h
 *
 *  Created on: Oct 12, 2020
 *      Author: retribuXion
 */

#ifndef PIANO_H_
#define PIANO_H_

void Piano_Init();
uint8_t Piano_In();



#endif /* PIANO_H_ */

//Sound.c


/*
 * Sound.c
 *
 *  Created on: Oct 12, 2020
 *      Author: retribuXion
 */
#include <stdint.h>
#include "SysTickInts.h"

struct Freq {    // Notes will be E A D from Guitar Strings
    uint32_t FrequencyE; //  330 Hz
    uint32_t FrequencyA; //  440 Hz
    uint32_t FrequencyD; //  294 Hz
    uint8_t Sine[32];

};




void Sound_Init() {
    // Was going to hold data structure but instead I am just using Sound_play to hold it
    return;
}



uint8_t Sound_play(uint8_t note, uint8_t counter){ // This Function Reads In a Number and Messes with Systick Delay

    typedef const struct Freq Frequencies;
    Frequencies idk = {284, 213, 319,{8,9,11,12,13,14,14,15,15,15,14,14,13,12,11,9,8,7,5,4,3,2,2,1,1,1,2,2,3,4,5,7}};

    Frequencies *Pt;
    Pt = &idk;

   if (note == 1){SysTick_Wait(Pt->FrequencyE);}


   if (note == 2){SysTick_Wait(Pt->FrequencyA);}


   if (note == 3){SysTick_Wait(Pt->FrequencyD);}





   if (note == 0){return 0;}

    // Change Systick Reload Value
   return Pt->Sine[counter];



}

//Sound.h

/*
 * Sound.h
 *
 *  Created on: Oct 12, 2020
 *      Author: retribuXion
 */

#ifndef SOUND_H_
#define SOUND_H_


void Sound_Init();
uint8_t Sound_play(uint8_t note, uint8_t wave);

#endif /* SOUND_H_ */

//SysTickInts.c

// SysTickInts.c
// Runs on MSP432
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano, Jonathan Valvano
// June 1, 2015

/* This example accompanies the books
   "Embedded Systems: Introduction to MSP432 Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015
   Volume 1 Program 9.7

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#include <stdint.h>
#include "Sound.h"
#include "msp432p401r.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

// **************SysTick_Init*********************
// Initialize SysTick periodic interrupts
// Input: interrupt period
//        Units of period are 333ns (assuming 3 MHz clock)
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none
volatile uint32_t Counts;
void SysTick_Init(void){
    SysTick->CTRL = 0;                    // disable SysTick during setup
    SysTick->LOAD = 0x00FFFFFF;           // maximum reload value
    SysTick->VAL = 0;                    // any write to current clears it
    SysTick->CTRL = 0x00000005;           // enable SysTick with no interrupts
}

/*void SysTick_Init(uint32_t period) {
	long sr = StartCritical();

	Counts = 0;

	SysTick->CTRL = 0;                   // disable SysTick during setup
	SysTick->LOAD = period - 1;          // maximum reload value
	SysTick->VAL = 0;                    // any write to current clears it
	SCB->SHP[3] = (SCB->SHP[3]&0x00FFFFFF)|0x40000000;	// priority 2
	SysTick->CTRL = 0x00000007;          // enable SysTick with no interrupts

	EndCritical(sr);
}*/

void SysTick_Wait(uint32_t delay){
  // method #1: set Reload Value Register, clear Current Value Register, poll COUNTFLAG in Control and Status Register
  if(delay <= 1){
    // without this step:
    // if delay == 0, this function will wait 0x00FFFFFF cycles
    // if delay == 1, this function will never return (because COUNTFLAG is set on 1->0 transition)
    return;                   // do nothing; at least 1 cycle has already passed anyway
  }
  SysTick->LOAD = (delay - 1);// count down to zero
  SysTick->VAL = 0;          // any write to CVR clears it and COUNTFLAG in CSR
  while((SysTick->CTRL&0x00010000) == 0){};
  // method #2: repeatedly evaluate elapsed time
/*  volatile uint32_t elapsedTime;
  uint32_t startTime = SysTick->VAL;
  do{
    elapsedTime = (startTime-SysTick->VAL)&0x00FFFFFF;
  }
  while(elapsedTime <= delay);*/

//void SysTick_Handler(void){
	
}

//SysTickInts.h

// SysTickInts.h
// Runs on MSP432
// Use the SysTick timer to request interrupts at a particular period.
// Jonathan Valvano
// June 1, 2015

/* This example accompanies the books
   "Embedded Systems: Introduction to MSP432 Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015
   Volume 1 Program 9.7

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// P4.0 is an output to profiling scope/logic analyzer

// **************SysTick_Init*********************
// Initialize SysTick periodic interrupts
// Input: interrupt period
//        Units of period are 333ns (assuming 3 MHz clock)
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none
void SysTick_Init(uint32_t period);
void SysTick_Wait(uint32_t delay);








