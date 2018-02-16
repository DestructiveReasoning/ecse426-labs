#include "voltmeter.h"
#include "math.h"

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

char get_display_leds(int c) {
	//0bABCDEFG{DP}
	c %= 10;
	char led_config = 0;
	if(c != 1 && c != 4) led_config |= SEG7_A;
	if(c != 5 && c != 6) led_config |= SEG7_B;
	if(c != 2) led_config |= SEG7_C;
	if(c != 1 && c != 4 && c != 7) led_config |= SEG7_D;
	if(c % 2 == 0 && c != 4) led_config |= SEG7_E;
	if(c != 1 && c != 2 && c != 3 && c != 7) led_config |= SEG7_F;
	if(c != 1 && c != 7 && c != 0) led_config |= SEG7_G;
	return led_config;
}

void plot_point(float input, float* output) {
	static int count = 0;
	static float min_val = 10.0;
	static float max_val = 0.0;
	static float rms_counter = -1.0;
	static float min_queue[5];
	static float max_queue[5];
	static float rms_queue[10];
	static int m_seconds_counter = 0;
	static int m_queue_length = 0;
	static int r_seconds_counter = 0;
	static int r_queue_length = 0;

	if(min_val > 4.0) {
		min_val = input;
		max_val = input;
		rms_counter = input * input;
	}

	if(count == 0) {
		min_queue[m_seconds_counter] = min_val;
		max_queue[m_seconds_counter] = max_val;
		min_val = input;
		max_val = input;
		m_seconds_counter = (m_seconds_counter + 1) % 5;
		m_queue_length = (m_queue_length >= 5) ? 5 : m_queue_length + 1;
		rms_queue[r_seconds_counter] = rms_counter;
		r_seconds_counter = (r_seconds_counter + 1) % 10;
		r_queue_length = (r_queue_length >= 10) ? 10 : r_queue_length + 1;
		rms_counter = input * input;
	} 
	else {
		if(count % 50 == 0) {
			rms_queue[r_seconds_counter] = rms_counter;
			r_seconds_counter = (r_seconds_counter + 1) % 10;
			r_queue_length = (r_queue_length >= 10) ? 10 : r_queue_length + 1;
			rms_counter = input * input;
		} else rms_counter += input * input;
		if(input < min_val) min_val = input;
		else if(input > max_val) max_val = input;
	}

	float total_squares = 0.0;
	int c;
	for(c = 0; c < r_queue_length; c++) {
		total_squares += rms_queue[c];
	}
	output[RMS_MODE] = sqrt(total_squares / (r_queue_length * 50));
	float m = min_queue[0];
	for(c = 1; c < m_queue_length; c++) {
		if(min_queue[c] < m) m = min_queue[c];
	}
	output[MIN_MODE] = m;
	m = max_queue[0];
	for(c = 1; c < m_queue_length; c++) {
		if(max_queue[c] > m) m = max_queue[c];
	}
	output[MAX_MODE] = m;
	count = (count + 1) % 100;
}