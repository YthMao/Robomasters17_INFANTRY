#ifndef _GIMBALCONTROL_H_
#define _GIMBALCONTROL_H_


#include "sys.h"
#include "stm32f4xx.h"
#include "can2.h"
#include "pid.h"

/*
*********************************************************************************************************
*                                         PID参数设置
*********************************************************************************************************
*/

#define PIDVAL_YAW_POS_p				8.0
#define PIDVAL_YAW_POS_i				20.0
#define PIDVAL_YAW_POS_d				0.03
#define PIDVAL_YAW_SPEED_p			21.0
#define PIDVAL_YAW_SPEED_i			0.0
#define PIDVAL_YAW_SPEED_d			0.0

#define PIDVAL_PITCH_POS_p			13.0
#define PIDVAL_PITCH_POS_i			1.5
#define PIDVAL_PITCH_POS_d			0.06
#define PIDVAL_PITCH_SPEED_p		18.0
#define PIDVAL_PITCH_SPEED_i		0.0
#define PIDVAL_PITCH_SPEED_d		0.0
typedef struct 
{
	PID_Struct Position;
	PID_Struct Speed;
	
}MotorPID_t;
extern MotorPID_t	MotorPID_pitch;	
extern MotorPID_t	MotorPID_yaw;



/*
*********************************************************************************************************
*                                         云台电机参数
*********************************************************************************************************
*/
#define INFANTRY_1        //设置这个值来改变云台电机参数

#ifdef INFANTRY_1
#define MIDDLE_YAW_INFANTRY 5695
#define MIDDLE_PITCH_INFANTRY 1200
#endif

#ifdef INFANTRY_2
#define MIDDLE_YAW_INFANTRY 3000
#define MIDDLE_PITCH_INFANTRY 5000
#endif
/*****************************/

#define MIDDLE_YAW 		MIDDLE_YAW_INFANTRY  	//YAW云台中值
#define MIDDLE_PITCH 	MIDDLE_PITCH_INFANTRY //PITCH云台中值
#define YawMax 1000   					//最左 	+1000  or 800
#define YawMin -1000						//最右		-1000
#define PitMax 310	//最下	+310
#define PitMin -600 	//最上		-600			

#define MOTORMINPOSITION -4000//最大设定RPM绝对值
#define MOTORMAXPOSITION 10000//最大设定RPM绝对值 6000
//#define MOTORRPMTHRESHOLD 20 //设定RPM的门限绝对值

typedef struct 
{
	float yaw;
	float angle_yaw_set;
	float angle_yaw_ex;
	float pitch;
	float angle_pitch_set;
	float angle_pitch_ex;
	
}Gimbal_Control_t;
extern Gimbal_Control_t Gimbal_control;

void GimbalControlInit(void);
void GimbalControlLoop(void);
void GimbalStop(void);
void GimbalControlValSet(void);
void PidWholeInit(void);
float GimbalPositionControl(float SetPosition,float NowPosition,PID_Struct* pid);//云台电机位置环控制
float GimbalSpeedControl(float SetSpeed, float NowSpeed , PID_Struct* pid);
void GimbalControl_Pitch(float SetPosition);
void GimbalControl_Yaw(float SetPosition);
float MotorPositionLegalize(float PositionPara , float MAX , float MIN);
#endif
