/******************************************************************************************
*  File Name                             :prot.h
*  Copyright                             :2014.2 wei
*  Module Name                           :Init port
*
*  CPU                                   :STM32F103RC
*  RTOS                                  :NULL
*
*  Create Date                           :2014/2/24
*  Author                                :wei
*  Abstract Dscription                   :
*
*
*----------------------Revision History-----------------------------
*  No   Version   Date        Revised By          Item           Description
*
*
*******************************************************************************/

#ifndef __TIMER2_H
#define __TIMER2_H

#include "debug.h"

void TIM2_Int_Init(u16 arr,u16 psc);


extern u8 ReTimeout ; //debug mode 延时器
extern u8 ms100; //100ms定时器，将清狗动作放到while外面。
extern u8 USART3_Rx_timer ; //定时30ms，用来清空串口3接收缓存，防止干扰，和一些不必要的返回数据
extern u8 read_GSM_time ; //读GSM的间隔时间
extern u16 authentication_count ; //终端重复注册时间周期 ，先设为10s

extern u8 jinji_xiaodou_timer  ;//紧急报警消抖定时器
extern u8 OneSecond_Timer  ; //开辟一个1S定时器，
//extern u16 FiveSecond ; //5S定时器
extern u16 PB0_high_timer;  //PB0高电平计时
extern u16 PB0_low_timer;   //PB0低电平计时
#endif

