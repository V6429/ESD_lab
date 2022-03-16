	;EXPERIMENT 3&4      lab-09/03/22
	;3b Find the sum of the first n natural numbers.
	
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
	MOV R0,#10; The N
	MOV R2,#2; udiv does not take immidiate data 
	MOV R3,R0 ; N
	ADD R3,#1 ; N+1
	MUL R3,R3,R0
	UDIV R3,R2
	END
		