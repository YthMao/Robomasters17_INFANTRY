/************************************************************
 *File		:	mpu6050_process.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: 	Configuration , Data Filter and Angle Calculate
 ************************************************************/
#include "mpu6050_process.h"
#include "mpu6050_driver.h"
#include <math.h>
#include "sys.h"
#include "main.h"

//#define Angel_Pitch_Cal

ACCEL_AVERAGE_DATA   Accel_Raw_Average_Data; 
GYRO_RADIAN_DATA     Gyro_Radian_Data;
MPU6050_ANGLE        MPU6050_Angle;

//MPU6050_Configuration
//���ܣ����������ݳ�ʼ��
//�������� 
//����ֵ��void
void MPU6050_Configuration(void)
{
	int i = 0;
	while(MPU6050_Initialization() == 0xff) 
    {
        i++;     //���һ�γ�ʼ��û�гɹ����Ǿ�����һ��                     
        if(i>20) //�����ʼ��һֱ���ɹ����Ǿ�ûϣ���ˣ�������ѭ����������һֱ��
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


//MPU6050_Data_Filter
//���ܣ������������˲�����
//�������� 
//����ֵ��void
void MPU6050_Data_Filter(void)  // this*0.01 + (��һ���õ�)last *0.99 Ҫ��
{
    unsigned int i=0;
    static unsigned int first_flag = 0;
    static unsigned int filter_cnt = 0;    //������ٶȼ��˲��Ĵ���
    
		//���ٶ��Ȳ���
//    long temp_accel_x = 0; //������ż��ٶȼ�X��ԭ�����ݵ��ۼӺ�
//    long temp_accel_y = 0; //������ż��ٶȼ�Y��ԭ�����ݵ��ۼӺ�
//    long temp_accel_z = 0; //������ż��ٶȼ�Z��ԭ�����ݵ��ۼӺ�
		long temp_gyro_x = 0; 
    long temp_gyro_y = 0; 
    long temp_gyro_z = 0;	
	
//    static short accel_x_buffer[10] = {0}; //������ż��ٶȼ�X�����10�����ݵ�����
//    static short accel_y_buffer[10] = {0}; //������ż��ٶȼ�Y�����10�����ݵ�����
//    static short accel_z_buffer[10] = {0}; //������ż��ٶȼ�Z�����10�����ݵ�����
    static short gyro_x_buffer[5] = {0}; 
    static short gyro_y_buffer[5] = {0}; 
    static short gyro_z_buffer[5] = {0}; 
		
    if(first_flag == 0) //�����һ�ν����ú��������������ƽ����������г�ʼ��
    {
        first_flag = 1; //�Ժ��ٽ���
        for(i=0;i<5;i++)
        {
//            accel_x_buffer[i] = MPU6050_Raw_Data.Accel_X;
//            accel_y_buffer[i] = MPU6050_Raw_Data.Accel_Y;
//            accel_z_buffer[i] = MPU6050_Raw_Data.Accel_Z;
						gyro_x_buffer[i]  = MPU6050_Raw_Data.Gyro_X;
            gyro_y_buffer[i]  = MPU6050_Raw_Data.Gyro_Y;
            gyro_z_buffer[i]  = MPU6050_Raw_Data.Gyro_Z;
						
        }
    }
    else  //������ǵ�һ����
    {
//        accel_x_buffer[filter_cnt] = MPU6050_Raw_Data.Accel_X;
//        accel_y_buffer[filter_cnt] = MPU6050_Raw_Data.Accel_Y;
//        accel_z_buffer[filter_cnt] = MPU6050_Raw_Data.Accel_Z;   
			
				gyro_x_buffer[filter_cnt]  = MPU6050_Raw_Data.Gyro_X;
        gyro_y_buffer[filter_cnt]  = MPU6050_Raw_Data.Gyro_Y;
        gyro_z_buffer[filter_cnt]  = MPU6050_Raw_Data.Gyro_Z;	
			
        filter_cnt ++;
        if(filter_cnt == 5)
        {
            filter_cnt = 0;
        }        
    }
    
    for(i=0;i<5;i++)
    {
//        temp_accel_x += accel_x_buffer[i];
//        temp_accel_y += accel_y_buffer[i];
//        temp_accel_z += accel_z_buffer[i];
				temp_gyro_x += gyro_x_buffer[i];
				temp_gyro_y += gyro_y_buffer[i];
				temp_gyro_z += gyro_z_buffer[i];
    }
    
//    Accel_Raw_Average_Data.X = (float)temp_accel_x / 10.0f;
//    Accel_Raw_Average_Data.Y = (float)temp_accel_y / 10.0f;
//    Accel_Raw_Average_Data.Z = (float)temp_accel_z / 10.0f;
    
		MPU6050_Real_Data.Gyro_X = (float)temp_gyro_x / 5.0f;
		MPU6050_Real_Data.Gyro_Y = (float)temp_gyro_y / 5.0f;
		MPU6050_Real_Data.Gyro_Z = (float)temp_gyro_z / 5.0f;
		
		MPU6050_Real_Data.Gyro_X = -(float)(MPU6050_Raw_Data.Gyro_X )/32.8f;     //��datasheet 32 of 47
    MPU6050_Real_Data.Gyro_Y = -(float)(MPU6050_Raw_Data.Gyro_Y )/32.8f;     //��datasheet 32 of 47
    MPU6050_Real_Data.Gyro_Z =  (float)(MPU6050_Raw_Data.Gyro_Z )/32.8f;     //��datasheet 32 of 47
//    Gyro_Radian_Data.X = (float)(MPU6050_Real_Data.Gyro_X  * (3.14159265/180.0));
//    Gyro_Radian_Data.Y = (float)(MPU6050_Real_Data.Gyro_Y  * (3.14159265/180.0));
//    Gyro_Radian_Data.Z = (float)(MPU6050_Real_Data.Gyro_Z  * (3.14159265/180.0));
	}

#define Kp 0.3f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.008f                          // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.0005f                   // half the sample period�������ڵ�һ��

	
//MPU6050_Angle_Calculate
//���ܣ������������ݾ���Ԫ��������pitch�Ƕȣ���ֱ�ӻ��ֵó�yaw�Ƕ�
//�������� 
//����ֵ������0
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error
u8 MPU6050_Angle_Calculate( float gx,float gy,float gz,float ax,float ay,float az)
{
		float gz_input;
	
#ifdef	Angel_Pitch_Cal
//  float norm;
//  float hx, hy, hz, bx, bz;
//  float vx, vy, vz;// wx, wy, wz;
//  float ex, ey, ez;


  // �Ȱ���Щ�õõ���ֵ���
//  float q0q0 = q0*q0;
//  float q0q1 = q0*q1;
//  float q0q2 = q0*q2;
////  float q0q3 = q0*q3;
//  float q1q1 = q1*q1;
////  float q1q2 = q1*q2;
//  float q1q3 = q1*q3;
//  float q2q2 = q2*q2;
//  float q2q3 = q2*q3;
//  float q3q3 = q3*q3;
	
#endif
	gz_input=gz/Gyro_Gr*AtR;
	if(ax*ay*az==0)
 		return 0;
		
#ifdef	Angel_Pitch_Cal
//  norm = sqrt(ax*ax + ay*ay + az*az);       //acc���ݹ�һ��
//  ax = ax / norm;
//  ay = ay / norm;
//  az = az / norm;

//  // estimated direction of gravity and flux (v and w)              �����������������/��Ǩ
//  vx = 2*(q1q3 - q0q2);												//��Ԫ����xyz�ı�ʾ
//  vy = 2*(q0q1 + q2q3);
//  vz = q0q0 - q1q1 - q2q2 + q3q3 ;

//  // error is sum of cross product between reference direction of fields and direction measured by sensors
//  ex = (ay*vz - az*vy) ;                           					 //�������������õ���־������
//  ey = (az*vx - ax*vz) ;
//  ez = (ax*vy - ay*vx) ;

//  exInt = exInt + ex * Ki;								          //�������л���
//  eyInt = eyInt + ey * Ki;
//  ezInt = ezInt + ez * Ki;

//  // adjusted gyroscope measurements
//  gx = gx + Kp*ex + exInt;					   							//�����PI�󲹳��������ǣ����������Ư��
//  gy = gy + Kp*ey + eyInt;
//  gz = gz + Kp*ez + ezInt;				   							//�����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ����������Լ�

//  // integrate quaternion rate and normalise						   //��Ԫ�ص�΢�ַ���
//  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
//  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
//  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
//  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

//  // normalise quaternion
//  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
//  q0 = q0 / norm;
//  q1 = q1 / norm;
//  q2 = q2 / norm;
//  q3 = q3 / norm;
  //ŷ���������̬��
//  MPU6050_Angle.Pitch  = asin(-2 * q1q3 + 2 * q0q2)* 57.3; // pitch
#endif
//  MPU6050_Angle.Roll = atan2(2 * q2q3 + 2 * q0q1, -2 * q1q1 - 2 * q2q2 + 1)* 57.3; // roll

  MPU6050_Angle.Yaw += 2*gz_input*halfT; // yaw
	return 0;
}

	
//CalculateAngleRateGyro
//���ܣ����ַ���YAW��
//�������� 
//����ֵ��AngleGyro�Ƕ�ֵ
float CalculateAngleRateGyro(float GyroX)       //���ַ���YAW��
{
	float AngleRateGyro;
	AngleRateGyro = - GyroX ;//* PI/ 180.00;
	AngleGyro += AngleRateGyro * dt;
	return AngleGyro;
}
