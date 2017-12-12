#ifndef __DEBUG_H
#define __DEBUG_H


#include "stm32f10x.h"
#include "usart.h"

#define Version_Len 10

extern void SystemReset(void);
extern void D_data_deal(void);
extern u16 Debug_waitcounter ;
extern u8 Mid[6];
extern u16 Debug_Timeout ;//5min=30000定时器，设置模式维持5分钟，5分钟之后自动退出
#endif


