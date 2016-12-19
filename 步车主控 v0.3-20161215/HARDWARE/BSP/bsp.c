/************************************************************
 *File		:	bsp.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: 	BSP(board support Package) information. 
								Provided  BSP_Init();
 ************************************************************/

#include "main.h"
/*************************************************
Function		: BSP_Init
Description	: Init NVIC, Delay, GPIO, USART, CAN , RC, PWM ,TIMER
Input				: None
Output			: None
Return			: None
*************************************************/
void BSP_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	Led_Configuration();
	Laser_Configuration();
	Buzzer_Init();
	USART1_Configuration(9600);	//��ʼ�����ڲ�����Ϊ115200
	USART3_DMA_Init();
	CAN1_Mode_Init(CAN_SJW_2tq,CAN_BS2_3tq,CAN_BS1_6tq,7,CAN_Mode_Normal);		//CAN1 ���� ������Ϊ1Mbps
	CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,3,CAN_Mode_Normal);   	//CAN2 ��̨ ������Ϊ1Mbps
	RC_Init();	      				//ң������ʼ��   2ͨ����+-660
	PWM_Configuration();      //GUN��PWM��ʼ��  TIM3��TIM4

	WholeInitTask(); 					//���������ʼ��
	TIM5_Int_Init();					//controlTask���ƶ�ʱ��
	TIM2_Int_Init();
}
