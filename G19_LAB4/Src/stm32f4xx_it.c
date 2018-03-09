/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "voltmeter.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;

extern int counter;
extern int pmode;
int last_button_press = -1;
extern int systick_counter;

extern int holding;
extern int hold_count;
extern int col;
extern int state_counter;
extern int state;
extern float temp_voltage;
extern float the_reading;
/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

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
	if(systick_counter % 3 == 0) {
		display_num(get_display_leds((int)(val * 100) % 100));
		HAL_GPIO_WritePin(LED_DP, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DIG_SEL_ONES, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DIG_SEL_TENTHS, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DIG_SEL_HUNDREDTHS, GPIO_PIN_RESET);
	} else if(systick_counter % 3 == 1) {
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
}

void shut_off_display(void) {
	HAL_GPIO_WritePin(LED_DP, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIG_SEL_ONES, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIG_SEL_TENTHS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIG_SEL_HUNDREDTHS, GPIO_PIN_RESET);
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  osSystickHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

//void SysTick_Handler(void)
//{
//	counter++;
//	systick_counter++;
//	if (systick_counter % 400 == 0) state_counter++;
//	if(counter % 100 == 0) col = (col + 1) % 3;
//  /* USER CODE BEGIN SysTick_IRQn 0 */

//  /* USER CODE END SysTick_IRQn 0 */
//  HAL_IncTick();
//  HAL_SYSTICK_IRQHandler();
//  if(holding) hold_count++;
//  /* USER CODE BEGIN SysTick_IRQn 1 */

//  /* USER CODE END SysTick_IRQn 1 */
//  switch(state) {
//	  case FIRST_KEY:
//		  write_to_display(the_reading);
//		  break;
//	  case SECOND_KEY:
//		  write_to_display(temp_voltage);
//		  break;
//	  case WAIT:
//		  write_to_display(temp_voltage);
//		  break;
//	  case SLEEP:
//		  shut_off_display();
//		  break;
//  }
//}


/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles ADC1, ADC2 and ADC3 global interrupts.
*/
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */

  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC_IRQn 1 */

  /* USER CODE END ADC_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
