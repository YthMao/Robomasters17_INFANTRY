#ifndef _SHOOTCONTROL_H_
#define _SHOOTCONTROL_H_

/*
*********************************************************************************************************
*                                           PERIPH BUF SIZES 
*********************************************************************************************************
*/
#include "sys.h"
#include "stm32f4xx.h"

void ShootControlInit(void);
void ShootControlLoop(void);
void FrictionControl(void);
void FrictionJugg(void);
void ShootStop(void);
void GunControl(void);								//�����������
void GunShootAction(void);							//���������������

typedef enum
{
    Friction_OFF=0,     	//
		Friction_ON,
	
}FrictionState_e;

#define Shoot_UP 2500       //Ħ���ֿ�  
#define Shoot_DOWN 1000     //Ħ���ֹ�	
#define GUNMAXPERTIME 30		//ÿ�����ʱ����������ֹͣ

extern FrictionState_e frictionState;


#endif
