.MODEL SMALL
.STACK 100H

.DATA
    msg1 DB 'Enter your name: $' 
    msg2 DB 'Enter a number: $'
    msg3 DB 'Game Over :( $'
    strName DB 'Name: $'
    strScore db 'Score: $'
    minus db '- ', '$' 
    plus db '+ ', '$'
    equal db '= ', '$'
    linefeed db 13, 10, "$"
    bufName DW 100 dup('$')
    digit DW 0   
    num DW 0 
    random DW 0
    seed DW 0
    score DW 0
    row DB 0
    col DB 0 
    a DW 0
    c DW 0
    random1 DW 0
    random2 DW 0
    faults DW 0
    userAns DW 0
    correctAns DW 0 
    remainTime DW 0
    opr dw 0 
    SignRow DB  0
    SignColumn DB  0   
    AnswerRow  DB  0
    AnswerColumn DB 0
    cntr DW 0 
    BUF1 DB  8 dup('$')
    FlagEnter DB  0 
    messagePointer DW 0 
    AnswerNumber DW 0 
    Data1 DW  10
    negativeFlag DB 0
    negSign db '-', '$'
    ansSign DB 0
    currRow DB 0
    currColumn DB 0

.CODE 
MAIN PROC
    
    MOV AX, @DATA
    MOV DS, AX

    
    CALL takeInput        ; get name and seed
    CALL showScore        ; show name and score    
    
    MOV Al, 5
    MOV row, Al
    CALL setCursor        ; set cursor for asking question
    
    
    
  mainLoop:
    
    
    CALL question         ; ask a question 
    MOV AX, random1
    MOV BX, random2
    
    MOV CX, 0
    CMP opr,CX            ; to see if operation is adding 
    JNE adding

    SUB AX, BX
    MOV correctAns, AX    ; calculate the answer
    JMP ansfound
  adding:
  
    ADD AX, BX
    MOV correctAns, AX  
  ansfound:
    
      
    mov ah, 03h           ; Get current cursor position
    mov bh, 0             ; Page number 0
    int 10h
    mov row, dh           ; Store the cursor position
    mov col, dl           
    mov AnswerRow,dh      ; store the cursor position to print the equation
    mov AnswerColumn,dl
    
    MOV AX, 1             
    MOV cntr, AX          ; set counter to 1 for timer
    MOV SI,OFFSET BUF1    ; buffer to store the user input answer                 
    MOV AL,0
    MOV FlagEnter,AL      ; set flag when user press enter it sets to 1 else 0
    MOV negativeFlag,AL   ; set flag when user enter negative number
    CALL GetCursorSign    ; get the cursor position of signs and set it on next line of equation
    

  AnswerLoop:             ; loop for inputting user's answer

    CALL  ColoredSign     ; function to print time line   
    CALL  InputAnswer     ; function to input number from user
    MOV   DL,FlagEnter        
    CMP   DL,1            ; if user press enter go to exit3 label
    JE    exit3       
    MOV   CX,cntr         
    CMP   CX,20       
    JA    callTimeout     ; if timer ends, call function timeout 
    LOOP  AnswerLoop   

  callTimeout: 

    CALL timeOut
    JMP  cont

  exit3:                  ; calculate the remain time for user's score
    
    MOV BX,21 
    SUB BX,CX      
    SUB BX,2
    MOV remainTime,BX 
    XOR BX,BX        
    CALL convertToNumber  ; call this function to convert the input number from ascii codes to number
    
    MOV CX, AnswerNumber
    MOV userAns, CX       ; store user answer in userAns
    
    MOV AX, correctAns    ; check the absolute value
    CMP AX, userAns
    JNE wrong
                          ; check sign
    MOV DL, ansSign
    CMP DL, negativeFlag
    JNE wrong
    
    
    CALL trueAns
    JMP cont
  wrong: 
     CALL falseAns
  cont:
  
    MOV CX, 3             ; check the numbers of faults
    CMP faults, CX
    JE  gameOver
    
    
    ;\n
    CALL GetCurrCursor    ; get current position of cursor
    
    CMP  currRow,23       ; if cursor's current row is 23 , we need to scroll up
    JNE  continueLoop
    CALL Scrolling        ; scroll 2 lines up
    DEC  AnswerRow
    DEC  SignRow
    CALL SetCurrCursor    ; after scrolling up set back current cursor
    MOV  currColumn,00
    JMP mainLoop
    
      
  continueLoop:           ; if cursor's current row is not 23 , enter two lines to write new question
    mov ah, 09
    mov dx, offset linefeed
    int 21h
    mov ah, 09
    mov dx, offset linefeed
    int 21h  
    JMP mainLoop
    
    
    
  gameOver:               ; clear entire screen 
    
    CALL FinishScrolling 
    CALL showScore        ; show user's final score  
    MOV AH,02
    MOV BH,00
    MOV DH,00
    MOV DL,00
    INT 10H
    MOV AH, 09H
    MOV DX, OFFSET msg3   ;write game over:(
    INT 21H  
    
    MOV AH, 4CH           ; exit
    INT 21H
MAIN ENDP    

;///////////////////////////// 

takeInput PROC  
    PUSH AX
    PUSH BX
    PUSH CX 
    PUSH DX
      
        ;read name:
    MOV AH, 09H
    MOV DX, OFFSET msg1        ; print first msg to ask name
    INT 21H
    MOV AH, 0AH
    MOV DX, OFFSET bufName
    INT 21H                    ; read and store the name
    
    ;\n
    mov ah, 09
    mov dx, offset linefeed
    int 21h 

    
    MOV AH, 09H               ;read number & store it in seed:
    MOV DX, OFFSET msg2
    INT 21H
    CALL readint
    MOV AX, num
    MOV seed, AX              ; initializing seed with entered number
    ;CALL print  
    
    POP DX                    ; recovering registers
    POP CX
    POP BX
    POP AX
    RET
takeInput ENDP

;/////////////////////////////

readint PROC  
    PUSH AX
    MOV num, 0 
    
    readintLoop:
        MOV AH, 01H
        INT 21H      
        CMP AL, ' '
        JE  next
        MOV AH, 0
        SUB AL, 48               ; Convert ASCII to decimal
        MOV digit, AX
        MOV AX,10
        MUL num
        ADD AX, digit
        MOV num, AX
        JMP readintLoop
    next:    
    POP AX
    RET
readint ENDP       

;///////////////////////////// 

question PROC  
    PUSH AX
    PUSH BX
    PUSH CX 
    PUSH DX
    
    CALL randgenerator         ; set the operation
    MOV AX, random
    MOV DX, 0
    MOV BX, 2
    DIV BX
    CMP DX, 0
    JNE lable                  ; generate a random number & calculate its reminder for (div 2) to set operation
    MOV opr,0
    JMP else
  lable:
    MOV opr,1   
  else:
                               ; generating 2 random numbers in AX, BX
    CALL randgenerator
    MOV AX, random
    MOV seed, AX 
    CALL randgenerator
    MOV BX, random
    mov random1, ax            ; save them in random1, random2 
    mov random2, bx
                    
    CALL print                 ;PRINT FIRST NUMBER
    
    MOV CX, 0
    CMP opr,CX
    JNE jam                    ; OPR != 0 -> PLUS
    
  menha:  
    mov ah, 9
    lea dx, minus
    int 21h                    ; print operation : -
    JMP printsecondnum
  jam:
    mov ah, 9
    lea dx, plus
    int 21h                    ; print operation : +
    
  printsecondnum:  
    MOV AX, BX  
    CALL print                 ; print second number
    
    mov ah, 9
    lea dx, equal
    int 21h                    ; question asked 
                    
                    
    MOV CL, 0                  ; set the sign of the answer as positive
    MOV ansSign, CL 
    
    MOV AX, random1
    MOV BX, random2
    
    MOV CX, 1
    CMP opr,CX 
    JE  dontswap
    CMP AX, BX
    JGE dontswap 
    
    MOV CL, 1
    MOV ansSign, CL           ; set the sign of the answer as negative
    
    MOV AX, random1
    MOV BX, random2
    MOV random1, BX
    MOV random2, AX           ; swaping to get the absolute answer
    
  dontswap:
           
    POP DX                    ; recovering registers
    POP CX
    POP BX
    POP AX
    RET
question ENDP
   
;/////////////////////////////   
   
showScore PROC 
    
    PUSH AX
    PUSH BX
    PUSH CX 
    PUSH DX
    
    MOV AH,02
    MOV BH,00 
    MOV DH,00
    MOV DL,67                  ; change the cursor position to the corner
    INT 10H

    MOV AH, 09H
    MOV DX, OFFSET strName
    INT 21H
    MOV DX, OFFSET bufName+2
    INT 21H                    ; to show the name of the user
    
    MOV AH,02
    MOV BH,00  
    MOV DH,01
    MOV DL,67
    INT 10H                    ; change the cursor position
    
    MOV AH, 09H
    MOV DX, OFFSET strScore    ; to show the score of user
    INT 21H
    
    MOV AX,score
    CALL Print 
    
    POP DX                    ; recovering registers
    POP CX
    POP BX
    POP AX
     
    RET
    
showScore ENDP 

;///////////////////////////// 

trueAns PROC  
    PUSH AX
    PUSH BX
    PUSH CX 
    PUSH DX

               
    MOV BX, remainTime        ; remainTime time is in BX
    ADD score, BX
    
    MOV AH,02
    MOV BH,00  
    MOV DH,01
    MOV DL,74
    INT 10H
    
    MOV AX,score
    CALL Print        ; score updated
    
    MOV AH,02
    MOV BH,00  
    MOV DH,row
    MOV DL,col
    INT 10H
    
                              ; write the answer in green
    mov ah, 09h               ; Set the color attribute
    mov al, 0                 ; Character to print (null character)
    mov bh, 0                 ; Page number 0
    mov bl, 0Ah               ; Green foreground color attribute
    mov cx, 10                ; Number of repetitions
    int 10h 
    
    mov Dh, 0
    CMP negativeFlag, Dh 
    JE  notnegative
    MOV AH, 2                 ; AH = 2 -> printing a char
    MOV DL, '-'               ; print a ' ' after number
    INT 21H
   notnegative: 
    MOV AX, userAns
    CALL print
    
    POP DX                    ; recovering registers
    POP CX
    POP BX
    POP AX    
    RET
trueAns ENDP       

;///////////////////////////// 

falseAns PROC  
    PUSH AX
    PUSH BX
    PUSH CX 
    PUSH DX
    
    MOV AX, 1
    ADD faults, AX            ; user has made a mistake so we update faults
    
    MOV AH,02
    MOV BH,00  
    MOV DH,row
    MOV DL,col
    INT 10H
    
                              ; write the user answer in red
    mov ah, 09h               ; Set the color attribute
    mov al, 0                 ; Character to print (null character)
    mov bh, 0                 ; Page number 0
    mov bl, 0Ch               ; red foreground color attribute
    mov cx, 10                ; Number of repetitions
    int 10h
    
    mov Dh, 0
    CMP negativeFlag, Dh 
    JE  notnegative2
    MOV AH, 2                 ; AH = 2 -> printing a char
    MOV DL, '-'               ; print a ' ' after number
    INT 21H 
  notnegative2:  
    MOV AX, userAns
    CALL print
    
    MOV AH, 2                 ; AH = 2 -> printing a char
    MOV DL, ' '               ; print a ' ' after number
    INT 21H
    
                              ; write the correct answer in yellow
    mov ah, 09h               ; Set the color attribute
    mov al, 0                 ; Character to print (null character)
    mov bh, 0                 ; Page number 0
    mov bl, 0Eh               ; yellow foreground color attribute
    mov cx, 10                ; Number of repetitions
    int 10h
    
    mov Dh, 0
    CMP ansSign, Dh 
    JE  notnegative3
    MOV AH, 2                 ; AH = 2 -> printing a char
    MOV DL, '-'               ; print a ' ' after number
    INT 21H
  notnegative3:  
    MOV AX, correctAns
    CALL print
 
    
    POP DX                    ; recovering registers
    POP CX
    POP BX
    POP AX    
    RET
falseAns ENDP 

;///////////////////////////// 
 
timeOut PROC 
    PUSH AX
    PUSH BX 
    PUSH CX
    PUSH DX
    
    MOV AX, 1
    ADD faults, AX            ; user has made a mistake so we update faults
  
    mov ah, 09h               ; Set the color attribute
    mov al, 0                 ; Character to print (null character)
    mov bh, 0                 ; Page number 0
    mov bl, 0Eh               ; yellow foreground color attribute
    mov cx, 10                ; Number of repetitions
    int 10h
    
    
    mov Dh, 0
    CMP ansSign, Dh 
    JE  notnegative4
    MOV AH, 2                 ; AH = 2 -> printing a char
    MOV DL, '-'               ; print a ' ' after number
    INT 21H
  notnegative4:  
    MOV AX, correctAns
    CALL print
    

    POP DX                    ; recovering registers
    POP CX
    POP BX
    POP AX
    RET
timeOut ENDP

;///////////////////////////// 
 
setCursor PROC 
    PUSH AX
    PUSH BX 
    PUSH DX
    
    MOV AH,02
    MOV BH,00  
    MOV DH,row
    MOV DL,00
    INT 10H 
    

    POP DX                    ; recovering registers
    POP BX
    POP AX
    RET
setCursor ENDP

;/////////////////////////////                    
                    
randgenerator PROC  
    PUSH AX
    PUSH CX 
    PUSH DX
    
    
    MOV AH,2CH                 ; To get System Time
    INT 21H
    MOV AX, 0
    MOV AL,DH                  ; sec 
    MOV a, AX 
          
    
    MOV AH,2CH                 ; To get System Time
    INT 21H
    MOV AX, 0
    MOV AL,CL                  ; min      
    MOV c, AX 
    
    
    MOV AX, a
    MUL seed
    ADD AX, c 
    MOV CX, 10000              ; Xi+1 = a * Xi + c mod 10000
    DIV CX                     ; create random number based on LCG algorithm
    
    
    
    MOV random, DX   
     
    POP DX                    ; recovering registers
    POP CX
    POP AX
    RET
randgenerator ENDP 

;/////////////////////////////  
 
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

;/////////////////////////////////

GetCursorSign PROC            ; get the cursor sign from row and col variables and store it to signRow and signColumn 

     PUSH AX
     PUSH BX
     PUSH CX
     PUSH DX
     
     MOV AL,row
     ADD AL,1
     MOV BL,00
     MOV SignRow,AL
     MOV SignColumn,BL

     POP  DX                  ; recovering registers
     POP  CX
     POP  BX
     POP  AX
     
     RET

GetCursorSign ENDP  

;/////////////////////////////////

ColoredSign PROC              ; print colored # to show timeline of the game
       
    PUSH AX
    PUSH BX
    PUSH CX 
    PUSH DX 
    
    
    CALL SetCursorSign        ; set cursor position to print #
    
    MOV  CX,cntr              ; 7 green , 7 yellow , 6 red = 20 s
    CMP  CX,8
    JB   green
    CMP  CX,15
    JB   yellow    
       
red:                          ; print red #
    
    MOV AH,09
    MOV BH,00
    MOV AL,23H
    MOV CX,1
    MOV BL,0Ch
    INT 10H
    JMP exit
   
green:                        ; print green #
    
    
    MOV AH,09
    MOV BH,00
    MOV AL,23H
    MOV CX,1
    MOV BL,0Ah
    INT 10H 
    JMP exit

yellow:                       ; print yellow #
    
    
    MOV AH,09
    MOV BH,00
    MOV AL,23H
    MOV CX,1
    MOV BL,0Eh
    INT 10H
    JMP exit    
       
exit:                        ; add column 2 to print next #
    
    INC  cntr 
    MOV  CL,2
    ADD  SignColumn,CL
    CALL Delay               ; wait 1 sec to make real time line
    POP DX                   ; recovering registers
    POP CX
    POP BX
    POP AX
    
    RET
        
ColoredSign ENDP

;//////////////////////

SetCursorSign PROC           ; set cursor for printing #
    
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    
    MOV  AH,02
    MOV  BH,00
    MOV  DH,SignRow
    MOV  DL,SignColumn
    INT  10H
    
                             ; recovering registers
    POP DX
    POP CX
    POP BX
    POP AX
    
    RET
    
SetCursorSign ENDP

;//////////////////////

Delay PROC                   ; wait 1 sec each time
    
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    
    ;mov al, 0
    MOV CX,0DH
    MOV DX,2000H  
    MOV AH,86H
    INT 15h 
    
                             ; recovering registers
    POP DX
    POP CX
    POP BX
    POP AX
    
    RET
    
Delay ENDP

;///////////////////////

InputAnswer PROC             ; inputting user's answer
    
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    
    CALL SetCursorAns        ; set cursor to print user's answer
    
    MOV AH,01H               ; this function : don't clean buffer but not wait for key press
    INT 16H 
    JZ  exit2                ; if user don't press the key jump to exit 
    
    MOV AH,00H               ; this function : clean the buffer but wait for user to press the key
    INT 16H                  ; use both of this functios to have clean buffer each time and don't wait for pressing key
    
    CMP AL,13                ; interrupt 16 , the key that user presses stores in al register
    JE  exitEnter            ; if the key has ascii 13 , it is enter key
    
    CMP AL,8
    JE  backSpaceExit        ; if the key has ascii 8 , it is backspace key
    
    MOV AH,02                ; print the key on screen
    MOV DL,AL
    INT 21H
    INC AnswerColumn
    MOV [SI],AL              ; store the key in buffer
    INC SI
    
    JMP exit2 
    

backSpaceExit:
    
    DEC  SI                  ; store $ instead of the char to edit it
    MOV  AL,'$'
    MOV  [SI],AL 
    
    DEC  AnswerColumn        ; print space to edit the digit
    MOV  AL,' '
    MOV  AH,02
    MOV  DL,AL
    INT  21H
    
    POP DX                   ; recovering registers
    POP CX
    POP BX
    POP AX
    
    RET 
    
exitEnter:  
    
    MOV FlagEnter,1          ; set flag enter to 1

    POP DX
    POP CX                   ; recovering registers
    POP BX
    POP AX
    
    RET 
    
exit2: 

    POP DX
    POP CX                   ; recovering registers
    POP BX
    POP AX
    
    RET 

InputAnswer ENDP

;/////////////////////////////

SetCursorAns PROC             ; set cursor to print user's answer
    
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    
    MOV  AH,02
    MOV  BH,00
    MOV  DH,AnswerRow
    MOV  DL,AnswerColumn
    INT  10H
    
                              ; recovering registers
    POP DX
    POP CX
    POP BX
    POP AX
    
    RET
    
SetCursorAns ENDP

;////////////////////

convertToNumber PROC          ; read char of user's key press from buffer and convert it to number
       
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI
    
    MOV AX,0                  ; make ansnumber 0 each time
    MOV AnswerNumber,AX
       
    MOV SI,OFFSET BUF1 
    XOR DX,DX
    
    MOV CL,[SI]               ; read first char
    CMP CL,2DH
    JE  negativeAnswer        ; if it is "-" , the number is negative so jump to label
       
PositiveAnswer:
                              ; loop to convert digits to number , if char is '$' , exit --> end of the buffer and user's answer is $
    MOV CL,[SI]
    MOV [SI],'$'              ; after read each char , put '$' again to clear buffer for next input
    CMP CL,36
    JE  exit4
    
    SUB CL,'0'                ; subtract ascii of each char from ascii of '0' to have the digit
    
    
    MUL DATA1                 ; ax=ax*10
    
    ADD AX,CX                 ; ax=ax+cx(digit)
    
    INC SI
    
    JMP PositiveAnswer 
       
negativeAnswer: 
                              ; if number is negative increase pointer and set neaativeFlag to 1
    INC SI
    MOV BL,1
    MOV negativeFlag,BL 
     
NegativeLoop:                 ; same as positive number

    MOV CL,[SI]
    MOV [SI],'$'
    CMP CL,36
    JE  exit4
    
    SUB CL,'0'
     
    MUL DATA1
    
    ADD AX,CX
    
    INC SI
    
    JMP NegativeLoop
 
    
exit4: 

    MOV AnswerNumber,AX       ; store number in AnswerNumber
    POP SI
    POP DX
    POP CX                    ; recovering registers
    POP BX
    POP AX
    
    RET  
    
convertToNumber ENDP 
;///////////////////////////
Scrolling PROC
    
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
      
   
    MOV AH, 06   ; Function 0x06 - Scroll up window
    MOV AL, 02   ; Number of lines to scroll
    MOV BH, 07   ; Display attribute for blank lines (color)
    MOV CH, 00   ; Top line of scrolling region (0-based index)
    MOV CL, 00   ; Bottom line of scrolling region (0-based index + number of lines to scroll - 1)
    MOV DH, 24   ; Starting row position
    MOV DL, 79   ; Starting column position
    INT 10H      ; Call BIOS interrupt 0x10
   

    
    CALL showScore  ; in each scroll , call show score to have name and score on screen
    
    POP  DX
    POP  CX
    POP  BX
    POP  AX 
     
    RET
     
Scrolling ENDP 

;//////////////////

GetCurrCursor PROC   ; get current position of cursor
    
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    
    MOV AH,03
    MOV BH,00
    INT 10H
    
    MOV currRow,DH      ; store the current position in two variabales
    MOV currColumn,DL
    
    
    POP  DX
    POP  CX            ; recovering registers
    POP  BX
    POP  AX
    
    RET
    
GetCurrCursor ENDP 

;///////////////

SetCurrCursor PROC     ; after scrolling , set new cursor position
    
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    
    MOV AL,currRow
    
    
    MOV AH,02
    MOV BH,00
    MOV DH,AL
    MOV DL,00
    INT 10H
    
    
    POP  DX            ; recovering registers
    POP  CX
    POP  BX
    POP  AX
    
    RET
    
SetCurrCursor ENDP    
    
;///////////////////

FinishScrolling PROC  ; end  of the game , clear entire screen
    
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX 
    
    MOV AH, 06   ; Function 0x06 - Scroll up window
    MOV AL, 00   ; Number of lines to scroll
    MOV BH, 07   ; Display attribute for blank lines (color)
    MOV CH, 00   ; Top line of scrolling region (0-based index)
    MOV CL, 00   ; Bottom line of scrolling region (0-based index + number of lines to scroll - 1)
    MOV DH, 24   ; Starting row position
    MOV DL, 79   ; Starting column position
    INT 10H
    
    
    POP  DX       ; recovering registers
    POP  CX
    POP  BX
    POP  AX
    
    RET
    
    
FinishScrolling ENDP    

 END MAIN 