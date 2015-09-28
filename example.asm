        include "avr.inc"
        include "Z600registers.inc"

	adiw    ZL,0x10 
	ldi     r22,0xF0 
.sendcfi2: 
	out     RAMPZ,r18 
	ld      r16, Z+ 
	add     r23,r16 
	rcall    serial_pc_put
	adiw    ZL,1 
	dec     r22 
	brne    .sendcfi2 

serial_pc_put:
