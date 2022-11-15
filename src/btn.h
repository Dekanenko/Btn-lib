#ifndef BTN_H_
#define BTN_H_

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stdbool.h"

static bool PRESSED = false;
static bool RELEASED = true;

void GPIOA_Config(void);
bool isPressed();
bool isReleased();
void EXTI_Config(void);
void EXTI0_IRQHandler(void);

#endif /* BTN_H_ */
