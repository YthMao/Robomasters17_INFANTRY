/************************************************************
 *File		:	RemoteControl.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: 	Remote data analysis
                Change remoteState (state of the system);
 ************************************************************/

#include "RemoteControl.h"
#include "rc.h"

RemoteState_e remoteState = PREPARE_STATE;

void RemoteControlInit(void)
{
	DMA1_Stream5_IRQHandler();  
	ItoE();
	remoteState = PREPARE_STATE;
}

void RemoteControlLoop(void)
{
	DMA1_Stream5_IRQHandler();    //遥控器解码
	ItoE();												//更新Ex值，并用Ex值来进行操作
	RemoteStateControl();
}

void RemoteStateControl(void)    //更改状态机
{
	switch(RC_Ex_Ctl.rc.s2)
	{
		case 1:	remoteState = NORMAL_REMOTE_STATE;  	//遥控器控制
			break;	
		case 2:	remoteState = STANDBY_STATE;					//断电等待
			break;
		case 3:	remoteState = KEY_REMOTE_STATE;				//键鼠控制
			break;
		default: remoteState = PREPARE_STATE;
			break;
	}
}


