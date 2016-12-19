#ifndef __BUZZER_H__
#define __BUZZER_H__
#include "sys.h"

void Buzzer_Init(void);
u8 Start(void);
void Buzzer_toggle(void);
#define BUZZER_ON()      GPIO_SetBits(GPIOC,GPIO_Pin_12)
#define BUZZER_OFF()     GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define BUZZER_TOGGLE()  GPIO_ToggleBits(GPIOC,GPIO_Pin_12)

#endif
