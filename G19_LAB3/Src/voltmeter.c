#include "voltmeter.h"
#include "math.h"
/**
 *******************************************
 * @file voltmeter.c
 * @brief Handles Voltmeter Utilities
 *******************************************
 */

/**
 * @def RMS_MODE 0
 * @def MIN_MODE 1
 * @def MAX_MODE 2
 * @def RMS_PIN LD6_Pin
 * @def MIN_PIN LD4_Pin
 * @def MAX_PIN LD3_Pin
 * @def SEG7_A 0x80
 * @def SEG7_B 0x40
 * @def SEG7_C 0x20
 * @def SEG7_D 0x10
 * @def SEG7_E 0x08
 * @def SEG7_F 0x04
 * @def SEG7_G 0x02
 * @def SEG7_DP 0x01
 * @def LED_A GPIOE, GPIO_PIN_7
 * @def LED_B GPIOE, GPIO_PIN_10
 * @def LED_C GPIOE, GPIO_PIN_11
 * @def LED_D GPIOE, GPIO_PIN_14
 * @def LED_E GPIOE, GPIO_PIN_15
 * @def LED_F GPIOB, GPIO_PIN_12
 * @def LED_G GPIOB, GPIO_PIN_13
 * @def LED_DP GPIOD, GPIO_PIN_8
 * @def DIG_SEL_HUNDREDTHS	GPIOD, GPIO_PIN_7
 * @def DIG_SEL_TENTHS		GPIOD, GPIO_PIN_3
 * @def DIG_SEL_ONES		GPIOD, GPIO_PIN_0
 */

/**
 * @brief FIR filter
 * 4th order filter that outputs the average over the past 5 inputs
 * @param Input Digital input reading
 * @param Output Reference to float to store the filtered output
 * @retval None
 */
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

/**
 * @brief Get byte to indicate which segments should be turned on for a given integer
 * Should be used as input to display_num()
 * @param c Integer to get byte code for
 * @retval Byte indicating which segments shouold be turned on
 */
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

/**
 * @brief Processes ADC readings 
 * Used to keep track of RMS, min, and max over the past 10 seconds
 * @param input Latest analog ADC reading
 * @param output Array to store the three display values (RMS, min, max)
 * @retval None
 */
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
