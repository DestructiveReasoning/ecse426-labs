#include <stdio.h>
#include "arm_math.h"

int Example_asm(int Input);
int asm_math(float *input, float *output, int length);

void FIR_C(int, float *);

int main()
{
	
	int Input = 10;
	printf("Begins Asm\n");
	
	Example_asm(Input);

	printf("The end!\n");

	float input[] = {1.0,2.0,3.0,4.0,5.0,6.0};
	float output[5];
	
	ams_math(input, output, 6);
	printf("RMS: %f\nMax value: %f\nMin value: %f\nMax index: %f\nMin index: %f\n", output[0], output[1], output[2], output[3], output[4]);
	
	return 0;
}

void FIR_C(int Input, float *Output) {
	float b[] = {0.1, 0.15, 0.5, 0.15, 0.1};
	static int x[5];
	static int count = 0;
	float out = 0.0;
	int c;
	int start = count % 5;
	x[(count++) % 5] = Input;
	for(c = 0; c < count && c < 5; c++) {
		int pos = (5 + (start - c) % 5) % 5;
		out += x[pos] * b[c];
	}
	*Output = out;
}
