mov r2, #0x5
label_1:
sub r2, r2, #0x1
mov r1, #0xA
label_0:
sub r1, r1, #0x1
cmp r1, r2, #0x0
bne label_0
cmp r2, #0x0
bne label_1
andeq r0, r0, r0
