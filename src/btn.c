#include "stm32f4xx.h"
#include "stdbool.h"

static bool PRESSED = false;
static bool RELEASED = true;

void GPIOA_Config(void) {
	GPIO_InitTypeDef gpioConf;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	gpioConf.GPIO_Pin = GPIO_Pin_0;
	gpioConf.GPIO_Mode = GPIO_Mode_IN;

	GPIO_Init(GPIOA, &gpioConf);
}

void EXTI0_IRQHandler(void){
    // Checks whether the interrupt from EXTI0 or not
    if (EXTI_GetITStatus(EXTI_Line0)){
    	if(RELEASED){
    		PRESSED = true;
    		RELEASED = false;
    	}else{
    		PRESSED = false;
    		RELEASED = true;
    	}
        // Clears the EXTI line pending bit
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI_Config(void) {
    // Clock for SYSCFG
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    // Selects the GPIOA pin 0 used as external interrupt source
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

    // External interrupt settings
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_Init(&EXTI_InitStruct);

    // Nested vectored interrupt settings
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    // EXTI0_IRQn has Most important interrupt
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_Init(&NVIC_InitStruct);
}

