; non existing labels 
.entry LIST
.extern fn1
; this comment should be ignored...
add r3, LIST
jsr fn1
prn #48
lea STR,r6
inc r6
mov *r6, L3
sub r1,r4
cmp r3, #-6
bne END
add r7, *r6
clr K
sub L3, L3
.entry MAIN
jmp LOOP
.data -100
.data 31