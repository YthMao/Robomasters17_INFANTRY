#ifndef _PWM_H
#define _PWM_H
#include "stm32f4xx.h"
//#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void TIM4_PWM_Init(u32 arr,u32 psc);
extern u16 TIM4Timeout ;
#define Shoot_UP 2500       //Ħ���ֿ�  
#define Shoot_DOWN 1000     //Ħ���ֹ�	


#endif
