	;EXPERIMENT 3&4      lab-09/03/22
	; if (a > b) y = 1; else y = [(a / 2) * b] + c;


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
	MOV R0,#5;a
	MOV R1,#10;b
	MOV r2,#300;c
	MOV R5,#2; for udiv
	CMP R0,R1
	BLT lessthan
	UDIV R0,R5   ; a/2
	MLA R5,R0,R1,R2 ; y=r0*r1+r2
	B exit
lessthan
	MOV R5,#1 ; Y
exit	
	END