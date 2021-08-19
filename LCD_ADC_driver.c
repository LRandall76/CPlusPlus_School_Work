//LCD and ADC

//Main.c
#include <stdint.h>
#include "LCD.h"
#include "SysTick.h"
#include "ADC14.h"
#include "msp432p401r.h" // Is this one important?

#define P2OUT2    (*((volatile uint8_t *) 0x40004C03))
#define P2DIR2    (*((volatile uint8_t *) 0x40004C05))
#define P2SEL02   (*((volatile uint8_t *) 0x40004C0B))
#define P2SEL12   (*((volatile uint8_t *) 0x40004C0D))

 uint8_t ADCStatus;
 uint8_t *ADCref;

void main()
{
    uint32_t  mailbox;


    ADCref = &ADCStatus;

    P2SEL02 &= 0xF8;
    P2SEL12 &= 0xF8;
    P2DIR2  |= 0x7;


   uint32_t wait25ms = 3000000 / 43;



    SysTick_Init(wait25ms,ADCref,0);
    LCD_Init();
    ADC0_InitSWTriggerCh0();
    SysTick_Init(wait25ms,ADCref,1);

    while(1){
    LCD_Clear(); // Clear Display
        if(ADCStatus == 1){
            P2OUT2  = 0x0;
            mailbox =SysTick_Mailbox();
            mailbox = mailbox * 1000 / 8191; // Convert to the Range of 2 cm
            LCD_OutUFix(mailbox); // Output to LDC
            LCD_OutChar(0x43);
            LCD_OutChar(0x4D);
            ADCStatus = 0;
            SysTick->VAL = 0;
            SysTick_Wait(wait25ms);
            SysTick->CTRL = 0x00000007;
                      }

    }


                      // Main loop
}

//LCD.c
/*
  size is 1*16
  if do not need to read busy, then you can tie R/W=ground
  ground = pin 1    Vss
  power  = pin 2    Vdd   +3.3V or +5V depending on the device
  ground = pin 3    Vlc   grounded for highest contrast
  P6.4   = pin 4    RS    (1 for data, 0 for control/status)
  ground = pin 5    R/W   (1 for read, 0 for write)
  P6.5   = pin 6    E     (enable)
  P4.4   = pin 11   DB4   (4-bit data)
  P4.5   = pin 12   DB5
  P4.6   = pin 13   DB6
  P4.7   = pin 14   DB7
16 characters are configured as 1 row of 16
addr  00 01 02 03 04 05 ... 0F
*/

#include <stdint.h>
#include "LCD.h"
#include "SysTick.h"
#include "msp.h"

// Marcros
#define BusFreq 3					// assuming a 3 MHz bus clock
#define T6us 6*BusFreq				// 6us
#define T40us 40*BusFreq			// 40us
#define T160us 160*BusFreq			// 160us
#define T1ms 1000*BusFreq			// 1ms
#define T1600us 1600*BusFreq		// 1.60ms
#define T5ms 5000*BusFreq			// 5ms
#define T15ms 15000*BusFreq			// 15ms

// Global Vars
uint8_t LCD_RS, LCD_E;				// LCD Enable and Register Select

/**************** Private Functions ****************/

void OutPort6() {
	P6OUT = (LCD_RS<<4) | (LCD_E<<5);
}

void SendPulse() {
	OutPort6();
	SysTick_Wait(T6us);				// wait 6us
	LCD_E = 1;						// E=1, R/W=0, RS=1
	OutPort6();
	SysTick_Wait(T6us);				// wait 6us
	LCD_E = 0;						// E=0, R/W=0, RS=1
	OutPort6();
}

void SendChar() {
	LCD_E = 0;
	LCD_RS = 1;						// E=0, R/W=0, RS=1
	SendPulse();
	SysTick_Wait(T1600us);			// wait 1.6ms
}

void SendCmd() {
	LCD_E = 0;
	LCD_RS = 0;						// E=0, R/W=0, RS=0
	SendPulse();
	SysTick_Wait(T40us);			// wait 40us
}

/**************** Public Functions ****************/
// Clear the LCD
// Inputs: none
// Outputs: none
void LCD_Clear() {
	LCD_OutCmd(0x01);				// Clear Display
	LCD_OutCmd(0x80);				// Move cursor back to 1st position
}

// Initialize LCD
// Inputs: none
// Outputs: none
void LCD_Init() {
	P4SEL0 &= ~0xF0;
	P4SEL1 &= ~0xF0;				// configure upper nibble of P4 as GPIO
	P4DIR |= 0xF0; 					// make upper nibble of P4 out

	P6SEL0 &= ~0x30;
	P6SEL1 &= ~0x30;				// configure P6.4 and P6.5 as GPIO
	P6DIR |= 0x30;					// make P6.4 and P6.5 out

	//Clock_Init48MHz();				// set system clock to 48 MHz
	// Volume 1 Program 4.7, Volume 2 Program 2.12

	LCD_E = 0;
	LCD_RS = 0;						// E=0, R/W=0, RS=0
	OutPort6();

	LCD_OutCmd(0x30);				// command 0x30 = Wake up
	SysTick_Wait(T5ms);				// must wait 5ms, busy flag not available
	LCD_OutCmd(0x30);				// command 0x30 = Wake up #2
	SysTick_Wait(T160us);			// must wait 160us, busy flag not available
	LCD_OutCmd(0x30);				// command 0x30 = Wake up #3
	SysTick_Wait(T160us);			// must wait 160us, busy flag not available

	LCD_OutCmd(0x28);				// Function set: 4-bit/2-line
	LCD_Clear();
	LCD_OutCmd(0x10);				// Set cursor
	LCD_OutCmd(0x06);				// Entry mode set
}

// Output a character to the LCD
// Inputs: letter is ASCII character, 0 to 0x7F
// Outputs: none
void LCD_OutChar(char letter) {
	unsigned char let_low = (0x0F&letter)<<4;
	unsigned char let_high = 0xF0&letter;

	P4OUT = let_high;
	SendChar();
	P4OUT = let_low;
	SendChar();
	SysTick_Wait(T1ms);				// wait 1ms
}

// Output a command to the LCD
// Inputs: 8-bit command
// Outputs: none
void LCD_OutCmd(unsigned char command) {
	unsigned char com_low = (0x0F&command)<<4;
	unsigned char com_high = 0xF0&command;
	P4OUT = com_high;
	SendCmd();
	P4OUT = com_low;
	SendCmd();
	SysTick_Wait(T1ms);				// wait 1ms
}

//------------LCD_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void LCD_OutString(char *pt) {

    int i = 0;

    for(i = 0; i < sizeof(pt) + 1; i = i + 1)
    {LCD_OutChar(*(pt + i));}

}

//-----------------------LCD_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void LCD_OutUDec(uint32_t n) {
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string

    int i = 0;
    int count = 0;
    int powr = 1;
    int m = n;

    // Check if Scanned through all Numbers
    while (m == 0)
    {

          m = m/10;
        i = i + 1;
        if(i == 8){return;} // Exit Function if So
    }

    // Count Remaining Integers
    m = n;
    do
      {
          count = count + 1;
          m = m / 10;

          if(m == 0)
          {
          while (i != 8)
             {
                 m = m/10;
                 i = i + 1;
                 if(m != 0){count = count + 1;} // Make Sure Number is All Zero
             }
          }
      } while(m != 0);


     // Find the First Number on the Left
     m=n;

     for( i=0; i < (count-1); i = i +1)
            {
            m   = m / 10;
            powr = powr * 10;
            }

     m = m % 10; // Second Extraction
     m = m + 48; //add 0x30
     LCD_OutChar(m);

     m = m - 48; // Restore Number
     m = m * powr; // Multiply to use as Modulus
     n = n % m; // Get Remaining Numbers

        LCD_OutUDec(n);

}

//--------------------------LCD_OutUHex----------------------------
// Output a 32-bit number in unsigned hexadecimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1 to 8 digits with no space before or after
void LCD_OutUHex(uint32_t number) {
// This function uses recursion to convert the number of
//   unspecified length as an ASCII string

      int i = 0;
      int count = 0;
      int powr = 1;
      int addto = 48;
      int m = number;

      // Check if Scanned through all Numbers
      while (m == 0)
      {

            m = m/16;
          i = i + 1;
          if(i == 8){return;}// Exit Function if So
      }

      // Count Remaining Integers
      m = number;
      do
        {
            count = count + 1;
            m = m / 16;

            if(m == 0)
            {
            while (i != 8)
               {
                   m = m/16;
                   i = i + 1;
                   if(m != 0){count = count + 1;} // Make Sure Number is All Zero
               }
            }
        } while(m != 0);


      // Find the First Number on the Left
       m=number;

       for( i=0; i < (count-1); i = i +1)
              {
              m   = m / 16;
              powr = powr * 16;
              }


       m = m % 16; // Second Extraction

       if(m >= 10)  // Check if Number is a Letter
       {

           addto = 55; // Change if So
       }
       m = m + addto;
       LCD_OutChar(m);

       m = m - addto; // Restore
       m = m * powr; // Multiply by shift
       number = number % m; // Get Remaining Numbers

          LCD_OutUHex(number);

}

// -----------------------LCD_OutUFix----------------------
// Output characters to LCD display in fixed-point format
// unsigned decimal, resolution 0.001, range 0.000 to 9.999
// Inputs:  an unsigned 32-bit number
// Outputs: none
// E.g., 0,    then output "0.000 "
//       3,    then output "0.003 "
//       89,   then output "0.089 "
//       123,  then output "0.123 "
//       9999, then output "9.999 "
//       9999, then output "*.*** "
void LCD_OutUFix(uint32_t n) {

    int number = n;
    int i=0;
    int j=0;
    int m=number;

    int count=0;
    int signal=0; // signal to let other for loop know not to output *
    int powr=1;
    int addto = 0;
    int addto2 = 0;
        do
          {


              if(m == 0)
              {
                  while (i != 8)
                  {
                     m = m/10;
                     i = i + 1;
                     if(m != 0){count = count + 1; break;}
                  }
                  if (!count){break;}
              }

              count = count + 1;
               m = m / 10;

          } while(m != 0);

        // Check if Number is too Big
        if(count >= 5){
            LCD_OutChar(42); // *
            LCD_OutChar(46); // .
            LCD_OutChar(42); // *
            LCD_OutChar(42); // *
            LCD_OutChar(42); // *
            return;
        }


        if(4 != count)
        {
            for(i = 0; i < 5 - count; i = i + 1)
            {

                if (i != 1)
                {
                LCD_OutChar(48); // 0
                }
                else
                {
                LCD_OutChar(46); // .
                signal =  1;
                }


            }

        }


if(count == 4){addto = 1;} // Increment for Asterisk


    m = number;
    for(i = 0; i < count + addto; i = i + 1)
    {

        if( i != 1 || signal == 1)
        {

            for( j=0; j < (count-1) - i + addto2; j = j +1)
                      {
                      m   = m / 10;
                      powr = powr * 10;
                      }

            m = m % 10; // Second Extraction
            m = m + 48;
            LCD_OutChar(m);
            m = m - 48; // Restore Number
            m = m * powr; // Multiply to use as Modulus
            number = number % m; // Get Remaining Numbers
            m = number;
            powr = 1;

        }


        else
        {
            LCD_OutChar(46); // .
            addto2 = 1;
        }



    }
    return;
}

//ADC14.c

#include <stdint.h>
#include "msp432p401r.h"

// P5.5 = A0
void ADC0_InitSWTriggerCh0(void){
	while(REFCTL0&0x0400);					// wait for the reference to be idle before reconfiguring (see REF4 in errata)

	REFCTL0 = 0x0039;						// 1) configure reference for static 2.5V

	//debug code
//  REFCTL0 = 0x003B;					// 1) configure reference for static 2.5V and constant output on P5.6 (see REF7 in errata)
//	P5SEL1 |= 0x40;							// analog mode on P5.6
//	P5SEL0 |= 0x40;
	//end of debug code



	// 15-14 reserved                          00b (reserved)
	// 13    REFBGRDY   bandgap voltage ready   0b (read only)
	// 12    REFGENRDY  reference voltage ready 0b (read only)
	// 11    BGMODE     bandgap mode            0b (read only)
	// 10    REFGENBUSY no reconfigure if busy  0b (read only)
	// 9     REFBGACT   bandgap active          0b (read only)
	// 8     REFGENACT  reference active        0b (read only)
	// 7     REFBGOT    trigger bandgap         0b = no software trigger set
	// 6     REFGENOT   trigger reference       0b = no software trigger set
	// 5-4   REFVSEL    voltage level select   11b = 2.5V
	// 3     REFTCOFF   temperature disable     1b = temperature sensor disabled to save power
	// 2     reserved                           0b (reserved)
	// 1     REFOUT     reference output buffer 0b = reference output not on P5.6 (see also REFBURST in ADC14->CTL1, P5SEL1, and P5SEL0)
	// 0     REFON      reference enable        1b = reference enabled in static mode
	while((REFCTL0&0x1000) == 0);				// wait for the reference to stabilize before continuing (optional, see REF8 in errata)
	ADC14->CTL0 &= ~0x00000002;					// 2) ADC14->ENC = 0 to allow programming
	while(ADC14->CTL0&0x00010000);				// 3) wait for BUSY to be zero
	ADC14->CTL0 = 0x04203310;					// 4) single, SMCLK, on, disabled, /1, 32 SHM
	// 31-30 ADC14->PDIV  predivider,            00b = Predivide by 1
	// 29-27 ADC14->SHSx  SHM source            000b = ADC14->SC bit
	// 26    ADC14->SHP   SHM pulse-mode          1b = SAMPCON the sampling timer
	// 25    ADC14->ISSH  invert sample-and-hold  0b =  not inverted
	// 24-22 ADC14->DIVx  clock divider         000b = /1
	// 21-19 ADC14->SSELx clock source select   100b = SMCLK
	// 18-17 ADC14->CONSEQx mode select          00b = Single-channel, single-conversion
	// 16    ADC14->BUSY  ADC14-> busy              0b (read only)
	// 15-12 ADC14->SHT1x sample-and-hold time 0011b = 32 clocks
	// 11-8  ADC14->SHT0x sample-and-hold time 0011b = 32 clocks
	// 7     ADC14->MSC   multiple sample         0b = not multiple
	// 6-5   reserved                          00b (reserved)
	// 4     ADC14->ON    ADC14-> on                1b = powered up
	// 3-2   reserved                          00b (reserved)
	// 1     ADC14->ENC   enable conversion       0b = ADC14-> disabled
	// 0     ADC14->SC    ADC14-> start             0b = No start (yet)
	ADC14->CTL1 = 0x00000030;					// 5) ADC14->MEM0, 14-bit, ref on, regular power
	// 20-16 STARTADDx  start addr          00000b = ADC14->MEM0
	// 15-6  reserved                  0000000000b (reserved)
	// 5-4   ADC14->RES   ADC14-> resolution       11b = 14 bit, 16 clocks
	// 3     ADC14->DF    data read-back format   0b = Binary unsigned
	// 2     REFBURST   reference buffer burst  0b = reference on continuously
	// 1-0   ADC14->PWRMD ADC power modes        00b = Regular power mode
	ADC14->MCTL[0] = 0x00000180;				// 6) 0 to 2.5V, channel 0
	// 15   ADC14->WINCTH Window comp threshold   0b = not used
	// 14   ADC14->WINC   Comparator enable       0b = Comparator disabled
	// 13   ADC14->DIF    Differential mode       0b = Single-ended mode enabled
	// 12   reserved                            0b (reserved)
	// 11-8 ADC14->VRSEL  V(R+) and V(R-)      0001b = V(R+) = VREF, V(R-) = AVSS
	// 7    ADC14->EOS    End of sequence         1b = End of sequence
	// 6-5  reserved                           00b (reserved)
	// 4-0  ADC14->INCHx  Input channel        0110b = A0, P5.5
	ADC14->IER0 = 0;							// 7) no interrupts
	ADC14->IER1 = 0;							// no interrupts
	P5SEL1 |= 0x20;								// 8) analog mode on A0, P5.5
	P5SEL0 |= 0x20;
	ADC14->CTL0 |= 0x00000002;					// 9) enable
}
// ADC14->IFGR0 bit 0 is set when conversion done
//                  cleared on read ADC14->MEM0
// ADC14->CLRIFGR0 bit 0, write 1 to clear flag
// ADC14->IVx is 0x0C when ADC14->MEM0 interrupt flag; Interrupt Flag: ADC14->IFG0
// ADC14->MEM0 14-bit conversion in bits 13-0 (31-16 undefined, 15-14 zero)
uint32_t ADC_In(void){

    while(ADC14->CTL0&0x00010000);				// 1) wait for BUSY to be zero
	ADC14->CTL0 |= 0x00000001;					// 2) start single conversion
	while((ADC14->IFGR0&0x01) == 0);			// 3) wait for ADC14->IFG0


	return ADC14->MEM[0];						// 4) return result 0 to 16383
}

//SysTick.c
*/
#include <stdint.h>
#include "ADC14.h"
#include "msp432p401r.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

volatile uint32_t ADCvalue;

#define P2OUT2    (*((volatile uint8_t *) 0x40004C03))
volatile uint32_t Counts;

uint8_t * flag;
uint32_t wait_per;
// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(uint32_t period, uint8_t *f, uint8_t interrupts) {


    if (interrupts){
    long sr = StartCritical();
    wait_per = period;

    flag = f;
    ADC0_InitSWTriggerCh0();            // initialize ADC sample P5.5/A0

    Counts = 0;

    SysTick->CTRL = 0;                  // disable SysTick during setup

    SysTick->LOAD = period - 1;         // maximum reload value

    SysTick->VAL = 0;                   // any write to current clears it
    SCB->SHP[3] = (SCB->SHP[3]&0x00FFFFFF)|0x40000000;  // priority 2
    SysTick->CTRL = 0x00000007;         // enable SysTick with interrupts

    EnableInterrupts();

    EndCritical(sr);
    }else{

        SysTick->CTRL = 0;                    // disable SysTick during setup
        SysTick->LOAD = 0x00FFFFFF;           // maximum reload value
        SysTick->VAL = 0;                    // any write to current clears it
        SysTick->CTRL = 0x00000005;           // enable SysTick with no interrupts

    }



}
// Time delay using busy wait.
// The delay parameter is in units of the core clock. (units of 333 nsec for 3 MHz clock)
void SysTick_Wait(uint32_t delay) {
    long sr = StartCritical();
    // method #1: set Reload Value Register, clear Current Value Register, poll COUNTFLAG in Control and Status Register
    if(delay <= 1) {
        // without this step:
        // if delay == 0, this function will wait 0x00FFFFFF cycles
        // if delay == 1, this function will never return (because COUNTFLAG is set on 1->0 transition)
        return;                             // do nothing; at least 1 cycle has already passed anyway
    }

    SysTick->LOAD = (delay - 1);            // count down to zero


    SysTick->VAL = 0;                       // any write to CVR clears it and COUNTFLAG in CSR

    while((SysTick->CTRL&0x00010000) == 0);

    SysTick->LOAD = wait_per - 1;           // maximum reload value


    EndCritical(sr);
}


void SysTick_Handler() {
    ADCvalue = ADC_In();
    flag[0] = 1;
    SysTick->CTRL =  0x00000005;
    P2OUT2 ^= 0x7;
}

uint32_t SysTick_Mailbox() {
    return ADCvalue;
}



// Time delay using busy wait.
// This assumes 3 MHz system clock.
void SysTick_Wait10ms(uint32_t delay){
  uint32_t i;
  for(i=0; i<delay; i++){
    SysTick_Wait(30000);  // wait 10ms (assumes 3 MHz clock)
  }
}
