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
extern u16 JinJi_liushui ;  //�����Ϸ�������������Ϣ��ˮ��
extern u8  JinJi_Num ;  //�����������ʹ�����
extern u16 JinJi_JianGe ;//�������½�������ʱ����Ҫ���һ��ʱ���ڱ�
extern u16 Shake_continu_timer ;  //�������½�������ʱ����Ҫ���һ��ʱ���ڱ�
extern u8 Shake_Num ;      //�𶯵�ƽ�½��ش������ƴ�

extern u16 Shake_LianXuCount ; //��ֹ�𶯱���̫Ƶ��
extern u16 Shake_JianGE ;  //�����𶯱��������ʱ��
extern u8 Shake_sendNum ;  //�𶯱���
extern u16 Shake_liushui ; //��¼�Ϸ��𶯱�������Ϣ��ˮ��
#endif
