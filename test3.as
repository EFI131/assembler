mov #-1, r2
x: .data 23
dec x
inc *r1
mov *r1,*r1
clr r1
A: .data 17
mov A, r1
cmp A,r1
add A,r0
ROUTINE: sub #3, r1
HELLO: .string "hello world"
lea HELLO,    r1
clr r2
not r3
inc r4
C: .data -1
dec C
jmp *r5
bne ROUTINE
red r6
prn r7
jsr ROUTINE
rts
stop