#include "voltmeter.h"

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