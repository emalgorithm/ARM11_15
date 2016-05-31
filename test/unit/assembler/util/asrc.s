; comments should be ignored by tokeniser

                       	; empty lines too

  mov r1, r2            ; an instruction with two registers
l1:                    	; a label should be skipped

l2:   					; inline comment for label
  add r1, r2        	; label with instruction on same line

l3:
  sub r1, r2, r3 		; inline comment for instruction
  
l4:

