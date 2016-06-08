add r2,r1,#2
mov r1,#0x0F
mov r1,#1
eor r2,r1,#0x0F
sub r1, r2, r3, lsl r4
mul r3,r1,r2
mla r3,r1,r2,r4
andeq r0, r0, r0
