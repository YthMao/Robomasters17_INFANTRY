#ifndef _CMCONTROL_H_
#define _CMCONTROL_H_

/*
*********************************************************************************************************
*                                           PERIPH BUF SIZES 
*********************************************************************************************************
*/
#include "sys.h"
#include "stm32f4xx.h"


#define speedMIN ((uint16_t)364)
#define speedMAX ((uint16_t)1684)

#define SLOWSLOW 200
#define SLOWSLOWHALF 100
#define SLOWDRIVE 350
#define SLOWHALF 175
#define FASTDRIVE 550
#define FASTHALF 225
#define MOVELIMIT 1200

extern int16_t speedA,speedB,speedC,speedD;
extern u8 gears ;


void CMControlInit(void);
void CMControlLoop(void);

void move(int16_t speedX, int16_t speedY, int16_t rad);
void gearChange(uint16_t keyboardvalue);
void keyboardmove(uint16_t keyboardvalue , int16_t rad);
int16_t followValCal(void);

#endif
