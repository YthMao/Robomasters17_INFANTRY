/************************************************************
 *File		:	buzzer.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: 	Buzzer
 ************************************************************/

#include "sys.h"
#include "delay.h"
#include "buzzer.h"
#include "rc.h"

//Buzzer_Init
//功能：Buzzer定义初始化函数
//参数：无 PC 12 给值'1'为启动蜂鸣器
//返回值：void
void Buzzer_Init(void)     //PC12
{
    GPIO_InitTypeDef gpio;
    
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	  gpio.GPIO_Pin = GPIO_Pin_12;
	  gpio.GPIO_Mode = GPIO_Mode_OUT;
	  gpio.GPIO_OType = GPIO_OType_PP;
	  gpio.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_Init(GPIOC,&gpio);
    
    BUZZER_OFF();
}

//Start
//功能：系统启动解锁程序
//参数：无 
//返回值：若遥控器两遥杆“内八字”则解锁，返回0 ；否则返回1
u8 Start(void)             //遥控器内八字解锁
{
    u8 startflag = 1;
	  if( (RC_Ex_Ctl.rc.ch2 <= 364) && (RC_Ex_Ctl.rc.ch3 <= 364) )
		{
			  delay_ms(1500);
			  if((RC_Ex_Ctl.rc.ch2 <= 364) && (RC_Ex_Ctl.rc.ch3 <= 364))
				{
						BUZZER_ON();
						delay_ms(600);
						BUZZER_OFF();
						startflag = 0;
				}
		}
	  return startflag;
}
//Buzzer_toggle
//功能：蜂鸣器鸣叫1下
//参数：无 
//返回值：void
void Buzzer_toggle(void)
{
		BUZZER_ON();
		delay_ms(300);
		BUZZER_OFF();
		delay_ms(300);
}
