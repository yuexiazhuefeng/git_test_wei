#ifndef __ALARM_H
#define __ALARM_H

#include "stm32f10x.h"
#include "sys.h"
#include "iwdg.h"
#include "usart.h"
#include "timer2.h"
#include "OBD.h"
#include "main.h"








extern void alarm(void);
extern u16 JinJi_liushui ;  //保存上发紧急报警的消息流水号
extern u8  JinJi_Num ;  //紧急报警发送次数，
extern u16 JinJi_JianGe ;//连续按下紧急报警时，需要间隔一段时间在报
extern u16 Shake_continu_timer ;  //连续按下紧急报警时，需要间隔一段时间在报
extern u8 Shake_Num ;      //震动电平下降沿次数，计次

extern u16 Shake_LianXuCount ; //防止震动报警太频繁
extern u16 Shake_JianGE ;  //限制震动报警的最短时间
extern u8 Shake_sendNum ;  //震动报警
extern u16 Shake_liushui ; //记录上发震动报警的消息流水号
#endif
