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
extern int MotorSetPositionPit, MotorSetPositionYaw;    //֮ǰ��Ϊstatic
extern float MotorPositionPit,MotorSpeedPit;									//λ�á��ٶȻ����Ʋ���
extern float MotorPositionYaw,MotorSpeedYaw;  


void Relative_Cal(void);
void Relative_Cal_key(void);
void Relative_Cal_view(void);
void FrictionJugg(void);
#endif

