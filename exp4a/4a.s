	;EXPERIMENT 3&4      lab-09/03/22
	; if (a==b)  y = (a * b) + c; else y = 10;


	
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

	MOV R0,#0; y
	MOV R1,#10;a
	MOV R2,#10;b
	MOV R3,#10;c
	CMP R1,R2; a==b 
	MULEQ r0,R1,R2;
	ADDEQ R0,R3;
	MOVNE R0,#10
	END