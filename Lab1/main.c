#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "arm_math.h"
#include "stm32f407xx.h"

int Example_asm(int Input);
int asm_math(float *input, float *output, int length);
int c_math(float *, float *, int);
int cmsis_math(float32_t *input, float32_t *output, uint32_t length);

void FIR_C(int, float *);

int main()
{
	srand(time(NULL));
	int Input = 10;
	printf("Begins Asm\n");
	
	Example_asm(Input);

	printf("The end!\n");
	
	const int LENGTH = 100;
	int MAX_MAGNITUDE = 50;

	int input[] = {12,21,19,45,34,39,50,52,63,66,83,87,74,92,86,93,99,109,101,115,103,112,117,111,114,113,103,101,117,99,94,102,105,98,85,84,82,88,67,76,67,56,46,45,41,27,31,27,14,12,-1,-7,-6,-20,-14,-17,-28,-41,-42,-57,-46,-51,-56,-72,-69,-84,-79,-84,-90,-92,-88,-96,-87,-90,-86,-86,-86,-98,-96,-98,-82,-77,-80,-68,-67,-58,-62,-62,-62,-47,-38,-40,-41,-32,-28,-19,-10,-2,3,18};
	int c;
	float filtered[LENGTH];
	for(c = 0; c < LENGTH; ++c) {
		FIR_C(input[c], filtered + c);
		printf("%d: %f\n", c, filtered[c]);
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
	float b[] = {0.2, 0.2, 0.2, 0.2, 0.2}; //coefficients
	static int x[5]; //stores last 5 inputs
	static int count = 0; //counts how many inputs have been seen
	float out = 0.0;
	int c; //loop index
	int start = count % 5; //index of newest input
	x[(count++) % 5] = Input;
	for(c = 0; c < count && c < 5; ++c) {
		int pos = (5 + (start - c) % 5) % 5; //obtain non-negative position of cth newest item in array
		out += x[pos] * b[c];
	}
	if(count == 10) {
		count = 5; //avoid overflowing count
	}
	*Output = out;
}

int c_math(float *input, float *output, int length) {
	if(length == 0) {
		return -1; //avoid dividing by 0
	}
	int c; //loop index
	float32_t rms = input[0] * input[0];
	uint32_t min_index = 0;
	uint32_t max_index = 0;
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
	output[3] = (float)max_index; //explicit cast to float because output is a float array
	output[4] = (float)min_index;
	return 0; //no errors
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
	output[3] = (float) max_index; //explicit cast to float because output is a float array
	output[4] = (float) min_index;
	return 0; // no errors
}
