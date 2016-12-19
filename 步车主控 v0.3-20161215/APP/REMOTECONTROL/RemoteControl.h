#ifndef _REMOTECONTROL_H_
#define _REMOTECONTROL_H_

/*
*********************************************************************************************************
*                                           PERIPH BUF SIZES 
*********************************************************************************************************
*/
#include "sys.h"
#include "stm32f4xx.h"

typedef enum
{
    PREPARE_STATE=0,     	//上电后初始化状态 4s钟左右
		NORMAL_REMOTE_STATE,
		KEY_REMOTE_STATE,
    STANDBY_STATE,			//云台停止不转状态
		ERROR_STATE,
}RemoteState_e;

void RemoteControlInit(void);
void RemoteControlLoop(void);
void RemoteStateControl(void);


extern RemoteState_e remoteState;

#endif
