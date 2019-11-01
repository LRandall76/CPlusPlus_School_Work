			.thumb

			.data
;OUT			.byte	0				; LED Output
;CYC			.half	XXXXX			; Cycles

			.text
;CYC_ptr		.word	CYC				; Pointer to cycles
;OUT_ptr		.word	OUT				; Pointer to LED Output

P2IN		.word	0x40004C01		; Port 2 Input
P2OUT		.word	0x40004C03		; Port 2 Output
P2DIR		.word	0x40004C05		; Port 2 Direction
P2REN		.word	0x40004C07		; Port 2 Resistor Enable
P2DS		.word	0x40004C09		; Port 2 Drive Strength
P2SEL0		.word	0x40004C0B		; Port 2 Select 0
P2SEL1		.word	0x40004C0D		; Port 2 Select 1


;/////SLIDE 25-31
TA1CTL		.word	0x40000400		; TimerAx Control Register
TA1CCTL0	.word	0x40000402		; Timer_A Capture/Compare Control Register
TA1CCR0		.word	0x40000412		; Timer_A Capture/Compare Register
TA1EX0		.word	0x40000420		; Timer_A Expansion Register

;/////SLIDE 14-18
NVIC_IPR2	.word	0xE000E408		; NVIC interrupt priority 2
NVIC_ISER0	.word	0xE000E100		; NVIC enable register 0		USE FOR IPR8 ALSO
NVIC_IPR8 	.word	0xE000E420
;NVIC_ISER1	.word

			.global asm_main
			.thumbfunc asm_main

			.global TimerA1_ISR
			.thumbfunc TimerA1_ISR


			.global Port2_Init
			.thumbfunc Port2_Init
asm_main:	.asmfunc		; main



			b Port2_Init






    		.endasmfunc


;///////////////////////////////////////////////

Port2_Init:	.asmfunc				; ~DONE

   			;sel 0 & 1
			LDR R1, P2SEL0
			LDRB R10, [R1]
			BIC R10, #0x12
			STRB R10, [R1]

			LDR R1, P2SEL1
			LDRB R10, [R1]
			BIC R10, #0x12
			STRB R10, [R1]


			;P2REN
			;LDR R1, P2REN
			;LDRB R10, [R1]
			;ORR R10, #0x12
			;STRB R10, [R1]


			;LDR R1, P2OUT
			;LDRB R10, [R1]
			;ORR R10, #0x12
			;STRB R10, [R1]

			;P2DIR
			LDR R1, P2DIR
			LDRB R10, [R1]
			BIC R10, R10, #0x12
			ORR R10, #7
			STRB R10, [R1]

				;P1IN
			;LDR R1, P2IN
			;LDRB R10, [R1]
			;ORR R10, #0x12
			;STRB R10, [R1]

			b TimerA1_Init


exit1:




			bx		lr
	        .endasmfunc

;//////////////////////////////////////////////////////////////////

NVIC_Init:	.asmfunc				; NVIC_Init

;ARM BITS
;ENABLE 1
;Set its Priority and enable source in NVIC

;NVIC interrupt priority 2

;(10)

			LDR R2, NVIC_IPR2		;bits 23-21 set priority for TA1CCTL0
			LDR R1, [R2]
			ORR R1, #0x400000		;010000000000000000000000 This is either priority 2 or 3 (010 across 23-21)
			STR R1, [R2]


;Suppose you want to work with TA0. Its IRQ number is 8, so you set bit 8 in NVIC_ISER0.
;0_0
;TA1 handler seems to be IRQ 10 so we need to set 10 in NVIC

;(11)

			LDR R2, NVIC_ISER0		;Setting bit 10 to 1
			LDR R1, [R2]
			ORR R1, #0x400
			STR R1, [R2]


;			LDR R2, NVIC_ISER1
;			LDR R1, [R2]
;			ORR R1, #0x

			b exitNVIC
	        .endasmfunc

;////////////////////////////////////////////////////////////////////

TimerA1_Init:	.asmfunc			; TimerA1_Init

		;Note that a compare event occurs when a timer counts up to TA0CCR0.
		;This clears counter and sets TAIFG flag in TA0CTL register

;1. Ensure timer is stopped (TA0CTL bits 4,5 and stop = 00)			MODE CONTROl
;2. Specify clock source (TA0CTL bits 8,9)							CLOCK SOURCE
;3. Specify divider (TA0CTL bots 6,7)								CLOCK DIVIDER
;4. Arm timer TAIE (TA0CTL bit 1 = 1)								TIMER Interrupt ENABLER
;5. Disconnect I/O pins (TA0CCTL0 pins 14, 15. 00 = no capture)		CAPTURE MODE
;6. Set compare mode (TA0CCTL0 bit 8, 0 = compare)					CAMPARE MODE
;7. Enable interrupt (TA0CCTL0 bit 4, 1 = enable)					Interrupt Enabler
;8. Load period-1 in TA0CCR0
;9. Load 3 bit prescale in TA0EX0
;10. Set priority in correct NVIC priority register
;11. Enable the correct interrupt in the correct NVIC Interrupt enable register
;12. Start the timer (TA0CTL bits 4,5 and up mode = 01)

			CPSID I

		;CONTROL REGISTER
;(1)
			LDR R2, TA1CTL				;Setting bits 4 and 5 to 00
			LDRH R1, [R2]
			BIC R1, #0x30
			STRH R1, [R2]
;(2)
							;USING CLOCK TYPE ACLK- changing bit 8 to 1 and 9 to 0

			ORR R1, #0x100
			BIC R1, #0x200
			STRH R1, [R2]
;(3)
							;Using the largets clock divider to make it slow

			ORR R1, #0xC0
			STRH R1, [R2]
;(4)
							;Putting 1 in the 1 bit to enable the interrupts

			ORR R1, #2
			STRH R1, [R2]


		;CAPTURE/COMPARE CONTROL
;(5)
			LDR R2, TA1CCTL0			;Pushing 00 onto pins 15-14
			LDRH R1, [R2]
			BIC R1, #0xC000
			STRH R1, [R2]


;(6)
			LDR R2, TA1CCTL0			;Pusing 0 onto pin 8
			LDRH R1, [R2]
			BIC R1, #0x100
			STRH R1, [R2]

;(7)
			LDR R2, TA1CCTL0			;Enabling interrupts by putting 1 in bit 4
			LDRH R1, [R2]
			ORR R1, #0x10
			STRH R1, [R2]

									;7-5 change output mode... will probably use just out mode
									;4 enables interrupt flag needs 1b
									;2 changes output of high or low 1b is high


		;CAPTURE/COMPARE	---- THE NUMBER IT COUNTS UP TO (30,000)
;(8)
			LDR R2, TA1CCR0					;Holds data-2 modes
		 	LDRH R1, [R2]					;Compare mode holds data for the comparison to the timer value in TAxR
		 	mov R1, #449					;Capture copies data from TAxR to TAxCCRn
			STRH R1, [R2]




		;Timer Expansion Register

;(9)
			LDR R2, TA1EX0				;Setting second clock divider strength 8
			LDRH R1, [R2]
			ORR R1, #7
			STRH R1, [R2]

									;Divides clock based on bits 2-0 up to a maximum of 8 by pushing that number

;(10)

			b NVIC_Init


exitNVIC:
;(12)

			LDR R2, TA1CTL				;Setting bits 4 and 5 to 01 to count up
			LDRH R1, [R2]
			ORR R1, #0x10
			STRH R1, [R2]

			CPSIE I ; Enables Interrupts

			b exit1
	        .endasmfunc


;////////////////////////////////////////////////////////////////////////

TimerA1_ISR:	.asmfunc			; TimerA1_ISR



	;;acknowledge flag
	LDR R2, TA1CCTL0
	LDRH R1, [R2]
	BIC R1, #1
	STRH R1, [R2]

	LDR R2, TA1CTL
	LDRH R1, [R2]
	BIC R1, #1
	STRH R1, [R2]


	;increment out

	b LED_Out

exitLED:

			.endasmfunc

;/////////////////////////////////////////////////////////////////////////

LED_Out:	.asmfunc				; LED_Out



	LDR R2, P2OUT
	LDRB R1, [R2]
	BFC R1, #3, #29
	ADD R1, #1
	cmp R1, #7
	BGT reset

LED_back:

	LDRB  R3, [R2]
	mvn R4, R1
	BFC R4, #3, #29
	BIC R3, R4
	ORR R3, R1


	STRB R3, [R2]

 	bx lr

reset:

	mov R1, #0
	b LED_back


			.endasmfunc

	        .end









#include <stdint.h>
#include <stdio.h>
#include "msp432p401r.h"

extern void asm_main();
extern void TimerA1_ISR();
extern void Port2_Init();

// Timer A1 ISR
void TA1_0_IRQHandler() {
    TimerA1_ISR();      // Call assembly ISR
}

void main() {
    //asm_main();
    Port2_Init();
    // Main Loop
    while(1){asm_main();}
}


