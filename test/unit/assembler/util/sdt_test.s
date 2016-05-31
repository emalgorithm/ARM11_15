ldr r1, =0xf1       ; translate as mov
ldr r1, =0xffff     ; constant address stored in memory
ldr r1, [r2]        ; base r2 
ldr r1, [r2, #0xffa] ; base r2 with offset
ldr r1, [r2, r3, lsl #0x3] ; base r2 with shifted r3 offset
ldr r1, [r2, r3, asr r4] ; base r2 with shifted r3 offset, shift with r4
ldr r1, [r2, -PC, lsl #13] ; base r2 with shifted negative r3 offset

ldr r1, [r2], #0x1516
ldr r1, [PC], -r4, lsr r5
