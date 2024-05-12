    PRESERVE8
    THUMB
    AREA  STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem  SPACE 1024
__initial_sp
    
    AREA  RESET,DATA,READONLY
       EXPORT __Vectors
__Vectors          DCD    __initial_sp
       DCD    solution
       
               AREA   MYCODE,CODE,READONLY
       ENTRY

solution  PROC

	MOV R0, #0
	MOV R10, #10
	MOV R5, #0
	MOV R3, #3         ; NUMBER OF CHARACTERS 
	LDR R2, =var
	;SUB R2, R2, #0x20000000

turnToNum

	LDRB R1, [R2]
	SUB R1, R1, #48
	MUL R0, R0, R10
	ADD R0, R0, R1
	ADD R5, #1
	ADD R2, #1
	CMP R5, R3
	BNE  turnToNum
	
	
	; R0 is ready

    ;MOV R0, #2        ; the number we want to check if it is prime and palindrome   
    MOV R7, #0          ; counter for 10 numbers
    MOV R2, #0          ; the divissor
    LDR R5, =RESULT     ; mem address  
    MOV R10, #10        ; R10 = 10
mainLoop
    MOV R8, #0       
	MOV R6, R0 
reverseLoop
    
    UDIV R1, R6, R10    
    MUL R9, R1, R10
    SUB R2, R6, R9      ; store raminder in R2

    MUL R8, R8, R10  
    ADD R8, R8, R2      ; calculate reversed number and store it in R8

    UDIV R6, R6, R10    ; R6 /= 10
    CMP R6, #0          ; if R6 == 0 then we have calculated reversed number
    BNE reverseLoop

    CMP   R8, R0        ; if it is not palindrome then skip prime checking
    BNE   continue

    MOV R1, #2
primeLoop         
    CMP R0, #2          ; check the number, if it is 2 then we consider it as a prime number
    BEQ storeInMem
	UDIV R3, R0, R1     
    MUL R4, R1, R3 
    CMP R0, R4          ; check if the reminder is zero the it is not prime
    BEQ continue
    ADD R1, #1         
    CMP R1, R0          ; check loop condition
    BLT primeLoop     

storeInMem              ; the number is both palindrome and prime so we stre it in memory
    STR R0, [R5]
    ADD R5, #4          ; add 2 to R5 to point to the next word
	ADD R7, #1

continue
    ADD R0, #1          ; next number in R0 
    CMP R7, #10         ; check loop condition
    BNE mainLoop

endLoop         
    B   endLoop
               ENDP
var              DCB "790", 0 
               AREA MYDATA, DATA, READWRITE
RESULT           DCD 0

                 END
