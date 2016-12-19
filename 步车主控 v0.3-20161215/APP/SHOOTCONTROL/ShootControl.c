/************************************************************
 *File		:	ShootControl.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: 	Application for Shoot Motor, including 2 friction motor and 1 common DC motor.
								ShootControlLoop() shows the main control for Shoot motors.
								Pay attention to FrictionControl()
 ************************************************************/

#include "ShootControl.h"
#include "gun.h"
#include "delay.h"
#include "RemoteControl.h"
#include "rc.h"
#include "laser.h"

FrictionState_e frictionState = Friction_OFF;
static int friction_count ;
static uint8_t friction_rc_ex  ;     //用于储存S1通道值  上中下分别对应1 3 2
static int gun_count = 0;          //用于按键计时，防止触发太快
u8 flag_IsFrtctionOn = 0; //判断摩擦轮是否打开
u8 flag_FrtctionReady = 0; //判断摩擦轮是否打开
//u8 flag_

void ShootControlInit(void)
{
	ShootStop();
	delay_ms(300);
}

void ShootControlLoop(void)
{
	if(remoteState == PREPARE_STATE)
	{
		ShootStop();
	}
	else if(remoteState == NORMAL_REMOTE_STATE)     //Use remote to control
	{
		FrictionJugg();					//判断摩擦轮是否应该打开
		FrictionControl();
		GunControl();
	}
	else if(remoteState == KEY_REMOTE_STATE ) //Use keyboard   
	{
		FrictionJugg();
		FrictionControl();
		GunControl();
	}
	else if(remoteState == STANDBY_STATE ) //Use keyboard   without CM follow
	{
		ShootStop();
	}
}

void FrictionJugg(void)			//判断摩擦轮是否应该打开
{
	uint8_t friction_rc_new = RC_Ex_Ctl.rc.s1;
	if(RC_Ex_Ctl.mouse.press_r) friction_count++;    //鼠标长按计数
	else	friction_count = 0;
	if(frictionState == Friction_OFF && flag_FrtctionReady ==0 )
	{
		if( (friction_rc_new == 3) && (friction_rc_ex == 1) ) frictionState = Friction_ON;
		if(friction_count) frictionState = Friction_ON;
	}
	else if(frictionState == Friction_ON)
	{
		if( (friction_rc_new == 3) && (friction_rc_ex == 1) ) frictionState = Friction_OFF;
		if(friction_count > 40) 
		{
			frictionState = Friction_OFF;
			flag_FrtctionReady = 200;
		}
	}
//	frictionState = Friction_ON;				//test
	friction_rc_ex = friction_rc_new;    //更新s1_ex值
	if(flag_FrtctionReady>0) flag_FrtctionReady--;
}

void FrictionControl(void)		//摩擦轮实现函数
{
	if(frictionState == Friction_OFF )
	{
		TIM_SetCompare1(TIM4,Shoot_DOWN);	
		TIM_SetCompare2(TIM4,Shoot_DOWN);
		LASER_OFF();
	}
	if(frictionState == Friction_ON)
	{
		TIM_SetCompare1(TIM4,Shoot_UP);	
		TIM_SetCompare2(TIM4,Shoot_UP);
		LASER_ON();
	}
}

void ShootStop(void)					//射击待机函数
{
	TIM_SetCompare1(TIM4,Shoot_DOWN);	
	TIM_SetCompare2(TIM4,Shoot_DOWN);
	SHOOT_OFF();
	LASER_OFF();
	frictionState = Friction_OFF;
}

void GunControl(void)								//拨弹电机操作
{
	if(frictionState == Friction_OFF)
	{
		SHOOT_OFF();
	}
	if(frictionState == Friction_ON)
	{
		GunShootAction();
	}
}

void GunShootAction(void)							//拨弹电机动作函数
{

	if(RC_Ex_Ctl.mouse.press_l == 1 ||  RC_Ex_Ctl.rc.s1 == 2) gun_count++;
	else gun_count = 0;
	
	if( gun_count > 0 && gun_count < GUNMAXPERTIME)
	{
		SHOOT_ON();
	}
	else if( gun_count>=GUNMAXPERTIME && gun_count<(GUNMAXPERTIME+5) )
	{
		SHOOT_OFF();
		gun_count = 0;
	}
	else
	{
		SHOOT_OFF();
	}
}
