;;=============================================================
;;  For CS 2110 - Spring 2024
;;=============================================================
;;  Name: Eric Kim
;;============================================================

;;  In this file, you must implement the 'MOD' and 'GCD' subroutines.

.orig x3000
    ;; You do not need to write anything here
    LD R6, STACK_PTR

    ;; Pushes arguments A and B
    ADD R6, R6, #-2
    LD R1, A
    STR R1, R6, #0
    LD R1, B
    STR R1, R6, #1 
    JSR GCD
    LDR R0, R6, #0
    ADD R6, R6, #3
    HALT

    STACK_PTR   .fill xF000
    ;; You can change these numbers for debugging!
    A           .fill 24
    B           .fill 15


;;  MOD code  
;;  
;;  MOD(int a, int b) {
;;      while (a >= b) {
;;          a -= b;
;;      }
;;      return a;
;;  }

MOD ;; Do not change this label! Treat this as like the name of the function in a function header
    ADD R6, R6, #-1  ;reserve space for return value
    ADD R6, R6, #-1
    STR R7, R6, #0   ;store old return address
    ADD R6, R6, #-1
    STR R5, R6, #0   ; store old frame pointer
    ADD R6, R6, #-1  ; reserve space for local variable
    ADD R5, R6, #0   ; set this slot to be new frame pointer
    ADD R6, R6, #-1
    STR R0, R6, #0   ; Store R0 to preserve
    ADD R6, R6, #-1
    STR R1, R6, #0   ; Store R1 to preserve
    ADD R6, R6, #-1
    STR R2, R6, #0   ; Store R2 to preserve
    ADD R6, R6, #-1
    STR R3, R6, #0   ; Store R3 to preserve
    ADD R6, R6, #-1
    STR R4, R6, #0   ; Store R4 to preserve
    
    LDR R0, R5, #4  ; Load R0 = arugment A
    LDR R1, R5, #5  ; Load R1 = arugment B
    NOT R2, R1
    ADD R2, R2, #1  ; -b
WHILEMOD    ADD R3, R0, R2  ; a - b
    BRn ENDMOD
    ADD R0, R0, R2
    BR WHILEMOD
ENDMOD

    STR R0, R5, #3 ; load return value into reserved space
    LDR R4, R6, #0 ; restore R4
    ADD R6, R6, #1 ; pop old R4
    LDR R3, R6, #0 ; restore R3
    ADD R6, R6, #1 ; pop old R3
    LDR R2, R6, #0 ; restore R2
    ADD R6, R6, #1 ; pop old R2
    LDR R1, R6, #0 ; restore R1
    ADD R6, R6, #1 ; pop old R1
    LDR R0, R6, #0 ; restore R0
    ADD R6, R6, #1 ; pop old R0
    ADD R6, R6, #1 ; pop local variable
    LDR R5, R6, #0 ; load old frame pointer back into R5
    ADD R6, R6, #1 ; pop old frame pointer
    LDR R7, R6, #0 ; load old return address back into R7
    ADD R6, R6, #1 ; pop old return address
    RET


;;  GCD code
;;
;;  GCD(int a, int b) {
;;      if (b == 0) {
;;          return a;
;;      }
;;        
;;      while (b != 0) {
;;          int temp = b;
;;          b = MOD(a, b);
;;          a = temp;
;;      }
;;      return a;
;;  }

GCD ;; Do not change this label! Treat this as like the name of the function in a function header
    ADD R6, R6, #-1  ;reserve space for return value
    ADD R6, R6, #-1
    STR R7, R6, #0   ;store old return address
    ADD R6, R6, #-1
    STR R5, R6, #0   ; store old frame pointer
    ADD R6, R6, #-1  ; reserve space for local variable
    ADD R5, R6, #0   ; set this slot to be new frame pointer
    ADD R6, R6, #-1
    STR R0, R6, #0   ; Store R0 to preserve
    ADD R6, R6, #-1
    STR R1, R6, #0   ; Store R1 to preserve
    ADD R6, R6, #-1
    STR R2, R6, #0   ; Store R2 to preserve
    ADD R6, R6, #-1
    STR R3, R6, #0   ; Store R3 to preserve
    ADD R6, R6, #-1
    STR R4, R6, #0   ; Store R4 to preserve
    
    LDR R0, R5, #4  ; Arugment A
    LDR R1, R5, #5  ; Arugment B
    BRz GCDEND
GCDWHILE ADD R1, R1, #0
    BRz GCDEND
    ADD R2, R1, #0
    
    ADD R6, R6, #-1
    STR R1, R6, #0  ; Store second argument
    ADD R6, R6, #-1
    STR R0, R6, #0  ; Store first argument
    JSR MOD
    
    LDR R1, R6, #0  ; Load return value
    ADD R6, R6, #3  ; Return to original stack pointer
    
    ADD R0, R2, #0
    BR GCDWHILE
    
GCDEND
    STR R0, R5, #3 ; load return value into reserved space
    LDR R4, R6, #0 ; restore R4
    ADD R6, R6, #1 ; pop old R4
    LDR R3, R6, #0 ; restore R3
    ADD R6, R6, #1 ; pop old R3
    LDR R2, R6, #0 ; restore R2
    ADD R6, R6, #1 ; pop old R2
    LDR R1, R6, #0 ; restore R1
    ADD R6, R6, #1 ; pop old R1
    LDR R0, R6, #0 ; restore R0
    ADD R6, R6, #1 ; pop old R0
    ADD R6, R6, #1 ; pop local variable
    LDR R5, R6, #0 ; load old frame pointer back into R5
    ADD R6, R6, #1 ; pop old frame pointer
    LDR R7, R6, #0 ; load old return address back into R7
    ADD R6, R6, #1 ; pop old return address
    RET
.end