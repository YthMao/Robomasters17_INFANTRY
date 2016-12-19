/************************************************************
 *File		:	led.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: 	Led port
 ************************************************************/

#include "led.h" 
#include "sys.h"

//GPIO_ALL_Init
//���ܣ��������õ������������˿�
//PD1 2  : LED1 LED2
//PA1 	 : LASER ���
//������void
//����ֵ��void		
void Led_Configuration(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOA, ENABLE);//

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14;    //D12 D14�����Ӿ���������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
	
//	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	LED1_OFF();
	LED2_OFF();


}






