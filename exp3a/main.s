	;EXPERIMENT 3&4      lab-09/03/22
	AREA Initialise_RESET_setting,DATA,READONLY  ; seting a data area below called Initialise_RESET_setting 
	
	; Vector Table Mapped to Address 0 at Reset
	; Linker requires __Vectors to be exported
	EXPORT __Vectors ; Declaring a symbol called __Vectors referrable by other  source filesources (a label)
__Vectors;label 
	DCD 0x20001000; declaring a data setting stack pointer value
	DCD Reset_Handler; Reset vectoraddr  The program linker needs a reset vector								 	
			


	AREA ThisIsTheCode,CODE,READONLY   ; creating a code area below called ThisIsTheCode
	ENTRY          					   ; declaring where program execution starts
	EXPORT Reset_Handler					
Reset_Handler ; code below this gets executed after every start/reset

;Multiply a number by 5 without using multiply instruction
	MOV r0, #25;  The number to multiply
	MOV r1, #5;  The number of times to multiply
	MOV r2, #0 ;  The result to store
Loop
	ADD r2,r0    ; adding multiple times
	SUBS r1,#1
	BNE Loop
	END;
