/************************************************************
 *File		:	usart3.c
 *Author	:	@YangTianhao ,490999282@qq.com
 *Version	: v0.1
 *Update	: 2016.12.11
 *Description: 	Usart3, for  view data communication
 ************************************************************/
#include "usart3.h"

/*-----USART3_TX-----PB10-----*/
/*-----USART3_RX-----PB11-----*/

 u8  USART_RX_BUF3[LBUF];  
 u8	 USART_RX_BUF3_CP[LBUF];
 u16 USART_RX_STA3;

void USART3_Configuration(void)
{
		USART_InitTypeDef usart3;
		GPIO_InitTypeDef  gpio;
		NVIC_InitTypeDef  nvic;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

		GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 

		gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		gpio.GPIO_Mode = GPIO_Mode_AF;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_Speed = GPIO_Speed_100MHz;
		gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOB,&gpio);

		usart3.USART_BaudRate = 9600; //115200; 		//@hiyangdong(2015?4?11? 19:16:55)??????38400,?????????
		usart3.USART_WordLength = USART_WordLength_8b;
		usart3.USART_StopBits = USART_StopBits_1;
		usart3.USART_Parity = USART_Parity_No;
		usart3.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
		usart3.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART3,&usart3);

		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);  

		USART_Cmd(USART3,ENABLE);

		nvic.NVIC_IRQChannel = USART3_IRQn;
		//nvic.NVIC_IRQChannelPreemptionPriority = 1;
		// nvic.NVIC_IRQChannelSubPriority = 1;
		nvic.NVIC_IRQChannelPreemptionPriority = 2;
		nvic.NVIC_IRQChannelSubPriority = 3;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
}

void USART3_SendChar(unsigned char b)
{
  USART_SendData(USART3,b);  
	while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET){};
}


#define RxCountMax  10
uint8_t RxBuf[RxCountMax] ;
uint8_t RxCnt = 0;
static int16_t Yaw_adjust   = 0;
static int16_t Pitch_adjust = 0;
#define DebugCountMax  10
static uint8_t isReceived;

void resetIsReceive(){isReceived=0;}
uint8_t getIsReceive(){return isReceived;}

static u16 RS232_VisualScope_CRC16( u8 *Array, u16 Len )
{
	u16 USART_IX, USART_IY, USART_CRC;

	USART_CRC = 0xffff;
	for(USART_IX=0; USART_IX<Len; USART_IX++) {
		USART_CRC = USART_CRC^(u16)(Array[USART_IX]);
		for(USART_IY=0; USART_IY<=7; USART_IY++) {
			if((USART_CRC&1)!=0)
				USART_CRC = (USART_CRC>>1)^0xA001;
			else
				USART_CRC = USART_CRC>>1;
		}
	}
	return(USART_CRC);
}


void RS232_VisualScope( USART_TypeDef* USARTx, u8 *pWord, u16 Len )
{
	u8 i = 0;
	u16 Temp = 0;

	Temp = RS232_VisualScope_CRC16(pWord, Len);
	pWord[8] = Temp&0x00ff;
	pWord[9] = (Temp&0xff00)>>8;

	for(i=0; i<10; i++) {
		USART_SendData(USARTx, (uint8_t)*pWord);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
		pWord++;
	}
}


/*USART3 DMA test*/
void USART3_DMA_Init(void)
{
/* -------------- Enable Module Clock Source ----------------------------*/
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA1, ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
GPIO_PinAFConfig(GPIOB,GPIO_PinSource11, GPIO_AF_USART3);  //PB11 RX  PB10 TX
GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
/* -------------- Configure GPIO ---------------------------------------*/
		{
				GPIO_InitTypeDef gpio;
				USART_InitTypeDef usart3;
				gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
				gpio.GPIO_Mode = GPIO_Mode_AF;
				gpio.GPIO_OType = GPIO_OType_PP;
				gpio.GPIO_Speed = GPIO_Speed_100MHz;
				gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
				GPIO_Init(GPIOB, &gpio);
			
				USART_DeInit(USART3);
				usart3.USART_BaudRate = 115200;
				usart3.USART_WordLength = USART_WordLength_8b;
				usart3.USART_StopBits = USART_StopBits_1;
				usart3.USART_Parity = USART_Parity_No ;    //USART_Parity_Even;
				usart3.USART_Mode = USART_Mode_Tx|USART_Mode_Rx; //USART_Mode_Rx;
				usart3.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
				USART_Init(USART3,&usart3);
				USART_Cmd(USART3,ENABLE);
				USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
		}		

		/* -------------- Configure NVIC ---------------------------------------*/
		{
				NVIC_InitTypeDef nvic;
				nvic.NVIC_IRQChannel = DMA1_Stream1_IRQn;      //Stream 1 
				nvic.NVIC_IRQChannelPreemptionPriority = 2;
				nvic.NVIC_IRQChannelSubPriority = 3;
				nvic.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&nvic);
		}
		
		/* -------------- Configure DMA -----------------------------------------*/
		{
				DMA_InitTypeDef dma;
				DMA_DeInit(DMA1_Stream1);                      //DMA1_stream1 channel 4
				dma.DMA_Channel = DMA_Channel_4;
				dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
				dma.DMA_Memory0BaseAddr = (uint32_t)USART_RX_BUF3;    //储存
				dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
				dma.DMA_BufferSize = LBUF;       //要改？
				dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
				dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
				dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
				dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
				dma.DMA_Mode = DMA_Mode_Circular;
				dma.DMA_Priority = DMA_Priority_VeryHigh;
				dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
				dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
				dma.DMA_MemoryBurst = DMA_Mode_Normal;
				dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
				DMA_Init(DMA1_Stream1,&dma);
				DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);
				DMA_Cmd(DMA1_Stream1,ENABLE);
		}	
}

void DMA1_Stream1_IRQHandler(void)
{
		if(DMA_GetITStatus(DMA1_Stream1, DMA_IT_TCIF1))
		{
			//memset(USART_RX_BUF3,0,strlen(USART_RX_BUF3));    //不清0的效果会好点
			DMA_ClearFlag(DMA1_Stream1, DMA_IT_TCIF1);
			DMA_ClearITPendingBit(DMA1_Stream1, DMA_IT_TCIF1);	
			
		}
}
