#ifndef __LASER_H
#define __LASER_H
#include "stm32f4xx.h"

#define LASER_ON() 			GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define LASER_OFF() 		GPIO_ResetBits(GPIOA,GPIO_Pin_1)

void Laser_Configuration(void);

#endif
