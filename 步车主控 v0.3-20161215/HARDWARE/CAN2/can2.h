#ifndef __CAN2_H__
#define __CAN2_H__
#include "stm32f4xx.h"
//#include "sys.h"

extern int16_t ch0;//ÓÒÒ¡¸Ë×óÓÒ
extern int16_t ch1;//ÓÒÒ¡¸ËÉÏÏÂ
extern int16_t ch2;//×óÒ¡¸Ë×óÓÒ
extern int16_t ch3;//×óÒ¡¸ËÉÏÏÂ 
extern int8_t s1;//Êó±êxÖá
extern int8_t s2;//Êó±êyÖá
extern int8_t press_l;//Êó±ê×ó¼ü°´ÏÂ
extern int8_t press_r;//Êó±êÓÒ¼ü°´ÏÂ
extern int16_t mouse_y;

void CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);
void CAN2_Send_Msg(uint8_t candata1h,uint8_t candata1l);

void Cmd_ESC(int16_t current_201,int16_t current_202,int16_t current_203);

extern int16_t current_position_201;
extern int16_t current_position_203;


#endif 

