/************************************************************
 *File		:	GimbalMotor.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: 	Control Gimbal Motor by CAN2. 
								CAN2 bound should be set at 1Mbps
								This file contects the function for motor action.						
 ************************************************************/

#include "GimbalMotor.h"
#include "sys.h"
#include "delay.h"
#include "can2.h"
u8 MotorState201=0;//0-停止转动 1-正转 2-反转
u8 MotorState203=0;//0-停止转动 1-正转 2-反转
s16 MotorCurrent201 = 0;
s16 MotorCurrent203 = 0;

/*************************************************************************
                              电机控制底层程序       
注：此文档中的程序包含Pit和Yaw两个相对应的部分，
		本文档中只解释Pit部分，Yaw类同												
*************************************************************************/

//MotorCurrentLegalize
//功能：电机给定值规范化程序
//参数：无
//返回值：规范化的电机值	
float MotorCurrentLegalize(float MotorCurrent)
{
	return MotorCurrent<-4500?-4500:(MotorCurrent>4500?4500:MotorCurrent);
}


////////////////*以下为Pitch轴电机部分控制程序*////////////////////

//MotorStopPit(YAW)
//功能：PIT电机停转程序 通过Cmd_ESC()实现，即发送CAN信号
//参数：无
//返回值：void	
void MotorStopPit()//电机停转
{
	MotorCurrent201 = 0;//PWM设置为0
  Cmd_ESC(MotorCurrent201,0,MotorCurrent203);
	MotorState201=0;
}

//MotorRotatePit(YAW)
//功能：PIT电机旋转程序 通过Cmd_ESC()实现，即发送CAN信号
//      其中MotorState201用于判断电机当前的运转方向，若需要反转，则需要暂停一下
//参数：无
//返回值：void	
void MotorRotatePit()//电机旋转
{
	if(MotorCurrent201==0)
	{
		 MotorStopPit();
		 return;//如果PWM为0则停转直接返回
	} 
	
	if(MotorCurrent201>0)//电机正向加电
	{
		Cmd_ESC(MotorCurrent201,0,MotorCurrent203);       //在YAW轴一并执行
		MotorState201 = 1;//电机正向加电
	}
	else//电机反向加电
	{
		Cmd_ESC(MotorCurrent201,0,MotorCurrent203);
		MotorState201 = 2;//电机反向加电
	}
}

//MotorSetCurrentPit(YAW)
//功能：给定PIT电机旋转值 通过 MotorRotatePit()函数实现
//      其中MotorState201用于判断电机当前的运转方向，若需要反转，则需要暂停一下
//参数：无
//返回值：void	
void MotorSetCurrentPit(s16 MotorCurrent)
{

	if(MotorCurrent==0)
	{
		MotorStopPit();
		return;
	}
	if((MotorCurrent^MotorCurrent201)&0x8000)//电机在转动且方向改变，先刹车再旋转，否则直接改变PWM
	{	
		MotorStopPit();
		delay_ms(1);//刹车并等待1ms
	}
	MotorCurrent201=MotorCurrent;     //用于判断是否方向转变
	MotorRotatePit();     //主要执行
}



////////////////*以下为Yaw轴电机部分控制程序*////////////////////

void MotorStopYaw()//电机停转
{
	MotorCurrent203 = 0;//PWM设置为0
  Cmd_ESC(MotorCurrent201,0,MotorCurrent203);
	MotorState203=0;
}

void MotorRotateYaw()//电机旋转    YAW轴
{
	if(MotorCurrent203==0)
	{
		 MotorStopYaw();
		 return;//如果PWM为0则停转直接返回
	} 
	
	if(MotorCurrent203>0)//电机正向加电
	{
		Cmd_ESC(MotorCurrent201,0,MotorCurrent203);
		MotorState203 = 1;//电机正向加电
	}
	else//电机反向加电
	{
		Cmd_ESC(MotorCurrent201,0,MotorCurrent203);
		MotorState203 = 2;//电机反向加电
	}
}

void MotorSetCurrentYaw(s16 MotorCurrent)
{

	if(MotorCurrent==0)
	{
		MotorStopYaw();
		return;
	}
	if((MotorCurrent^MotorCurrent203)&0x8000)//电机在转动且方向改变，先刹车再旋转，否则直接改变PWM
	{	
		MotorStopYaw();
		delay_ms(1);//刹车并等待1ms
	}
	MotorCurrent203=MotorCurrent;     //用于判断是否方向转变
	MotorRotateYaw();
}
