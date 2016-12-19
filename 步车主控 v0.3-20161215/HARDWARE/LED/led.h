#ifndef __LED_H
#define __LED_H
#include "stm32f4xx.h"
//#include "sys.h"
//#include "rc.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED端口定义
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	 

//#define SHUT_ON()				GPIO_SetBits(GPIOA,GPIO_Pin_6);		GPIO_ResetBits(GPIOA,GPIO_Pin_7)
//#define SHUT_OFF()			GPIO_ResetBits(GPIOA,GPIO_Pin_6);	GPIO_ResetBits(GPIOA,GPIO_Pin_7)

#define LED1_ON()       GPIO_SetBits(GPIOD,GPIO_Pin_1)
#define LED1_OFF()      GPIO_ResetBits(GPIOD,GPIO_Pin_1)

#define LED1_TOGGLE()   (GPIOD->ODR) ^= GPIO_Pin_1//GREEN
#define LED2_ON()       GPIO_SetBits(GPIOD,GPIO_Pin_2)
#define LED2_OFF()      GPIO_ResetBits(GPIOD,GPIO_Pin_2)
#define LED2_TOGGLE()   (GPIOD->ODR) ^= GPIO_Pin_2//RED

#define VIEW_CHANGE 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) //PE4

void Led_Configuration(void);//初始化		 
u8 Start(void);

#endif
