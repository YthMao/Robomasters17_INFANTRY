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
	DMA1_Stream5_IRQHandler();    //ң��������
	ItoE();												//����Exֵ������Exֵ�����в���
	RemoteStateControl();
}

void RemoteStateControl(void)    //����״̬��
{
	switch(RC_Ex_Ctl.rc.s2)
	{
		case 1:	remoteState = NORMAL_REMOTE_STATE;  	//ң��������
			break;	
		case 2:	remoteState = STANDBY_STATE;					//�ϵ�ȴ�
			break;
		case 3:	remoteState = KEY_REMOTE_STATE;				//�������
			break;
		default: remoteState = PREPARE_STATE;
			break;
	}
}


