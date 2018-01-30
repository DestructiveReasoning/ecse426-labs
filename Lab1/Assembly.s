	AREA text, CODE, READONLY
	EXPORT Example_asm
	EXPORT ams_math

; function Example_asm
; inputs:
; -------
; R0: input


Example_asm

	MOV		R6, R0							
	BX		LR	
	
;END

ams_math
; R0 holds input pointer
; R1 holds output pointer
; R2 holds input length
; R3 holds current min index
; R4 holds current max index
; R5 index counter
; S0 holds progress of RMS
	CBNZ R2, fn			; Check if length is 0
	MOV R0, #-1			; Store only 0's in output
	BX r14
fn		MOV R5, #0				; loop counter
		MOV R3, R5				; initialize min index to 0
		MOV R4, R5				; initialize max index to 0
		LSL R2, #2				; multiply length by 4 for alignment
		B start					; skip loop label, don't increment counter before first iteration
loop	ADD R5, R5, #4			; increment counter before the next iteration
start	CMP R5, R2				; check if final iteration has completed
		BEQ calc
		ADD R6, R0, R5			; calculate address of next item for VLDR
		VLDR.32 S2, [R6]		; next item goes in S2
		VFMA.F32 S0, S2, S2		; increment S0 by (next item)^2
		ADD R6, R0, R3			; calculate address of current min for VLDR
		VLDR.32 S1, [R6]		; current min goes in S1
		VCMP.F32 S2, S1			; compare current item to current min
		VMRS APSR_nzcv, FPSCR	; send comparison results to core
		BLT min					; if new min is seen, branch...
		ADD R6, R0, R4			; ... else calculate address of current max for VLDR
		VLDR.32 S1, [R6]		; current max goes in S1
		VCMP.F32 S2, S1			; compare current item to current max
		VMRS APSR_nzcv, FPSCR	; send comparison results to core
		BLT loop				; if not min or max, go to next iteration...
max		MOV R4, R5				; ... else, it's a max, so put current index in R4
		B loop
min		MOV R3, R5				; it's a min, put current index in R3
		B loop
calc	LSR R2, #2				; restore R2 to original value by dividing by 4
		VMOV S1, R2				; put length in floating point register
		VCVT.F32.U32 S1, S1		; tell S1 to interpret data as int
		VDIV.F32 S0, S0, S1		; divide sum of squares by length (mean square)
		VSQRT.F32 S0, S0		; get RMS
		VSTR.32 S0, [R1]		; store RMS in first cell of output
		ADD R6, R0, R4			; calculate address of max value
		VLDR.32 S0, [R6]
		VSTR.32 S0, [R1, #4]	; store max value in second cell of output
		ADD R6, R0, R3			; calculate address of min value
		VLDR.32 S0, [R6]	
		VSTR.32 S0, [R1, #8]	; store min value in third cell of output
		LSR R4, #2				; convert max index to bytes
		LSR R3, #2				; convert min index to bytes
		VMOV S0, R4				; move max index to floating point register
		VCVT.F32.U32 S0, S0		; tell S0 to interpret data as integer
		VSTR.32 S0, [R1, #12]	; store max index in fourth cell of output
		VMOV S0, R3				; move min index to S0
		VCVT.F32.U32 S0, S0		; tell S0 to interpret data as integer
		VSTR.32 S0, [R1, #16]	; store min index in fifth cell of output
		BX r14
		
END
