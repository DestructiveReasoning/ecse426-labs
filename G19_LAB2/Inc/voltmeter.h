#ifndef __VOLTMETER_H_
#define __VOLTMETER_H_

#define RMS_MODE 0
#define MIN_MODE 1
#define MAX_MODE 2
#define RMS_PIN LD6_Pin
#define MIN_PIN LD4_Pin
#define MAX_PIN LD3_Pin
#define AMOUNT_OF_DISPLAY_MODES 3

#define SEG7_A 0x80
#define SEG7_B 0x40
#define SEG7_C 0x20
#define SEG7_D 0x10
#define SEG7_E 0x08
#define SEG7_F 0x04
#define SEG7_G 0x02
#define SEG7_DP 0x01

#define LED_A GPIOE, GPIO_PIN_7
#define LED_B GPIOE, GPIO_PIN_10
#define LED_C GPIOE, GPIO_PIN_11
#define LED_D GPIOE, GPIO_PIN_14
#define LED_E GPIOE, GPIO_PIN_15
#define LED_F GPIOB, GPIO_PIN_12
#define LED_G GPIOB, GPIO_PIN_13
#define LED_DP GPIOD, GPIO_PIN_8

#define DIG_SEL_HUNDREDTHS	GPIOD, GPIO_PIN_7
#define DIG_SEL_TENTHS		GPIOD, GPIO_PIN_3
#define DIG_SEL_ONES		GPIOD, GPIO_PIN_0

void FIR_C(int input, float *output);
void plot_point(float, float *);
char get_display_leds(int);

#endif
