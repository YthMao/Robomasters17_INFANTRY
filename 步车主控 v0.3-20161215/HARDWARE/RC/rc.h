#ifndef __RC_H__
#define __RC_H__


#include "stm32f4xx.h"
//#include "sys.h"

#define RC_CH_VALUE_MIN ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)

/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)

/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<7)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<5)

/* ----------------------- Data Struct ------------------------------------- */
typedef struct 
{
		struct
		{
				uint16_t ch0;//ÓÒÒ¡¸Ë×óÓÒ
				uint16_t ch1;//ÓÒÒ¡¸ËÉÏÏÂ
				uint16_t ch2;//×óÒ¡¸Ë×óÓÒ
				uint16_t ch3;//×óÒ¡¸ËÉÏÏÂ
				uint8_t s1;	//×óÇ°²¦Å¥
				uint8_t s2; //ÓÒÇ°²¦Å¥
		}rc;
		struct
		{
				int16_t x;//Êó±êxÖá
				int16_t y;//Êó±êyÖá
				int16_t z;
				uint8_t press_l; //Êó±ê×ó¼ü
				uint8_t press_r;// Êó±êÓÒ¼ü
		}mouse;
		struct
		{
				uint16_t v; 
//			Ã¿¸ö°´¼ü¶ÔÓ¦Ò»¸öbit£¬ÈçÏÂËùÊ¾
//			Bit0 ©\©\©\©\©\©\©\ W ¼ü
//			Bit1 ©\©\©\©\©\©\©\ S ¼ü
//			Bit2 ©\©\©\©\©\©\©\ A ¼ü
//			Bit3 ©\©\©\©\©\©\©\ D ¼ü
//			Bit4 ©\©\©\©\©\©\©\ Shift ¼ü
//			Bit5 ©\©\©\©\©\©\©\ Ctrl ¼ü
//			Bit6 ©\©\©\©\©\©\©\ Q ¼ü
//			Bit7 ©\©\©\©\©\©\©\ E ¼ü
		}key;
		
		uint16_t resv;
		
}RC_Ctl_t;

/* ----------------------- Internal Data ----------------------------------- */
extern volatile unsigned char sbus_rx_buffer[18];
extern RC_Ctl_t RC_Ctl;
extern RC_Ctl_t RC_Ex_Ctl; 
extern uint8_t test_rc;
void RC_Init(void);
void DMA1_Stream5_IRQHandler(void);
void TestRc(void);
void ItoE(void);
//void Relative_Cal(void);
//void Relative_Cal_key(void);

#endif
