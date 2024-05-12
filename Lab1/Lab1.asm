.MODEL SMALL

.DATA  
    NUMBERS DW 10 DUP(?)      ; Array to store the prime palindrome numbers
    FLAG1 DB 0                ; is prime
    FLAG2 DB 0                ; is palindrome
    REVERSE DW 0              ; REVERSE is reverse of original number
    REMAIN DW 0               ; REMAIN is used to keep remain of div
    DATA1 DW 10               ; DATA1 is used to divide the number to 10
    QOUT DW 0                 ; QOUT is used to keep the divisor in each iteration
    ORIGINAL DW 0             ; original number to save the ax   


.CODE  

MAIN PROC

    MOV AX, @DATA
    MOV DS, AX

    MOV SI, OFFSET NUMBERS    ; a pointer to array of numbers
    MOV CX, 10                ; counter for numbers in array
    MOV AX, 2                 ; check numbers from 2 if they are both prime and palindrome
    
    ; find numbers
    
CHECK_PRIME:                
                
    CALL IsPalindrome         
    CMP FLAG2, 0              ; if FLAG2 == 0 then it is not palindrome
    JE continue      
    
    CALL IsPrime
    CMP FLAG1, 0              ; if FLAG1 == 0 then it is not prime
    JE continue
    
    MOV [SI], AX              ; AX is both prime & palindrome so we store it in array
    INC SI                  
    INC SI                    ; 2 INC because AX is 16 bit
    DEC CX
    
continue:
    INC AX                    ; to check the next number -> AX = AX + 1
    CMP CX, 0                 ; check if we found 10 numbers
    JNE CHECK_PRIME
    
    ; print numbers

    MOV SI, OFFSET NUMBERS    ; set SI again 
    MOV CX, 10              

Print_numbers:
    MOV AX, [SI]              ; moving array content in AX 
    CALL Print                ; print the number in the AX 
    INC SI                    ; 2 INC because AX is 16 bit
    INC SI                        
    LOOP Print_numbers      

    MOV AH, 4CH               ; triggers the interrupt to communicate with the operating system
    INT 21H

MAIN ENDP                           
           
;/////////////////////////////////////                     

isPrime PROC
        PUSH AX               ; push registers to be recaverd later
        PUSH BX
        PUSH CX
        PUSH DX
        
        MOV CX, AX            ; a copy of AX 
        MOV BX, 2             ; loop counter
        MOV DX, 0             ; DX = 0 to prevent overflow in division
        
        CMP AX, 2             ; check if AX is 2 then it is prime
        JE  PRIME             
    
    L1:
        DIV BX                ; AX / BX
        CMP DX, 0             ; if reminder(DX) is 0 then it is not prime
        JE  NOT_PRIME

        INC BX                ; BX = BX + 1
        MOV DX, 0             ; DX = 0 to prevent overflow in division 
        
        MOV AX, CX            ; RECOVER AX
        CMP BX, CX          
        JNE L1                ; check end of the loop
    
    PRIME:
        MOV FLAG1, 1          ; FLAG1 = 1 means AX is a prime number
        JMP EXIT
    
    NOT_PRIME:
        MOV FLAG1, 0          ; FLAG1 = 0 means AX is not a prime number
    
    EXIT: 
        POP DX
        POP CX
        POP BX
        POP AX                ; recovering registers
        RET
    
isPrime ENDP  

;/////////////////////////////////////  

IsPalindrome PROC 
        PUSH AX                   ;push the registers
        PUSH BX
        PUSH CX
        PUSH DX
    
    mov ORIGINAL, AX              ;to save the original number for comparing  
    mov REVERSE,0                 ;to make reverse of number zero for each number to calculate the reverse in reverse loop
    reverse_loop:                 ;reverse loop for reversing the number
        xor DX, DX                ;make dx zero
        xor BX,BX                 ;make bx zero
        div DATA1                 ;divide number to ten --> ax=ax/10 dx=ax%10
        mov QOUT, AX              ;keep ax in QOUT
        mov REMAIN, DX            ;keep dx in REMAIN
        mov AX, REVERSE           ;put reverse in to the ax , at first reverse=0
        mul DATA1                 ;ax=ax*10 
        mov REVERSE,AX            ;move ax to reverse --> reverse=reverse*10
        mov BX, REMAIN            ;move remain to bx --> bx=remain
        add REVERSE,BX            ;add bx to reverse --> reverse=reverse*10+remain
        mov AX,QOUT               ;move qout to ax for next iteration
        cmp AX, 0                 ;if ax==0 exit the loop , if not continue 
        jnz reverse_loop

    
    mov AX, ORIGINAL              ; Check if num is equal to the reversed
    cmp AX, REVERSE
    je is_palindrome              ; if it equals to the original , it is palindrome
    

    not_palindrome: 
        MOV FLAG2, 0              ;if it is not palindrome make the flag2 zero
        JMP EXIT2                 ;jump to exit
    

    is_palindrome:
        mov FLAG2,1               ;if it is palindrome make the flag2 one
        JMP EXIT2                 ;jump to exit
             
     exit2:                       ;pop the registers and return
        POP DX
        POP CX
        POP BX
        POP AX
        RET        
                                
isPalindrome ENDP

;/////////////////////////////////////

Print PROC
    PUSH AX
    PUSH BX
    PUSH CX 
    PUSH DX                   ; push registers to be recaverd later
    
    MOV DX, 0                 ; DX = 0
    MOV BX, 10                ; BX = 10
    MOV CX, 0                 ; CX = 0 -> counter loop
  
  LOOP1:                      ; AX = AX / BX(10) 
    DIV BX                    ; push the first digit of the number in AX 
    PUSH DX
    MOV DX, 0                 ; DX = 0
    MOV AH, 0                 ; AL contains quotient
    INC CX
    CMP AX, 0
    JNE LOOP1                 ; jump not equal (a==0)
    MOV AH, 2
    
  LOOP2:                      
    POP DX                    ; recover DX from stack
    ADD DX, 48                ; add 48 to AX for fixing ascii code
    INT 21H 
    LOOP LOOP2 
    
    MOV AH, 2                 ; AH = 2 -> printing a char
    MOV DL, ' '               ; print a ' ' after number
    INT 21H
    
    POP DX                    ; recovering registers
    POP CX
    POP BX
    POP AX
    RET          
Print ENDP

END MAIN                                  