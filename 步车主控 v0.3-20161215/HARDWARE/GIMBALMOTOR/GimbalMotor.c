/************************************************************
 *File		:	GimbalMotor.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: 	Control Gimbal Motor by CAN2. 
								CAN2 bound should be set at 1Mbps
								This file contects the function for motor action.						
 ************************************************************/

#include "GimbalMotor.h"
#include "sys.h"
#include "delay.h"
#include "can2.h"
u8 MotorState201=0;//0-ֹͣת�� 1-��ת 2-��ת
u8 MotorState203=0;//0-ֹͣת�� 1-��ת 2-��ת
s16 MotorCurrent201 = 0;
s16 MotorCurrent203 = 0;

/*************************************************************************
                              ������Ƶײ����       
ע�����ĵ��еĳ������Pit��Yaw�������Ӧ�Ĳ��֣�
		���ĵ���ֻ����Pit���֣�Yaw��ͬ												
*************************************************************************/

//MotorCurrentLegalize
//���ܣ��������ֵ�淶������
//��������
//����ֵ���淶���ĵ��ֵ	
float MotorCurrentLegalize(float MotorCurrent)
{
	return MotorCurrent<-4500?-4500:(MotorCurrent>4500?4500:MotorCurrent);
}


////////////////*����ΪPitch�������ֿ��Ƴ���*////////////////////

//MotorStopPit(YAW)
//���ܣ�PIT���ͣת���� ͨ��Cmd_ESC()ʵ�֣�������CAN�ź�
//��������
//����ֵ��void	
void MotorStopPit()//���ͣת
{
	MotorCurrent201 = 0;//PWM����Ϊ0
  Cmd_ESC(MotorCurrent201,0,MotorCurrent203);
	MotorState201=0;
}

//MotorRotatePit(YAW)
//���ܣ�PIT�����ת���� ͨ��Cmd_ESC()ʵ�֣�������CAN�ź�
//      ����MotorState201�����жϵ����ǰ����ת��������Ҫ��ת������Ҫ��ͣһ��
//��������
//����ֵ��void	
void MotorRotatePit()//�����ת
{
	if(MotorCurrent201==0)
	{
		 MotorStopPit();
		 return;//���PWMΪ0��ͣתֱ�ӷ���
	} 
	
	if(MotorCurrent201>0)//�������ӵ�
	{
		Cmd_ESC(MotorCurrent201,0,MotorCurrent203);       //��YAW��һ��ִ��
		MotorState201 = 1;//�������ӵ�
	}
	else//�������ӵ�
	{
		Cmd_ESC(MotorCurrent201,0,MotorCurrent203);
		MotorState201 = 2;//�������ӵ�
	}
}

//MotorSetCurrentPit(YAW)
//���ܣ�����PIT�����תֵ ͨ�� MotorRotatePit()����ʵ��
//      ����MotorState201�����жϵ����ǰ����ת��������Ҫ��ת������Ҫ��ͣһ��
//��������
//����ֵ��void	
void MotorSetCurrentPit(s16 MotorCurrent)
{

	if(MotorCurrent==0)
	{
		MotorStopPit();
		return;
	}
	if((MotorCurrent^MotorCurrent201)&0x8000)//�����ת���ҷ���ı䣬��ɲ������ת������ֱ�Ӹı�PWM
	{	
		MotorStopPit();
		delay_ms(1);//ɲ�����ȴ�1ms
	}
	MotorCurrent201=MotorCurrent;     //�����ж��Ƿ���ת��
	MotorRotatePit();     //��Ҫִ��
}



////////////////*����ΪYaw�������ֿ��Ƴ���*////////////////////

void MotorStopYaw()//���ͣת
{
	MotorCurrent203 = 0;//PWM����Ϊ0
  Cmd_ESC(MotorCurrent201,0,MotorCurrent203);
	MotorState203=0;
}

void MotorRotateYaw()//�����ת    YAW��
{
	if(MotorCurrent203==0)
	{
		 MotorStopYaw();
		 return;//���PWMΪ0��ͣתֱ�ӷ���
	} 
	
	if(MotorCurrent203>0)//�������ӵ�
	{
		Cmd_ESC(MotorCurrent201,0,MotorCurrent203);
		MotorState203 = 1;//�������ӵ�
	}
	else//�������ӵ�
	{
		Cmd_ESC(MotorCurrent201,0,MotorCurrent203);
		MotorState203 = 2;//�������ӵ�
	}
}

void MotorSetCurrentYaw(s16 MotorCurrent)
{

	if(MotorCurrent==0)
	{
		MotorStopYaw();
		return;
	}
	if((MotorCurrent^MotorCurrent203)&0x8000)//�����ת���ҷ���ı䣬��ɲ������ת������ֱ�Ӹı�PWM
	{	
		MotorStopYaw();
		delay_ms(1);//ɲ�����ȴ�1ms
	}
	MotorCurrent203=MotorCurrent;     //�����ж��Ƿ���ת��
	MotorRotateYaw();
}
