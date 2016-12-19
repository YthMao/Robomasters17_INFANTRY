/************************************************************
 *File		:	can2.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: 	Use CAN2 to control Gimbal motor
								bound 1Mbps		YAW_ID:203,205		PITCH_ID:	201,206
 ************************************************************/

#include "can2.h"
#include "lcd.h"
CanRxMsg can2rx_message;
CanRxMsg rx_message;

int16_t current_position_201;
int16_t current_position_203;
u8 can2rx_received = 0;
u8 flag2 = 0;

//CAN2_Mode_Init
//功能：CAN2初始化函数
//参数：传入tsjw,tbs2,tbs1,brp,mode 决定CAN2的模式和波特率
//返回值：0
void CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
    GPIO_InitTypeDef 				GPIO_InitStructure; 
	  CAN_InitTypeDef        	CAN_InitStructure;
  	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
   	NVIC_InitTypeDef  			NVIC_InitStructure;


    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 ;    //B12 B13
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    CAN_DeInit(CAN2);
    CAN_StructInit(&CAN_InitStructure);

    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = ENABLE;    
    CAN_InitStructure.CAN_AWUM = DISABLE;    
    CAN_InitStructure.CAN_NART = ENABLE;    
    CAN_InitStructure.CAN_RFLM = DISABLE;    
    CAN_InitStructure.CAN_TXFP = DISABLE;     
    CAN_InitStructure.CAN_Mode = mode; 
    CAN_InitStructure.CAN_SJW  = tsjw;	//CAN_SJW_2tq ; //CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = tbs1;		//;//CAN_BS1_6tq;
    CAN_InitStructure.CAN_BS2 = tbs2;		//;//CAN_BS2_7tq;
    CAN_InitStructure.CAN_Prescaler = brp;   // 7 //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN2, &CAN_InitStructure);
    
    CAN_FilterInitStructure.CAN_FilterNumber=14;
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;//the message which pass the filter save in fifo0
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
    
    CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
}


//CAN2_RX0_IRQHandler
//功能：CAN2中断函数
//参数：读取Pitch(201、206) Yaw(203、205)电机值！！
//返回值：无		    
void CAN2_RX0_IRQHandler(void)
{   
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET) 
		{
				CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
        CAN_Receive(CAN2, CAN_FIFO0, &rx_message);       
                
        if(rx_message.StdId == 0x206)    // 201 206 是pitch
        {             
             //获得云台电机0x201的码盘值
             current_position_201 = (rx_message.Data[0]<<8) | rx_message.Data[1];					
        }
        if(rx_message.StdId == 0x202)
        { 
						
             //获得云台电机0x202的码盘值           
        }		
        if(rx_message.StdId == 0x205)               //205 和203是yaw
        { 
             current_position_203 = (rx_message.Data[0]<<8) | rx_message.Data[1];	
             //获得云台电机0x203的码盘值  
        }
    }
}

//Cmd_ESC
//功能：CAN2云台电机控制程序
//参数：传入pitch、yaw电机控制值
//返回值：无	
void Cmd_ESC(int16_t current_201,int16_t current_202,int16_t current_203)
{
    CanTxMsg tx_message;
    
    tx_message.StdId = 0x1ff;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = (unsigned char)(current_203 >> 8);
    tx_message.Data[1] = (unsigned char)current_203;
    tx_message.Data[2] = (unsigned char)(current_201 >> 8);
    tx_message.Data[3] = (unsigned char)current_201;
    tx_message.Data[4] = 0x00;
    tx_message.Data[5] = 0x00;
    tx_message.Data[6] = 0x00;
    tx_message.Data[7] = 0x00;
    
    CAN_Transmit(CAN2,&tx_message);
}



