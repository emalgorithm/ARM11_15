; Author: Tencho Tenev

ldr r0, =0x20200004

mov r1, #1
lsl r1, #18 ; set output

str r1, [r0]

mov r1, #1
lsl r1, #16 ; LED position

ldr r3, =0x20200028 ; clear position
ldr r4, =0x2020001c ; set position

ldr r5, =0x100000 ; delay parameter


loop$:

mov r0, #0
delay:
    cmp r0, r5
    beq after_delay
    add r0, r0, #1
    b delay

after_delay:

str r1, [r3] ; clear position
mov r0, #0

_delay:
    cmp r0, r5
    beq _after_delay
    add r0, r0, #1
    b _delay

_after_delay:
    
str r1, [r4] ; set position

b loop$

andeq r0, r0, r0
