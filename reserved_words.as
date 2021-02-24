;reserved words
.entry mov
; this comment should be ignored...
.data: add r3, LIST
mov .entry, r3
jsr: prn #48
lea STR,r6
inc r6
mov *r6, jsr
sub r1,r4
cmp r3, #-6
bne .data
add r7, *r6
clr K
sub L3, L3
jmp .data
.data: stop
.string: .string "abcd"
.data: .data 6, -9
.data -100
.string: .data 31
.extern jsr
