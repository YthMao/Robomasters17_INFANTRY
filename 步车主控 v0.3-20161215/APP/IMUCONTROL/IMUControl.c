/************************************************************
 *File		:	IMUControl.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: 	MPU6050 control init and loop
 ************************************************************/
#include "IMUControl.h"
#include "mpu6050_process.h"
#include "mpu6050_driver.h"
#include "delay.h"
#include "buzzer.h"
#include "usart1.h"

static int IMU_count = 0;

void IMUControlInit(void)
{
	int i = 0;
	while(MPU6050_Initialization() == 0xff) 
    {
        i++;     //如果一次初始化没有成功，那就再来一次                     
        if(i>20) //如果初始化一直不成功，那就没希望了，进入死循环，蜂鸣器一直叫
        {
            while(1) 
            {
                delay_ms(500);
								printf("MPU6050 ERROR\n\r");
								Buzzer_toggle();
            }
        } 
				delay_ms(200);	
    }    
}

void IMUControlLoop(void)
{
	IMU_count++;
	MPU6050_ReadData(1);    //读
	MPU6050_Data_Filter();  //过滤
	MPU6050_Angle_Calculate(	MPU6050_Real_Data.Gyro_X*Gyro_Gr,        //此处加float后精度可能会有影响
														MPU6050_Real_Data.Gyro_Y*Gyro_Gr,
														MPU6050_Real_Data.Gyro_Z*Gyro_G,
														MPU6050_Real_Data.Accel_X,
														MPU6050_Real_Data.Accel_Y,
														MPU6050_Real_Data.Accel_Z);
	IMU_count = 0;
	
}
