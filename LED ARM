			.thumb

			.text					; Puts code in ROM

P1REN		.word	0x40004C06
P1IN			.word	0x40004C00
P1OUT		.word 	0x40004C02
P1DIR			.word 	0x40004C04
P1SEL0		.word 	0x40004C0A
P1SEL1		.word 	0x40004C0C

			.global asm_main
			.thumbfunc asm_main

			.global port1_init
			.thumbfunc port1_init

asm_main:	.asmfunc		; main


			LDR R8, P1OUT
			LDRB R10, [R8]
			BIC R10, #1
			STRB R10, [R8]
			AND R0, #0xFF
			LDR R5, P1IN
			LDRB R6, [R5]

			MVN R7, R6
			MOV R6, R7
			AND R7, #0x02
			AND R6, #0x10



			cmp R7, #0x02
			BEQ rbutton

			cmp R6, #0x10
			BEQ lbutton

			b exit


rbutton:

			LDRB R7, [R8]
			ORR R7, #0x01
			STRB R7, [R8]



			ROR R0, R0, #0x01
			MOV R3, R0
			AND R3, #0x80000000
			CMP R3, #0x80000000
			BEQ rshift




			b exit


rshift
			ORR R0, #0x80
			AND R0, #0xFF

			b exit

lshift:

			ORR R0, #0x01
			AND R0, #0xFF
			b exit

lbutton:


			LDRB R7, [R8]
			ORR R7, #0x01
			STRB R7, [R8]


			;LDR R7, [R8]
			;BIC R7, #0
			;STRB R7, [R8]

			ROR R0, R0, #0x1F
			MOV R3, R0
			AND R3, #0x100
			CMP R3, #0x100
			BEQ lshift





			b exit

exit:

			bx		lr				; return to C program



	        .endasmfunc

port1_init: .asmfunc


	        	;sel 0 & 1
			LDR R1, P1SEL0
			LDRB R10, [R1]
			BIC R10, #0x12
			STRB R10, [R1]


			LDR R1, P1SEL1
			LDRB R10, [R1]
			BIC R10, #0x12
			STRB R10, [R1]

			;P1REN
			LDR R1, P1REN
			LDRB R10, [R1]

			ORR R10, #0x12
			STRB R10, [R1]

			;P1OUT
			LDR R1, P1OUT
			LDRB R10, [R1]
			ORR R10, #0x12
			STRB R10, [R1]

			;P1DIR
			LDR R1, P1DIR
			LDRB R10, [R1]
			BIC R10, R10, #0x12
			ORR R10, #1
			STRB R10, [R1]

			;P1IN
			LDR R1, P1IN
			LDRB R10, [R1]
			ORR R10, #0x12
			STRB R10, [R1]

			bx lr

	        .endasmfunc
	        .end






