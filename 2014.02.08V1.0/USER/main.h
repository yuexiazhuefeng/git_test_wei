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

extern u32 gps_counter ;//GPS��ʼ�����ȴ�2S����
extern u16 Debug_waitcounter ;
extern u16 oil_sampling_time ;  //L/H,����ʱ�䣬��Ϊ3��
extern u8  read_GSM_time ; //��GSM�ļ��ʱ��
extern u8  Read_oilmass_time ; //��ȡʣ��������ʱ��
//GLOBAL_Flag Flag_all

extern u16 timer2_100ms ;      //��ʱ��2  100ms��ʱ��
extern void Hardware_Init(void);
extern void wait(u32 count);
extern void SPI_check(void);
extern void Usart2_SendData(u8 * psend, u8 length);
extern void Debug(void);
extern void Parameter_Init(void); //����ʱ��������ʼ������
#endif




