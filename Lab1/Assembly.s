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
; R1 holds loop counter or output pointer
; R2 holds index of min value
; R3 holds index of max value
; R12 holds input length
; S0 holds progress of RMS or current min/max
; S1 holds current item in array
	CBNZ R2, fn			; Check if length is 0
	MOV R0, #-1			; Store only 0's in output
	BX r14
fn		PUSH {R1}				; put output pointer on stack because it isn't needed for now
		MOV R1, #0				; initialize loop counter to 0
		LSL R2, #2				; multiply length by 4 for alignment
		MOV R12, R2				; move input length to R12
		MOV R2, R1				; initialize min index to 0
		MOV R3, R1				; initialize max index to 0
		B start					; skip loop label, don't increment counter before first iteration
loop	ADD R1, R1, #4			; increment counter before the next iteration
		VPOP {S0}				; restore ongoing sum of squares
		POP {R12}				; retrieve input length from stack
start	CMP R1, R12				; check if final iteration has completed
		BEQ calc
		PUSH {R12}				; input length no longer needed in loop iteration, put it on stack
		ADD R12, R0, R1			; calculate address of next item for VLDR
		VLDR.32 S1, [R12]		; next item goes in S1
		VFMA.F32 S0, S1, S1		; increment S0 by (next item)^2
		VPUSH {S0}				; sum of squares no longer needed in loop iteration, put it on stack
		ADD R12, R0, R2			; calculate address of current min for VLDR
		VLDR.32 S0, [R12]		; current min goes in S0
		VCMP.F32 S1, S0			; compare current item to current min
		VMRS APSR_nzcv, FPSCR	; send comparison results to core
		BLT min					; if new min is seen, branch...
		ADD R12, R0, R3			; ... else calculate address of current max for VLDR
		VLDR.32 S0, [R12]		; current max goes in S0
		VCMP.F32 S1, S0			; compare current item to current max
		VMRS APSR_nzcv, FPSCR	; send comparison results to core
		BLT loop				; if not min or max, go to next iteration...
max		MOV R3, R1				; ... else, it's a max, so put current index in R4
		B loop
min		MOV R2, R1				; it's a min, put current index in R3
		B loop
calc	LSR R12, #2				; restore R2 to original value by dividing by 4
		POP {R1}
		VMOV S1, R12			; put length in floating point register
		VCVT.F32.U32 S1, S1		; tell S1 to interpret data as int
		VDIV.F32 S0, S0, S1		; divide sum of squares by length (mean square)
		VSQRT.F32 S0, S0		; get RMS
		VSTR.32 S0, [R1]		; store RMS in first cell of output
		ADD R12, R0, R3			; calculate address of max value
		VLDR.32 S0, [R12]
		VSTR.32 S0, [R1, #4]	; store max value in second cell of output
		ADD R12, R0, R2			; calculate address of min value
		VLDR.32 S0, [R12]	
		VSTR.32 S0, [R1, #8]	; store min value in third cell of output
		LSR R3, #2				; convert max index to bytes
		LSR R2, #2				; convert min index to bytes
		VMOV S0, R3				; move max index to floating point register
		VCVT.F32.U32 S0, S0		; tell S0 to interpret data as integer
		VSTR.32 S0, [R1, #12]	; store max index in fourth cell of output
		VMOV S0, R2				; move min index to S0
		VCVT.F32.U32 S0, S0		; tell S0 to interpret data as integer
		VSTR.32 S0, [R1, #16]	; store min index in fifth cell of output
		BX r14
		
END
