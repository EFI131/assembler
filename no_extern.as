;.ext file should not exist
MAIN: add r3, LIST
jsr LOOP
LOOP: prn #48
lea STR,r6
inc r6
mov *r6, L3
sub r1,r4
cmp r3, #-6
bne END
add r7, *r6
clr K
sub r1, r2
.entry MAIN
jmp LOOP
END: stop
STR: .string "abcd"
LIST: .data 6, -9
L3: .data -100
K: .data 31