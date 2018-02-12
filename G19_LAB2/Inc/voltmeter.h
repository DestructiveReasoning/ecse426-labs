#ifndef __VOLTMETER_H_
#define __VOLTMETER_H_

#define RMS_MODE 0
#define MIN_MODE 1
#define MAX_MODE 2
#define RMS_PIN LD6_Pin
#define MIN_PIN LD4_Pin
#define MAX_PIN LD3_Pin
#define AMOUNT_OF_DISPLAY_MODES 3

void FIR_C(int input, float *output);
void plot_point(float, float *);

#endif
