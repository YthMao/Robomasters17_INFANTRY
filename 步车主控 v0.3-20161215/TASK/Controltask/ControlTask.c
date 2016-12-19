/************************************************************
 *File		:	ControlTask.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: Main Control loop for the whole system
 ************************************************************/

#include <stm32f4xx.h>
#include "ControlTask.h"
#include "main.h"

#define Move

static uint32_t time_tick_1ms = 0;

/*************************************************
Function		: WholeInitTask
Description	: IMU, Remote, CM, Shoot, Gimbal Init
Input				: None
Output			: None
Return			: None
*************************************************/

void WholeInitTask()
{
	IMUControlInit();     //IMUControlLoop在Tim2中
	RemoteControlInit();
	CMControlInit();
	ShootControlInit();
	GimbalControlInit();
}

/*************************************************
Function		: Control_Task
Description	: IMU, Remote, CM, Shoot, Gimbal Control Loop. triggered by Timer_3 interrupt
							1ms:	RemoteLoop, GimbalLoop
							3ms:	CMLoop
							5ms:	ShootLoop
							247ms:Printf some information
Input				: None
Output			: None
Return			: None
*************************************************/
void Control_Task()
{
	time_tick_1ms++;
	//if(time_tick_1ms <100);	
	RemoteControlLoop();
	GimbalControlLoop();
#ifdef Move
	if(time_tick_1ms%3 == 1)         
	{
		CMControlLoop();
	}
#endif
	
	if(time_tick_1ms%5 ==1)
	{
		ShootControlLoop();
	}
#ifdef Printf_test
	if(time_tick_1ms%247==0)		//发送反馈数据
	{
		
	}
#endif
	if(time_tick_1ms%1500 == 0) time_tick_1ms = 0;
}


