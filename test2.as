
.extern Some7ABEL
.extern function
; this comment should be ignored...
MAIN: add r3, LIST
cmp #1, #-1
add #15, r4
lea LIST,r6
clr *r7
not K
inc *r7
dec *r7
jmp *r5
bne Some7ABEL
red r1
prn r4
jsr function
rts
END: stop
STR: .string "abcd"
LIST: .data 6, -9
K: .data 31