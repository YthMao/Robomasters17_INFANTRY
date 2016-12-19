#ifndef __LED_H
#define __LED_H
#include "stm32f4xx.h"
//#include "sys.h"
//#include "rc.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED�˿ڶ���
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

void Led_Configuration(void);//��ʼ��		 
u8 Start(void);

#endif
