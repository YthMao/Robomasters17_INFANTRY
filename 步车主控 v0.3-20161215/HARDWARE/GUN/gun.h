#ifndef _GUN_H_
#define _GUN_H_

/*
*********************************************************************************************************
*                                           PERIPH BUF SIZES 
*********************************************************************************************************
*/
#include "sys.h"
#include "stm32f4xx.h"

void PWM_Configuration(void);

#define Shoot_UP 		2500       //Ħ���ִ�ֵ
#define Shoot_DOWN 	1000     //Ħ���ֹر�ֵ	

#define SHOOT_ON()				TIM_SetCompare1(TIM3,600);TIM_SetCompare2(TIM3,0)	   //�Զ��� 40
#define SHOOT_OFF()				TIM_SetCompare1(TIM3,0) ;	TIM_SetCompare2(TIM3,0)	
#define SHOOT_RE()				TIM_SetCompare1(TIM3,0);TIM_SetCompare2(TIM3,600)

#endif

