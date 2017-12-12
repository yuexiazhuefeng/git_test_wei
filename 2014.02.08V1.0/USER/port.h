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
#ifndef _PORT_H
#define _PORT_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#define   GPS_LED_OFF  PCout(6)=0
#define   GPS_LED_ON   PCout(6)=1
#define   GPRS_LED_OFF PCout(8)=0
#define   GPRS_LED_ON  PCout(8)=1

extern void PORT_Init(void);
#endif


