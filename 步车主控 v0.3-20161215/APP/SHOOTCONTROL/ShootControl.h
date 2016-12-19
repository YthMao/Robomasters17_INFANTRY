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
void GunControl(void);								//拨弹电机操作
void GunShootAction(void);							//拨弹电机动作函数

typedef enum
{
    Friction_OFF=0,     	//
		Friction_ON,
	
}FrictionState_e;

#define Shoot_UP 2500       //摩擦轮开  
#define Shoot_DOWN 1000     //摩擦轮关	
#define GUNMAXPERTIME 30		//每次射击时长，超过后停止

extern FrictionState_e frictionState;


#endif
