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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	Led_Configuration();
	Laser_Configuration();
	Buzzer_Init();
	USART1_Configuration(9600);	//初始化串口波特率为115200
	USART3_DMA_Init();
	CAN1_Mode_Init(CAN_SJW_2tq,CAN_BS2_3tq,CAN_BS1_6tq,7,CAN_Mode_Normal);		//CAN1 底盘 波特率为1Mbps
	CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,3,CAN_Mode_Normal);   	//CAN2 云台 波特率为1Mbps
	RC_Init();	      				//遥控器初始化   2通道是+-660
	PWM_Configuration();      //GUN的PWM初始化  TIM3、TIM4

	WholeInitTask(); 					//所有任务初始化
	TIM5_Int_Init();					//controlTask控制定时器
	TIM2_Int_Init();
}
