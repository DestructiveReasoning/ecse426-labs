#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "arm_math.h"
#include "stm32f407xx.h"

int Example_asm(int Input);
int asm_math(float *input, float *output, int length);
int c_math(float *, float *, int);
int cmsis_math(float32_t *input, float32_t *output, uint32_t length);

void FIR_C(int, float *);

int main()
{
	
	int Input = 10;
	printf("Begins Asm\n");
	
	Example_asm(Input);

	printf("The end!\n");
	
	int LENGTH = 50;
	int MAX_MAGNITUDE = 50;

	int input[LENGTH];
	int c;
	for(c = 0; c < LENGTH; ++c) {
		input[c] = (rand() % (2 * MAX_MAGNITUDE)) - MAX_MAGNITUDE;
	}
	float filtered[LENGTH];
	for(c = 0; c < LENGTH; ++c) {
		FIR_C(input[c], filtered + c);
	}
	float output[5];
		
	asm_math(filtered, output, LENGTH);
	printf("RMS: %f\nMax value: %f\nMin value: %f\nMax index: %f\nMin index: %f\n", output[0], output[1], output[2], output[3], output[4]);
	c_math(filtered, output, LENGTH);
	printf("RMS: %f\nMax value: %f\nMin value: %f\nMax index: %f\nMin index: %f\n", output[0], output[1], output[2], output[3], output[4]);
	cmsis_math(filtered, output, LENGTH);
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
	for(c = 0; c < count && c < 5; ++c) {
		int pos = (5 + (start - c) % 5) % 5;
		out += x[pos] * b[c];
	}
	*Output = out;
}

int c_math(float *input, float *output, int length) {
	if(length == 0) {
		return -1;
	}
	int c;
	float rms = input[0] * input[0];
	int min_index = 0;
	int max_index = 0;
	for(c = 1; c < length; ++c) {
		rms += input[c] * input[c];
		if(input[c] < input[min_index]) {
			min_index = c;
		} else if (input[c] > input[max_index]) {
			max_index = c;
		}
	}
	rms = sqrt(rms/length);
	output[0] = rms;
	output[1] = input[max_index];
	output[2] = input[min_index];
	output[3] = (float)max_index;
	output[4] = (float)min_index;
	return 0;
}

int cmsis_math(float32_t *input, float32_t *output, uint32_t length) {
	if (length == 0) {
		return -1;
	}
	float32_t rms;
	float32_t min_value;
	float32_t max_value;
	uint32_t min_index; 
	uint32_t max_index;
	
	arm_rms_f32(input, length, &rms); // compute rms value of input
	arm_min_f32(input, length, &min_value, &min_index); // compute min value, ouput result to min_value and index to min_index
	arm_max_f32(input, length, &max_value, &max_index); // compute max value, ouput result to max_value and index to max_index
	
	output[0] = rms;
	output[1] = max_value;
	output[2] = min_value;
	output[3] = max_index;
	output[4] = min_index;
	return 0; // no errors
}
