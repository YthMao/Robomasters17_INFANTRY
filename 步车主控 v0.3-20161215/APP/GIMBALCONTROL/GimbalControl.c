/************************************************************
 *File		:	GimbalControl.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: 	Application for Gimbal Motor.
								GimbalControlLoop() shows the main control for Gimbal motor
								Use PID to optimize Gimbal motor control
 ************************************************************/
#include "GimbalControl.h"
#include "RemoteControl.h"
#include "rc.h"
#include "mpu6050_process.h"
#include "pid.h"
#include "GimbalMotor.h"
#include "mpu6050_driver.h"
Gimbal_Control_t Gimbal_control;
MotorPID_t	MotorPID_pitch;	
MotorPID_t	MotorPID_yaw;
float PIDOut_Position_Pit,PIDOut_Speed_Pit,PIDOut_Whole_Pit;					//Pit,Yaw PID总输出
float PIDOut_Position_Yaw,PIDOut_Speed_Yaw,PIDOut_Whole_Yaw;


void GimbalControlInit(void)
{
	PidWholeInit();
	GimbalStop();							//云台初始停转
}

void GimbalControlLoop(void)
{
	GimbalControlValSet();				//计算出Gimbal_control结构体的值，即需要达到的控制值
	GimbalControl_Pitch	(MIDDLE_PITCH	-	Gimbal_control.pitch );
	GimbalControl_Yaw		(MIDDLE_YAW		-	Gimbal_control.yaw);
	
}

void PidWholeInit(void)
{
	//PITCH轴PID设置
	PID_Init(&MotorPID_pitch.Position,PIDVAL_PITCH_POS_p, 	PIDVAL_PITCH_POS_i, 	PIDVAL_PITCH_POS_d,-300,300,0);    //15 1.5 0.04 
	SetPIDCR(&MotorPID_pitch.Position,(1<<PEN)+(1<<IEN)+(1<<PIDEN)+(1<<INTERVAL)+(1<<DEN));
	PID_Init(&MotorPID_pitch.Speed,		PIDVAL_PITCH_SPEED_p, PIDVAL_PITCH_SPEED_i, PIDVAL_PITCH_SPEED_d,-30,30,0);//13 0 0
	SetPIDCR(&MotorPID_pitch.Speed,(1<<PEN)+(1<<IEN)+(1<<DEN)+(1<<PIDEN)+(1<<INTERVAL));
	//YAW轴PID设置
	PID_Init(&MotorPID_yaw.Position,PIDVAL_YAW_POS_p,		PIDVAL_YAW_POS_i,		PIDVAL_YAW_POS_d,	-200,200,0);    //7.0 view 10 30 0.03
	SetPIDCR(&MotorPID_yaw.Position,(1<<PEN)+(1<<IEN)+(1<<PIDEN)+(1<<INTERVAL)+(1<<DEN));
	PID_Init(&MotorPID_yaw.Speed,		PIDVAL_YAW_SPEED_p,	PIDVAL_YAW_SPEED_i,	PIDVAL_YAW_SPEED_d,-30,30,0);    //13.55    //view 25 0 0
	SetPIDCR(&MotorPID_yaw.Speed,(1<<PEN)+(1<<IEN)+(1<<DEN)+(1<<PIDEN)+(1<<INTERVAL));
}

void GimbalControlValSet(void)	//cal Gimbal_control.yaw  and Gimbal_control.pitch
{
	if(remoteState == PREPARE_STATE)
	{
		Gimbal_control.pitch 	= 0;
		Gimbal_control.yaw 		= 0;
		//+停转
	}
	else if(remoteState == NORMAL_REMOTE_STATE)     //Use remote 
	{
		Gimbal_control.pitch 					+= 0.004	*	(RC_Ex_Ctl.rc.ch3 - 1024) ;
		Gimbal_control.angle_yaw_set 	+= 0.0003 * (RC_Ex_Ctl.rc.ch2 - 1024);   //系数要改
	
		Gimbal_control.angle_yaw_ex = MPU6050_Angle.Yaw ;           //注意，这个角度算出来反了！！
		if((Gimbal_control.angle_yaw_set - Gimbal_control.angle_yaw_ex)> 180) 	Gimbal_control.angle_yaw_set -=360;  								//YAW轴 test超过一圈
		if((Gimbal_control.angle_yaw_set - Gimbal_control.angle_yaw_ex)<-180) 	Gimbal_control.angle_yaw_set +=360;
		Gimbal_control.yaw = (Gimbal_control.angle_yaw_set - Gimbal_control.angle_yaw_ex)*8192/360;//8192/360;
		Gimbal_control.pitch 	= MotorPositionLegalize(Gimbal_control.pitch,	PitMax, PitMin);		//对计算出的控制值作规范化处理
		Gimbal_control.yaw 		= MotorPositionLegalize(Gimbal_control.yaw ,	YawMax, YawMin);
	}
	else if(remoteState == KEY_REMOTE_STATE ) //Use keyboard  
	{
		Gimbal_control.pitch 					-= 0.13 * RC_Ex_Ctl.mouse.y  ;
		Gimbal_control.angle_yaw_set	+= 0.0040 * RC_Ex_Ctl.mouse.x;   //系数要改

		Gimbal_control.angle_yaw_ex = MPU6050_Angle.Yaw ;           //注意，这个角度算出来反了！！
		if((Gimbal_control.angle_yaw_set - Gimbal_control.angle_yaw_ex)> 180) 	Gimbal_control.angle_yaw_set -=360;  								//YAW轴 test超过一圈
		if((Gimbal_control.angle_yaw_set - Gimbal_control.angle_yaw_ex)<-180) 	Gimbal_control.angle_yaw_set +=360;
		Gimbal_control.yaw = (Gimbal_control.angle_yaw_set - Gimbal_control.angle_yaw_ex)*8192/360;//8192/360;
		Gimbal_control.pitch 	= MotorPositionLegalize(Gimbal_control.pitch,	PitMax, PitMin);		//对计算出的控制值作规范化处理
		Gimbal_control.yaw 		= MotorPositionLegalize(Gimbal_control.yaw ,	YawMax, YawMin);
	}
	else if(remoteState == STANDBY_STATE ) //
	{
		Gimbal_control.pitch = 0;
		Gimbal_control.yaw = 0;
	}
}

float GimbalPositionControl(float SetPosition,float NowPosition,PID_Struct* pid)//云台电机位置环控制
{
	float pidout;
	(*pid).SetPoint = SetPosition;  //代入设定值和测定值
	(*pid).NowPoint = NowPosition;
	PID_Calc(pid,1000);
	pidout=(*pid).Out;
	pidout= MotorCurrentLegalize(pidout);
	return pidout;
}

float GimbalSpeedControl(float SetSpeed, float NowSpeed , PID_Struct* pid)  //SetSpeed-设定转速（rpm)，NowSpeed-当前转速(rpm)
{
	float pidout;
	(*pid).SetPoint = SetSpeed ;//MotorPIDOutYaw;//代入设定值和测定值
	if(abs(NowSpeed) < 0.0f)
		 NowSpeed = 0.0;
	(*pid).NowPoint = NowSpeed;
	PID_Calc(pid,1000);
	pidout=(*pid).Out ;
	pidout= MotorCurrentLegalize(pidout);
	return pidout;
}

void GimbalControl_Pitch(float SetPosition)
{
	//读云台电机当前值,角速度值
	float NowPosition = current_position_201;						//PITCH电机当前位置
	float NowSpeed = MPU6050_Real_Data.Gyro_X;					//PITCH电机当前角速度
	PIDOut_Position_Pit = GimbalPositionControl(SetPosition, NowPosition, &MotorPID_pitch.Position);    
	PIDOut_Speed_Pit 		= GimbalSpeedControl(0,NowSpeed,&MotorPID_pitch.Speed);
	
	PIDOut_Whole_Pit = PIDOut_Position_Pit + PIDOut_Speed_Pit;						//PID最终输出值
	MotorSetCurrentPit(-PIDOut_Whole_Pit);							//使能电机！！
}

void GimbalControl_Yaw(float SetPosition)
{
	//读云台电机当前值,角速度值
	float NowPosition = current_position_203;						//YAW电机当前位置
	float NowSpeed = -MPU6050_Real_Data.Gyro_Z;					//YAW电机当前角速度
	PIDOut_Position_Yaw = GimbalPositionControl(SetPosition, NowPosition, &MotorPID_yaw.Position);    
	PIDOut_Speed_Yaw		= GimbalSpeedControl(0,NowSpeed,&MotorPID_yaw.Speed);
	
	PIDOut_Whole_Yaw = PIDOut_Position_Yaw + PIDOut_Speed_Yaw;						//PID最终输出值
	MotorSetCurrentYaw(-PIDOut_Whole_Yaw);									//使能电机！！
}


void GimbalStop(void)
{
	Gimbal_control.pitch 	= 0;
	Gimbal_control.yaw 		= 0;
	MotorSetCurrentPit(0);
	MotorSetCurrentYaw(0);
}

float MotorPositionLegalize(float PositionPara , float MAX , float MIN)
{
	float temp;
	temp=PositionPara<MIN ?  MIN:(PositionPara>MAX ? MAX :PositionPara);
//	if(abs(temp)<MOTORRPMTHRESHOLD) temp=0;//转速设定值门限
	return temp;
}
