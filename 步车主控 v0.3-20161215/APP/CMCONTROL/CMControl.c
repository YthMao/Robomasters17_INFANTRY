/************************************************************
 *File		:	CMControl.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: 	Application for CM Motor.
								CMControlLoop() shows the main control for CM motor
 ************************************************************/

#include "CMControl.h"
#include "RemoteControl.h"
#include "delay.h"
#include "rc.h"
#include "stdlib.h"
#include "ControlTask.h"
#include "can.h"

#define gears_speedXYZ 1.6      
int16_t speedA,speedB,speedC,speedD;
int16_t CMFollowVal=0; 			//���̸���ֵ
u8 gears = 1;
u8 move_count = 0;


void CMControlInit(void)
{
	CAN1_Send_Bottom( 0,0,0,0 );
}

void CMControlLoop(void)
{
	if(remoteState == PREPARE_STATE)
	{
		CAN1_Send_Bottom( 0,0,0,0 );
	}
	else if(remoteState == NORMAL_REMOTE_STATE)     //Use remote to control
	{
		CMFollowVal = followValCal();
		move(RC_Ex_Ctl.rc.ch0,RC_Ex_Ctl.rc.ch1,CMFollowVal * 1.0);  //����0.2
	}
	else if(remoteState == KEY_REMOTE_STATE ) //Use keyboard  within CM follow
	{
		CMFollowVal = followValCal();
		keyboardmove(RC_Ex_Ctl.key.v,CMFollowVal);
	}
	else if(remoteState == STANDBY_STATE ) //Use keyboard   without CM follow
	{
		CAN1_Send_Bottom( 0,0,0,0 );
	}
}
//move
//���ܣ�������̵���ٶȸ���ֵ ��ң�������ơ�ͬʱ�и������ӵ��ٶ�����
//������speedX, speedY, rad
//ʹ��������б�º���
//����ֵ���� �ı���ȫ�ֱ���	speedA, speedB, speedC, speedD
void move(int16_t speedX, int16_t speedY, int16_t rad)
{
	  speedX -= RC_CH_VALUE_OFFSET;
	  speedY -= RC_CH_VALUE_OFFSET;
		speedX *= gears_speedXYZ;
		speedY *= gears_speedXYZ;		
		
		speedA = (float)(speedA*0.7 + (-speedX + speedY + rad)*0.3) ;//Y forward  
	  speedB = (float)(speedB*0.7 + ( speedX + speedY + rad)*0.3);
	  speedC = (float)(speedC*0.7 + ( speedX - speedY + rad)*0.3);
	  speedD = (float)(speedD*0.7 + (-speedX - speedY + rad)*0.3);

		if(speedA > MOVELIMIT) speedA = MOVELIMIT;
		if(speedB > MOVELIMIT) speedB = MOVELIMIT;
		if(speedC > MOVELIMIT) speedC = MOVELIMIT;
		if(speedD > MOVELIMIT) speedD = MOVELIMIT;
	
	CAN1_Send_Bottom(speedA,speedB,speedC,speedD);
}

//keyboardmove
//���ܣ�������̵���ٶȸ���ֵ �ɼ��̿���
//������keyboardvalue
//���������move����
//����ֵ���� �ı���ȫ�ֱ���	speedA, speedB, speedC, speedD
void keyboardmove(uint16_t keyboardvalue , int16_t rad)
{		
	switch(keyboardvalue & 0x00ff)
	{
		case ( KEY_PRESSED_OFFSET_W | KEY_PRESSED_OFFSET_A):
					move(RC_CH_VALUE_OFFSET - gears*SLOWSLOWHALF,RC_CH_VALUE_OFFSET + gears*SLOWSLOWHALF,rad);
		break;
		
		case ( KEY_PRESSED_OFFSET_W | KEY_PRESSED_OFFSET_D):
					move(RC_CH_VALUE_OFFSET + gears*SLOWSLOWHALF,RC_CH_VALUE_OFFSET + gears*SLOWSLOWHALF,rad);
		break;
		
		case (KEY_PRESSED_OFFSET_S | KEY_PRESSED_OFFSET_A):
					move(RC_CH_VALUE_OFFSET - gears*SLOWSLOWHALF,RC_CH_VALUE_OFFSET - gears*SLOWSLOWHALF,rad);
		break;
		
		case ( KEY_PRESSED_OFFSET_S | KEY_PRESSED_OFFSET_D):
					move(RC_CH_VALUE_OFFSET + gears*SLOWSLOWHALF,RC_CH_VALUE_OFFSET - gears*SLOWSLOWHALF,rad);
		break;
		
		case ( KEY_PRESSED_OFFSET_W) :
					move(RC_CH_VALUE_OFFSET,RC_CH_VALUE_OFFSET + gears*SLOWSLOW,rad);
		break;
		
		case ( KEY_PRESSED_OFFSET_S) :
					move(RC_CH_VALUE_OFFSET,RC_CH_VALUE_OFFSET - gears*SLOWSLOW,rad);
		break;
		
		case ( KEY_PRESSED_OFFSET_A) :
					move(RC_CH_VALUE_OFFSET - gears*SLOWSLOW,RC_CH_VALUE_OFFSET,rad);
		break;
		
		case ( KEY_PRESSED_OFFSET_D) :
					move(RC_CH_VALUE_OFFSET + gears*SLOWSLOW,RC_CH_VALUE_OFFSET,rad);
		break;
		
		default: 
					move(RC_CH_VALUE_OFFSET,RC_CH_VALUE_OFFSET,rad);  //����0.2
		break;
		}
}
//gear
//���ܣ���λ���ú���
//������keyboardvalue
//����ֵ���� �ı��˵�λֵ
void gearChange(uint16_t keyboardvalue)   //��Shift�Ļ��Ͳ��õ�λ
{
		switch(keyboardvalue & (KEY_PRESSED_OFFSET_Q| KEY_PRESSED_OFFSET_E | KEY_PRESSED_OFFSET_CTRL))
	  {
			case KEY_PRESSED_OFFSET_CTRL :    //CTRLΪ����ģʽ
			{
				    delay_ms(20);
				    if((keyboardvalue & KEY_PRESSED_OFFSET_CTRL) == KEY_PRESSED_OFFSET_CTRL)
				    {
							}

			} 
      break;
		}
}

int16_t followValCal(void)
{
	int16_t followVal = 0;
	int16_t GimbalPositionYaw = current_position_203;
	followVal =  -( GimbalPositionYaw - MIDDLE_YAW )* 1.5;					//���̸���
	if(abs(followVal)<15) followVal = 0; //350
	if(followVal > 900) followVal = 900;
	if(followVal <-900) followVal =-900;
	
	return followVal;
}

