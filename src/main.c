#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"
#include "btn.h"

void GPIOD_Config(void);
void NVIC_Config(void);
void INTTIM_Config(int, int);
void TIM2_IRQHandler(void);
void StraightDirection(void);
void ReversedDirection(void);
void Reset_ALL_GPIOD(void);

static int statement = 1;

int main(void) {
	int x = 0;
	GPIOD_Config();
	GPIOA_Config();
	NVIC_Config();

    // External interrupt initialization
    EXTI_Config();

	while(1){
		if(isPressed()){
			if(x != 1){
				Reset_ALL_GPIOD();
				INTTIM_Config(400, 42000);
				x = 1;
			}
			StraightDirection();
		}else{
			if(x != 2){
				Reset_ALL_GPIOD();
				INTTIM_Config(1000, 42000);
				x = 2;
			}
			ReversedDirection();
		}
	}
}

void GPIOD_Config(void) {
	GPIO_InitTypeDef gpioConf;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	gpioConf.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpioConf.GPIO_Mode = GPIO_Mode_OUT;
	gpioConf.GPIO_Speed = GPIO_Speed_100MHz;
	gpioConf.GPIO_OType = GPIO_OType_PP;
	gpioConf.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD, &gpioConf);
}

void StraightDirection(void){
	switch(statement){
		case 1:
			if(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_12))
				GPIO_ResetBits(GPIOD, GPIO_Pin_12);
			if(!GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_14)) //if led lights, won't turn it on again
				GPIO_SetBits(GPIOD, GPIO_Pin_14);
		break;
		case 2:
			if(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_14))
				GPIO_ResetBits(GPIOD, GPIO_Pin_14);
			if(!GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_13))
				GPIO_SetBits(GPIOD, GPIO_Pin_13);
		break;
		case 3:
			if(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_13))
				GPIO_ResetBits(GPIOD, GPIO_Pin_13);
			if(!GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_15))
				GPIO_SetBits(GPIOD, GPIO_Pin_15);
		break;
		case 4:
			if(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_15))
				GPIO_ResetBits(GPIOD, GPIO_Pin_15);
			if(!GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_12))
				GPIO_SetBits(GPIOD, GPIO_Pin_12);
		break;
	}
}

void ReversedDirection(void){
	switch(statement){
		case 1:
			if(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_14))
				GPIO_ResetBits(GPIOD, GPIO_Pin_14);
			if(!GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_12))
				GPIO_SetBits(GPIOD, GPIO_Pin_12);
		break;
		case 2:
			if(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_12))
				GPIO_ResetBits(GPIOD, GPIO_Pin_12);
			if(!GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_15))
				GPIO_SetBits(GPIOD, GPIO_Pin_15);
		break;
		case 3:
			if(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_15))
				GPIO_ResetBits(GPIOD, GPIO_Pin_15);
			if(!GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_13))
				GPIO_SetBits(GPIOD, GPIO_Pin_13);
		break;
		case 4:
			if(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_13))
				GPIO_ResetBits(GPIOD, GPIO_Pin_13);
			if(!GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_14))
				GPIO_SetBits(GPIOD, GPIO_Pin_14);
		break;
	}
}

void Reset_ALL_GPIOD(void){
	GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}

void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if(statement != 4){
			statement++;
		}else{
			statement = 1;
		}
	}
}

void INTTIM_Config(int period, int prescaler) {
	TIM_DeInit(TIM2);

	TIM_TimeBaseInitTypeDef tim_struct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	tim_struct.TIM_Prescaler = prescaler;
	tim_struct.TIM_Period = period;
	tim_struct.TIM_ClockDivision = 0;
	tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &tim_struct);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

void NVIC_Config(){
	NVIC_InitTypeDef nvic_struct;
	nvic_struct.NVIC_IRQChannel = TIM2_IRQn;
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_struct.NVIC_IRQChannelSubPriority = 1;
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvic_struct);
}
