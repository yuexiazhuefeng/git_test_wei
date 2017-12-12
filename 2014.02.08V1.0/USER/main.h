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
#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"
#include "timer2.h"
#include "iwdg.h"
#include "sys.h"
#include "delay.h"
#include "flash.h"
#include "usart.h"
#include "GSM.h"
#include "OBD.h"
#include "alarm.h"
#include "upload.h"
#include "sys_struct.h"
#include "exti.h"

#include "Test.h"
#define TRUE   1
#define FALSE  0

extern GLOBALstruct_Flag  Flag_all;

extern u32 gps_counter ;//GPS初始化，等待2S计数
extern u16 Debug_waitcounter ;
extern u16 oil_sampling_time ;  //L/H,采样时间，设为3秒
extern u8  read_GSM_time ; //读GSM的间隔时间
extern u8  Read_oilmass_time ; //读取剩余油量定时器
//GLOBAL_Flag Flag_all

extern u16 timer2_100ms ;      //定时器2  100ms定时器
extern void Hardware_Init(void);
extern void wait(u32 count);
extern void SPI_check(void);
extern void Usart2_SendData(u8 * psend, u8 length);
extern void Debug(void);
extern void Parameter_Init(void); //开机时，参数初始化程序，
#endif




