	AREA text, CODE, READONLY
	EXPORT Example_asm
	EXPORT asm_math

; function Example_asm
; inputs:
; -------
; R0: input


Example_asm

	MOV		R6, R0							
	BX		LR	
	
;END

asm_math
; R0 holds input pointer
; R1 holds loop counter and output pointer
; R2 holds input length
; R3 holds current min index
; R4 holds current max index
; S0 holds progress of RMS
; S1 holds current array item
; S2 holds current min
; S3 holds current max
	CBNZ R2, fn			; Check if length is 0
	MOV R0, #-1			; Store only 0's in output
	BX r14
fn		PUSH {R4}				; preserve R4 value
		PUSH {R1}				; preserve output pointer
		MOV R1, #0				; loop counter
		MOV R3, R1				; initialize min index to 0
		MOV R4, R1				; initialize max index to 0
		VMOV S0, R1				; initialize RMS to 0
		VLDR.32 S1, [R0]		; current item
		VMOV.F32 S2, S1			; current min and max default to first value
		VMOV.F32 S3, S1
		LSL R2, #2				; multiply length by 4 for alignment
		VFMA.F32 S0, S1, S1
loop	ADD R1, R1, #4			; increment counter before the next iteration
		CMP R1, R2				; check if final iteration has completed
		BEQ calc
		ADD R0, R0, #4			; increment input pointer to next item
		VLDR.32 S1, [R0]		; next item goes in S2
		VFMA.F32 S0, S1, S1		; increment S0 by (next item)^2
		VCMP.F32 S1, S2			; compare current item to current min
		VMRS APSR_nzcv, FPSCR	; send comparison results to core
		BLT min					; if new min is seen, branch...
		VCMP.F32 S1, S3			; compare current item to current max
		VMRS APSR_nzcv, FPSCR	; send comparison results to core
		BLT loop				; if not min or max, go to next iteration...
max		MOV R4, R1				; ... else, it's a max, so put current index in R4
		VMOV.F32 S3, S1			; store new max
		B loop
min		MOV R3, R1				; it's a min, put current index in R3
		VMOV.F32 S2, S1			; store new min
		B loop
calc	LSR R2, #2				; restore R2 to original value by dividing by 4
		POP {R1}				; restore output pointer to R1
		VMOV S1, R2				; put length in floating point register
		VCVT.F32.U32 S1, S1		; tell S1 to interpret data as int
		VDIV.F32 S0, S0, S1		; divide sum of squares by length (mean square)
		VSQRT.F32 S0, S0		; get RMS
		VSTR.32 S0, [R1]		; store RMS in first cell of output
		VSTR.32 S3, [R1, #4]	; store max value in second cell of output
		VSTR.32 S2, [R1, #8]	; store min value in third cell of output
		LSR R4, #2				; convert max index to bytes
		LSR R3, #2				; convert min index to bytes
		VMOV S0, R4				; move max index to floating point register
		VCVT.F32.U32 S0, S0		; tell S0 to interpret data as integer
		VSTR.32 S0, [R1, #12]	; store max index in fourth cell of output
		VMOV S0, R3				; move min index to S0
		VCVT.F32.U32 S0, S0		; tell S0 to interpret data as integer
		VSTR.32 S0, [R1, #16]	; store min index in fifth cell of output
		POP {R4}				; restore R4's value from previous context
		BX r14
		
END
