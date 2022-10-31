#ifndef BTN_H_
#define BTN_H_

#include "stm32f4xx.h"
#include "stdbool.h"

static bool PRESSED = false;
static bool RELEASED = true;

void GPIOA_Config(void);
void EXTI_Config(void);
void EXTI0_IRQHandler(void);

#endif /* BTN_H_ */
