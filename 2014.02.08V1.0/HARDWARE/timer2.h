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


extern u8 ReTimeout ; //debug mode ��ʱ��
extern u8 ms100; //100ms��ʱ�������幷�����ŵ�while���档
extern u8 USART3_Rx_timer ; //��ʱ30ms��������մ���3���ջ��棬��ֹ���ţ���һЩ����Ҫ�ķ�������
extern u8 read_GSM_time ; //��GSM�ļ��ʱ��
extern u16 authentication_count ; //�ն��ظ�ע��ʱ������ ������Ϊ10s

extern u8 jinji_xiaodou_timer  ;//��������������ʱ��
extern u8 OneSecond_Timer  ; //����һ��1S��ʱ����
//extern u16 FiveSecond ; //5S��ʱ��
extern u16 PB0_high_timer;  //PB0�ߵ�ƽ��ʱ
extern u16 PB0_low_timer;   //PB0�͵�ƽ��ʱ
#endif

