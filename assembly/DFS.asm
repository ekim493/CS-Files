;;=============================================================
;;  For CS 2110 - Spring 2024
;;=============================================================
;;  Name: Eric Kim
;;============================================================

;;  In this file, you must implement the 'SET_VISITED', 'IS_VISITED', and 'DFS' subroutines.


.orig x3000
    ;; You do not need to write anything here
    LD R6, STACK_PTR

    ;; Pushes arguments (address of node 1, target node 5)
    ADD R6, R6, -1
    AND R1, R1, 0
    ADD R1, R1, 5
    STR R1, R6, 0
    ADD R6, R6, -1
    LD R1, STARTING_NODE_ADDRESS
    STR R1, R6, 0
    JSR DFS
    LDR R0, R6, 0
    ADD R6, R6, 3
    HALT

    STACK_PTR .fill xF000
    STARTING_NODE_ADDRESS .fill x6110
    VISITED_VECTOR_ADDR .fill x4199 ;; stores the address of the visited vector.

;;  SET_VISITED Pseudocode

;; Parameter: The address of the node
;; Updates the visited vector to mark the given node as visited

;;  SET_VISITED(addr node) {
;;      visited = mem[mem[VISITED_VECTOR_ADDR]];
;;      data = mem[node];
;;      mask = 1;
;;      while (data > 0) {
;;          mask = mask + mask;
;;          data--;
;;      }
;;      mem[mem[VISITED_VECTOR_ADDR]] = (visited | mask); //Hint: Use DeMorgan's Law! --> ~(~visited & ~mask)
;;  }

SET_VISITED ;; Do not change this label! Treat this as like the name of the function in a function header
;; Code your implementation for the SET_VISITED subroutine here!
    
;; Most of this isn't needed here but I'll just use it for consistency
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
    
    LDR R0, R5, #4  ; Load argument addr node (R0)
    LDI R1, VISITED_VECTOR_ADDR ; R1 = mem[mem[VVA]] (visited)
    LDR R0, R0, #0  ; R0 = mem[node] (data)
    AND R2, R2, #0
    ADD R2, R2, #1  ; R2 = 1 (mask)
SETWHILE ADD R0, R0, #0
    BRnz SETEND
    ADD R2, R2, R2 ; mask = mask + mask
    ADD R0, R0, #-1 ; data--
    BR SETWHILE
SETEND
    NOT R1, R1
    NOT R2, R2
    AND R0, R1, R2
    NOT R0, R0  ; (visited | mask)
    STI R0, VISITED_VECTOR_ADDR
    
    STR R0, R5, #3 ; load return value into reserved space (not needed)
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


;;  IS_VISITED Pseudocode

;; Parameter: The address of the node
;; Returns: 1 if the node has been visited, 0 if it has not been visited

;;  IS_VISITED(addr node) {
;;       visited = mem[mem[VISITED_VECTOR_ADDR]];
;;       data = mem[node];
;;       mask = 1;
;;       while (data > 0) {
;;           mask = mask + mask;
;;           data--;
;;       }
;;       return (visited & mask) != 0;
;;   }

IS_VISITED ;; Do not change this label! Treat this as like the name of the function in a function header
;; Code your implementation for the IS_VISITED subroutine here!
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
    
    LDR R0, R5, #4  ; Load argument addr node
    LDI R1, VISITED_VECTOR_ADDR ; R1 = mem[mem[VVA]] (visited)
    LDR R0, R0, #0  ; R0 = mem[node] (data)
    AND R2, R2, #0
    ADD R2, R2, #1  ; R2 = 1 (mask)
ISWHILE ADD R0, R0, #0
    BRnz ENDIS
    ADD R2, R2, R2  ; mask = mask + mask
    ADD R0, R0, #-1 ; data --
    BR ISWHILE
ENDIS
    AND R0, R1, R2  ; (visited & mask)
    BRz ISZERO
    AND R1, R1, #0  
    ADD R1, R1, #1  ; If not 0, store true
    BR ENDISALL
ISZERO
    AND R1, R1, #0  ; If 0, store false
    
ENDISALL
    STR R1, R5, #3 ; load return value into reserved space
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



;;  DFS

;; Parameters: The address of the starting (or current) node, the data of the target node
;; Returns: the address of the node (if the node is found), 0 if the node is not found

;;  DFS(addr node, int target) {
;;        SET_VISITED(node);
;;        if (mem[node] == target) {
;;           return node;
;;        }
;;        result = 0;
;;        for (i = node + 1; mem[i] != 0 && result == 0; i++) {
;;            if (! IS_VISITED(mem[i])) {
;;                result = DFS(mem[i], target);
;;            }
;;        }       
;;        return result;
;;  }

DFS ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the DFS subroutine here!
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

;;  SET_VISITED(node);    
    LDR R0, R5, #4  ; Load addr node (R0)
    LDR R1, R5, #5  ; Load target (R1)
    ADD R6, R6, #-1
    STR R0, R6, #0  ; Store addr node (argument)
    JSR SET_VISITED
    ADD R6, R6, #2  ; Return to original stack pointer (also pops return value)
    
;;  if (mem[node] == target) {
;;      return node;
;; 
    LDR R2, R0, #0 ; R2 = mem[node]
    NOT R3, R1
    ADD R3, R3, #1  ; R3 = -target
    ADD R2, R2, R3  ; mem[node] - target
    BRnp AFTERIF
    STR R0, R5, #3  ; Load return value of node
    BR ENDDFS
AFTERIF
    AND R2, R2, #0  ; R2 = 0 (result)
    
;;        for (i = node + 1; mem[i] != 0 && result == 0; i++) {
;;            if (! IS_VISITED(mem[i])) {
;;                result = DFS(mem[i], target);
;;            }
;;        }  
    ADD R3, R0, #1  ; R3 = node + 1 (i)
FOR
    ADD R2, R2, #0  ; Check result == 0
    BRnp ENDFOR
    LDR R4, R3, #0  ; mem[i]
    BRz ENDFOR
    
    ADD R6, R6, #-1
    STR R4, R6, #0  ; Store addr node (argument)
    JSR IS_VISITED
    LDR R0, R6, #0  ; Load return value (R0)
    ADD R6, R6, #2  ; Return to original stack pointer
    ADD R0, R0, #0
    BRnp ENDIF2
    ADD R6, R6, #-1
    STR R1, R6, #0  ; Store target (argument)
    ADD R6, R6, #-1
    STR R4, R6, #0  ; Store mem[i] (argument)
    JSR DFS
    LDR R2, R6, #0  ; Load return value (R2)
    ADD R6, R6, #3  ; Return to original stack pointer

ENDIF2 ADD R3, R3, #1   ; i++
    BR FOR
    
    
    
ENDFOR  STR R2, R5, #3
ENDDFS
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

;; Assuming the graphs starting node (1) is at address x6100, here's how the graph (see below and in the PDF) is represented in memory
;;
;;         0      3
;;          \   / | \
;;            4   1 - 2 
;;             \ /    |
;;              5  -  6
;;

.orig x4199
    .fill 0 ;; visited set will be at address x4199, initialized to 0
.end

.orig x6110         ;; node 1 itself lives here at x6110
    .fill 1         ;; node.data (1)
    .fill x6120     ;; node 2 lives at this address
    .fill x6130     ;; node 3 lives at this address
    .fill x6150     ;; node 5 lives at this address   
    .fill 0
.end

.orig x6120	        ;; node 2 itself lives here at x6120
    .fill 2         ;; node.data (2)
    .fill x6110     ;; node 1 lives at this address
    .fill x6130     ;; node 3 lives at this address
    .fill x6160     ;; node 6 lives at this address
    .fill 0
.end

.orig x6130	        ;; node 3 itself lives here at x6130
    .fill 3         ;; node.data (3)
    .fill x6110     ;; node 1 lives at this address
    .fill x6120     ;; node 2 lives at this address
    .fill x6140     ;; node 4 lives at this address
    .fill 0
.end

.orig x6140	        ;; node 4 itself lives here at x6140
    .fill 4         ;; node.data (4)
    .fill x6100     ;; node 0 lives at this address
    .fill x6130     ;; node 3 lives at this address
    .fill x6150     ;; node 5 lives at this address
    .fill 0
.end

.orig x6100         ;; node 0 itself lives here at x6000
    .fill 0         ;; node.data (0)
    .fill x6140     ;; node 4 lives at this address
    .fill 0
.end

.orig x6150	        ;; node 5 itself lives here at x6150
    .fill 5         ;; node.data (5)
    .fill x6110     ;; node 1 lives at this address
    .fill x6140     ;; node 4 lives at this address
    .fill x6160     ;; node 6 lives at this address
    .fill 0
.end

.orig x6160	        ;; node 6 itself lives here at x6160
    .fill 6         ;; node.data (6)
    .fill x6120     ;; node 2 lives at this address
    .fill x6150     ;; node 5 lives at this address
    .fill 0
.end
 


