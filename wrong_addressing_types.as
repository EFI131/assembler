;wrong addressing types
.entry LIST
.extern fn1
; this comment should be ignored...
MAIN: add r3, #123
jsr r2
LOOP: prn #48
lea #1,r6
inc #-123
mov *r6, #1
sub r1,r4
cmp r3, #-6
bne r7
add r7, *r6
clr #1
.entry MAIN
jmp #1
END: #1
LIST: .data 6, -9