#ifndef _TIMER_H
#define _TIMER_H
#include "stm32f4xx.h"
//#include "sys.h"
#include "mpu6050_interrupt.h"
#include "mpu6050_driver.h"
#include "mpu6050_process.h"
//////////////////////////////////////////////////////////////////////////////////	 

void TIM5_Int_Init(void);
void TIM2_Int_Init(void);

void BoPanON_function(void);
void BoPanOFF_function(void);


#endif
