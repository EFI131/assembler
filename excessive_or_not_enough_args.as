;excessive/not enough arguments 
.entry LIST
.extern fn1
; this comment should be ignored...
MAIN: add r3, LIST, r6
jsr
LOOP: prn #48, #49
lea LIST,r6 , #1
inc r6, #1
mov *r6
sub r1
cmp r3
bne
add *r6
clr *r1
sub L3
.entry MAIN
jmp LOOP
END: stop #1
LIST: .data 6, -9
.data -100
K: .data 31
.extern L3