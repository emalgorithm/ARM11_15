mov r2, #0x1
mov r2, r2, #0x9
ldr r0, =#0x20200000
str r2, [r0, #0x0]
mov r1, #0x1
mov r1, r1, #0x3
str r1, [r0, #0x28]
mov r2, #0xA
label_0:
str r1, [r0, #0x28]
sub r2, r2, #0x1
cmp r2, #0x0
str r1, [r0, #0x1C]
bne label_0
mul r1, r2, r3
andeq r0, r0, r0
