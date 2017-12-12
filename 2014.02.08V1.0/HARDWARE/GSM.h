

#ifndef __GSM_H
#define __GSM_H

#include <stdlib.h>
#include "stm32f10x.h"
#include "sys.h"
#include "iwdg.h"
#include "usart.h"
#include "upload.h"
#include "sys_struct.h"
#include "alarm.h"


#define AT_Len          4
#define ATE0_Len        6
#define CPIN_Len        10  
#define CMGD_Len        13
#define CLIP_Len        11
#define CMIC_Len        12
#define CREG_Len        10
#define CGREG_Len       11

#define COPS_Len        10
#define CSQ_Len         8
#define SICS_con_Len    29
//#define SISS_conId_Len  19 //MG323
#define SISS_conId_Len  21 //MG301
//#define SISS_srv_Len    26 //MG323
#define SISS_srv_Len    30 //MG323
#define IOMODE_Len      15
#define SISO_Len        11
#define SISR_Len        16
#define GM301_CMGF_Len  11
#define GM301_CPMS_Len  10

#define power_on    PAout(1) = 1
#define power_off   PAout(1) = 0
#define TERM_ON     PBout(1)

#define SISW_OK 2

#define SISR_1  3 //SISR:0,1 收到GSM主动上报，GSM缓存里有数据要读取。
#define SISR_7E_OK 2 //^SISR:0,24, 收到平台发来的部标数据7E
#define SISR_7E_EMPTY 1 //SISR:0,0 缓存无数据
#define SISR_7E_UN 0 //没有收到SISR的数据。

#define TRUE   1
#define FALSE  0

#define SUB   1 //下发的数据被分包了
#define UNSUB  0 //下发的数据没有分包

#define CEND_NORMAL     0x01 // GSM通话正常结束
#define CEND_UNUSUAL    0x02 // GSM通话没有接通就挂断了
#define CALL_ERROR      0x03 //超过十秒没收到ORIG或CONF

#define HAVE_SM 0x01  //GSM模块收到短信，需要单片机去读
#define REC_SM  0x02  //单片机接收到短信


extern void delay_Nx100ms(u8 n); //100ms定时器，将清狗动作放到while外面。
extern u8 GSM_startup(void); //GSM上电并发送AT判断开机是否成功，成功:返回1;   失败:返回0;
extern u8 GSM_Send_ATCommnd(u8 *p,u8 length,u8 delay_count);  //GSM AT指令发送子函数，发送不成功的话，有五次机会。成功:返回1; 失败:返回0
extern u8 GSM_shutdown(void); //GSM关机程序,成功:返回1;   失败:返回0;
extern u8 GSM_INIT(void); //GSM初始化程序
extern u8 CREG_CGREG(void); //检测GSM模块是否注册上GSM和GPRS网络
extern u8 MG301_SM_Init(void);  //MG301短信初始化
extern void GSM_Module_Start(void); //
extern u8 GPRS_Connect(void); //GPRS拨号
//extern u8 GSM_Send_Data(u8 *p,u8 sendlen,u8 *pid,u8 idlen);//*p:要发送到服务器的数据， *pid:打印到串口2的数据，后面都是对应的数据长度
extern u8 GSM_Send_Data(u8 *p,u8 sendlen);//*p:要发送到服务器的数据
extern u8 GSM_Recivdata_deal(void);//远程数据处理函数
extern void GSM_Ring(void);//检查通话
extern u8 Read_GSM(void); //读取GSM数据函数，当定时器到2.5s或有收到GSM求读取标志
extern u8 GSM_Read_AT_SISR( u8 *p,u8 length,u8 delay_count); //指令成功返回1; 不成功返回0
extern u8 GSM_CALL(u8 * p_number, u8 number_len) ;//拨打电话程序
extern void Check_Net(void); //网络状态检测程序，通话结束后会调用，通过发送心跳测试通路
extern u8 GM301_CPMS_deal(void); //AT+CPMS?  处理函数
//extern void Read_SM(char *p,u8 param_len); //短信读取函数
extern void Read_SM(u8 add); //短信读取函数 
extern void SM_Data_Deal(void);
extern void ASCII2HEX(u8 *p_in,u8 *p_out,u8 len) ;//'AF'-->0xAF;
extern void SM_Deal() ;//短信处理总函数，包括：短信条数读取，每条短信的处理，短信全部删除
extern void Delete_AllSM(void); //短信删除函数，删除缓存中全部短信

extern u8 ms100;//100ms定时器，将清狗动作放到while外面。
extern u8  GSM_Reciev[1100];//GSM接收到来自服务器的数据
extern u16  GSM_Reciev_len ;//GSM接收到来自服务器的数据长度
extern u8  GSM_Ring_Number[16];//来电显示号码
extern u8  GSM_Ring_Number_len; //计算来电显示的号码长度

extern u8 temp_GSM_Reciev[1024] ; //临时存放转义后的数据
extern u16 temp_len ;  //转义后的数据长度

extern u32 Body_state ;
extern u32 alarm_state ; //报警标志，四个字节3
//extern u32 mileage_km; //里程表读数，部分车型可能不支持
//extern u16 oil_mass; //剩余油量，即油箱的剩余油量，用两个字节表示，大于0x8000为%表示，小于0x8000为L表示
extern u8 oil_LperH_average; //L/H,用来算平均值
extern u8 oil_LperH; //L/H,当前采样值,最大值是 0xDF
extern u16 LocationUptime ; //上传位置信息的时间间隔
extern u16 ACCOFF_LocationUptime ;//ACC 关的状态下位置信息上报时间间隔
extern u32 upheartime ; //心跳上传间隔
extern u16 upheart_timeout;//心跳应答超时
extern u8 oil_LperH_F; //上传间隔内每小时油耗的采样次数

extern u16 upheartime_count; //心跳时间计时
extern u16 upheart_timeout_count ;  //心跳应答超时，默认设为5min

extern u16 LocationUptime_count ; //位置信息上传计时器，

extern u16 JinJI_timeout ; //紧急报警时间间隔
extern u16 JinJI_count ; //紧急报警 计时器
extern u16 JinJi_LianXucount ; //连续按下紧急报警时,连续间隔

extern u16 CEND_time ; //提取CEND返回上来的通话时间，如果是0说明通话没有被接通
extern u8 CALL_Timer ; //打电话过程中用到的定时器

extern u8 Check_Net_timeover ; //网络检测，超时复位

extern u8 PID_user_defined[103];  //PID用户自定义
extern u8 PID_user_defined_number ;//PID用户自定义项目数

#endif

