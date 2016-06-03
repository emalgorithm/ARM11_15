ldr r0,=0x20200000                        ;r0 is Base Memory Address

mov r1, 1
lsl r1, #18                               ;Pattern to set 16th Pin to Output

mov r2, 1
lsl r2, #16                               ;Pattern to set 16th Pin On/Off

str r1, [r0, #4]                          ;Set 16th Pin to Output

delay:
    
    mov r4, #0
    
    delay_loop:
        add r4, r4, #1
        cmp r4, #0x40000000               ;Loop up to nearly 1 Billion
        bne delay_loop
        
    cmp r3, 0
    
    beq clear
    b set

inf_loop:
                 ;r3 is used in delay to know where to return after the function
    mov r3, 0    ;A value of 0 means that delay branches to clear                    
    b delay
    
    clear:
    str r2, [r0, #28]
    
    mov r3, 1    ;A value of 1 means that delay branches to set 
    b delay
    
    set:
    str r2, [r0, #40]
    
    b inf_loop
