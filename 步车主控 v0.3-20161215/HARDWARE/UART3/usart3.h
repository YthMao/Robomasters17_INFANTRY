#ifndef __USART3_H__
#define __USART3_H__

#include "stm32f4xx.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
//#include "com2mani.h"
//#include "RMVariables.h" 
#define LBUF 30
 extern u8  USART_RX_BUF3[LBUF];  
 extern u16 USART_RX_STA3;
 extern u8	USART_RX_BUF3_CP[LBUF];
void USART3_Configuration(void);
void USART3_SendChar(unsigned char b);
void USART3_IRQHandler(void);
//called by main()
int16_t getYunTaiAdjustYaw(void);
int16_t getYunTaiAdjustPitch(void);
void setMotoParameter(void);
void resetIsReceive(void);
uint8_t getIsReceive(void);
void RS232_VisualScope( USART_TypeDef* USARTx, u8 *pWord, u16 Len );
      		
void DMA1_Stream1_IRQHandler(void);
void USART3_DMA_Init(void);
#endif
