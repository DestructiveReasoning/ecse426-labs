#include "voltmeter.h"
#include "math.h"
#include "stm32f4xx_hal.h"
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

extern TIM_HandleTypeDef htim3;

/**
 * @brief FIR filter
 * 4th order filter that outputs the average over the past 5 inputs
 * @param Input Digital input reading
 * @param Output Reference to float to store the filtered output
 * @retval None
 */
void FIR_C(int Input, float *Output) {
	float b[] = {0.15, 0.15, 0.15, 0.15, 0.15, 0.05, 0.05, 0.05, 0.05, 0.05}; 
	static int x[10]; //stores last 5 inputs
	static int count = 0; //counts how many inputs have been seen
	float out = 0.0;
	int c; //loop index
	int start = count % 10; //index of newest input
	x[(count++) % 10] = Input;
	for(c = 0; c < count && c < 10; ++c) {
		int pos = (5 + (start - c) % 10) % 10; //obtain non-negative position of cth newest item in array
		out += x[pos] * b[c];
	}
	if(count == 20) {
		count = 10; //avoid overflowing count
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
	static int point_count = 0;
	static float rms_counter = 0.0;
	if(point_count == 0) {
		rms_counter = input * input;
	} else {
		rms_counter += input * input;
	}
	point_count++;
	if(point_count == RMS_UPDATE_WINDOW) *output = sqrt(rms_counter / ((float) point_count));
	point_count %= RMS_UPDATE_WINDOW;
}

void display_num(char code) {
	if(code & SEG7_A) HAL_GPIO_WritePin(LED_A, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LED_A, GPIO_PIN_RESET);
	if(code & SEG7_B) HAL_GPIO_WritePin(LED_B, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LED_B, GPIO_PIN_RESET);
	if(code & SEG7_C) HAL_GPIO_WritePin(LED_C, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LED_C, GPIO_PIN_RESET);
	if(code & SEG7_D) HAL_GPIO_WritePin(LED_D, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LED_D, GPIO_PIN_RESET);
	if(code & SEG7_E) HAL_GPIO_WritePin(LED_E, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LED_E, GPIO_PIN_RESET);
	if(code & SEG7_F) HAL_GPIO_WritePin(LED_F, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LED_F, GPIO_PIN_RESET);
	if(code & SEG7_G) HAL_GPIO_WritePin(LED_G, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LED_G, GPIO_PIN_RESET);
	if(code & SEG7_DP) HAL_GPIO_WritePin(LED_DP, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(LED_DP, GPIO_PIN_RESET);
}

void write_to_display(float val) {
	static int display_counter = 0;
	if(display_counter % 3 == 0) {
		display_num(get_display_leds((int)(val * 100) % 100));
		HAL_GPIO_WritePin(LED_DP, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DIG_SEL_ONES, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DIG_SEL_TENTHS, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DIG_SEL_HUNDREDTHS, GPIO_PIN_RESET);
	} else if(display_counter % 3 == 1) {
		display_num(get_display_leds((int)(val * 10) % 10));
		HAL_GPIO_WritePin(LED_DP, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DIG_SEL_ONES, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DIG_SEL_TENTHS, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DIG_SEL_HUNDREDTHS, GPIO_PIN_RESET);
	} else {
		display_num(get_display_leds((int)val));
		HAL_GPIO_WritePin(LED_DP, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DIG_SEL_ONES, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DIG_SEL_TENTHS, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DIG_SEL_HUNDREDTHS, GPIO_PIN_SET);
	}
	display_counter++;
}

void shut_off_display(void) {
	HAL_GPIO_WritePin(LED_DP, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIG_SEL_ONES, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIG_SEL_TENTHS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIG_SEL_HUNDREDTHS, GPIO_PIN_RESET);
}

void adjust_pwm(float cur, float target) {
	double diff = target - cur;
	duty_cycle += diff * P_CONSTANT;
	if(duty_cycle < 0.0) duty_cycle = 0.0;
	else if(duty_cycle > 1.0) duty_cycle = 1.0;
	TIM_OC_InitTypeDef sConfigOC;
	sConfigOC.Pulse = duty_cycle * PWM_PERIOD;
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	HAL_TIM_MspPostInit(&htim3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}
