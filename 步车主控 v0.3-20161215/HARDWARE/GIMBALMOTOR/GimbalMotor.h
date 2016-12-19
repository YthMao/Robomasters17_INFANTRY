#ifndef __GIMBALMORTOR_H__
#define __GIMBALMORTOR_H__

#include "stm32f4xx.h"
//#include "sys.h"
#define MOTORFOREWARD 0
#define MOTORBACKWARD 1

float MotorCurrentLegalize(float PWMPara);
void MotorSetCurrentPit(s16 MotorCurrent);
void MotorSetCurrentYaw(s16 MotorCurrent);

extern s16 MotorCurrent201 ;
extern s16 MotorCurrent203 ;

//void MotorRotatePit();//电机旋转
//void MotorRotateYaw();//电机旋转
#define abs(x) ((x)>0? (x):(-(x)))

#endif
