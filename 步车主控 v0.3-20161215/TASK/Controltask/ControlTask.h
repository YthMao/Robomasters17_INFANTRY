#ifndef __ControlTask_H
#define __ControlTask_H	

#include "stm32f4xx.h"
//#include "sys.h"
#include "main.h"


void WholeInitTask(void);
void Control_Task(void);

extern int dat_pack[3];
extern int16_t move_round;
extern u8 isFrictionON;
extern int MotorSetPositionPit, MotorSetPositionYaw;    //之前设为static
extern float MotorPositionPit,MotorSpeedPit;									//位置、速度环控制参数
extern float MotorPositionYaw,MotorSpeedYaw;  


void Relative_Cal(void);
void Relative_Cal_key(void);
void Relative_Cal_view(void);
void FrictionJugg(void);
#endif

