;;=============================================================
;;  For CS 2110 - Spring 2024
;;=============================================================
;;  Name: Eric Kim
;;============================================================

;;  In this file, you must implement the 'isPalindrome' subroutine.
 

.orig x3000
    ;; You do not need to write anything here
    LD R6, STACK_PTR

    ;; Pushes arguments (word addr and len)
    ADD R6, R6, -2
    LEA R0, STRING
    LD R1, LENGTH
    STR R0, R6, 0
    STR R1, R6, 1
    JSR IS_PALINDROME
    LDR R0, R6, 0
    ADD R6, R6, 3
    HALT
    STACK_PTR .fill xF000
    LENGTH .fill 6                 ;; Change this to be length of STRING
    STRING .stringz "rottor"	       ;; You can change this string for debugging!


;;  IS_PALINDROME **RECURSIVE**
;;

IS_PALINDROME ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the isPalindrome subroutine here!
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
    
;;  isPalindrome(word (addr), len) { 
;;      if (len == 0 || len == 1) {
;;          return 1;
;;      } else {
;;          if (word[0] == word[len - 1]) {
;;              return IS_PALINDROME(word + 1, len - 2);
;;          } else { 
;;              return 0;
;;          }
;;      }
;;  }
;;

    BRz RET1
    ADD R2, R1, #-1
    BRz RET1
    LDR R2, R0, #0  ; R2 = word[0]
    ADD R3, R0, R1
    LDR R3, R3, #-1  ; R3 = word[len - 1]
    NOT R3, R3
    ADD R3, R3, #1
    ADD R2, R2, R3  ; R2 = word[0] - word[len - 1]
    BRz RETPAL
    AND R2, R2, #0  ; Set R2 = return = 0
    BR END
    
    
RET1 AND R2, R2, #0
    ADD R2, R2, #1
    BR END
    
RETPAL ADD R6, R6, #-1
    ADD R1, R1, #-2
    STR R1, R6, #0  ; Load second argument
    ADD R6, R6, #-1
    ADD R0, R0, #1
    STR R0, R6, #0  ; Load first argument
    JSR IS_PALINDROME
    LDR R2, R6, #0  ; Load return value
    ADD R6, R6, #3  ; Return to original stack
    
END
    STR R2, R5, #3 ; load return value into reserved space
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