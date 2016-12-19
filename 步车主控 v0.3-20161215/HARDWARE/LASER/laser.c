/************************************************************
 *File		:	laser.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: 	Laser output  
 ************************************************************/

#include "laser.h" 
#include "sys.h"

void Laser_Configuration(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOA, ENABLE);//
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;    //���ڼ���		//���ڿ��Ƽ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��	
	
	LASER_OFF();   //д�ڶ�����ʼ����
}






