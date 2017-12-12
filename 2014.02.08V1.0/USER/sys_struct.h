/******************************************************************************************
*  File Name                             :prot.h
*  Copyright                             :2014.8 wei
*  Module Name                           :Init port
*
*  CPU                                   :STM32F103RC
*  RTOS                                  :NULL
*
*  Create Date                           :2014/8/22
*  Author                                :wei
*  Abstract Dscription                   :
*
*
*----------------------Revision History-----------------------------
*  No   Version   Date        Revised By          Item           Description
*
*
*******************************************************************************/
#ifndef _SYS_STRUCT_H
#define _SYS_STRUCT_H

#include "stm32f10x.h"
#include "ring.h"
typedef struct 
{
     unsigned int GPSHead : 1 ; //"$GPRMC"  标志
     unsigned int GPSreceiv_complete : 1 ; //GPS数据接受完成标志
	   unsigned int GPSreceiv_eorr : 1 ;  //GPS数据接收错误
     unsigned int GPSdeal_complete : 1 ; //GPS数据处理完成
	   unsigned int debugmodel : 1; //程序进入参数设置模式
	   unsigned int debughead  : 1; //调试指令头标志
	   unsigned int PC_Command : 1; //接收到PC发送的参数设置指令
     unsigned int OK : 1;  	//接收到OK标志
	   unsigned int OK_O : 1; //接收到O,
	   unsigned int SISW : 1; //接收到可发送数据的返回
	   unsigned int SISR : 1; //接收数据
	   unsigned int RING : 1; //有电话进来
	   unsigned int RING_S : 1;//接收到RING
	   unsigned int jieting : 1;//接听标志 
	   unsigned int guaji   : 1;//挂机标志
	   unsigned int COPS   : 1;//在检测网络运营商
	   unsigned int haveGSMdata : 2 ;//GSM缓存有数据，求读取。
	   unsigned int GSMData_Dealing :1 ;//0:没有数据处理，  1:正在处理GSM数据，不要向GSM_Reciev[]数组中写数据。
	   unsigned int Subpackage : 1;  //消息分包标志，SUB=1：分包,  UNSUB =0：无分包
	   unsigned int GSM_Reciev_Flag : 1; //GSM_Reciev[]这个数组能否接受串口收到的数据，标志。
	   unsigned int Terminal_auth_Flag :1; //判断终端是否注册成功。TRUE:成功; FALSE:失败
		 unsigned int Terminal_JianQuan_Flag :1; //判断终端是否鉴权成功成功。TRUE:成功; FALSE:失败
		 unsigned int SendHeart_Flag : 1 ; //发送心跳标志，1:心跳发出;  0:收到心跳应答
		 unsigned int JinJi_Flag : 1; //紧急报警标志  1:有紧急报警,且平台没有人工解除;  0:没有紧急报警
		 unsigned int JinJi_Switch_Flag : 1; //紧急报警开关状态 1:紧急报警按下  0:紧急报警开关没按
		 unsigned int SendLocation_Flag : 1 ; // 位置信息上传标志, 1:位置信息发出，还没收到平台应答; 0:收到平台应答
		 unsigned int oil_LperH_average_Flag : 1 ; // 平均油耗采样完成，等待读取。
     unsigned int JinJi_Exti_Rising_flag : 1; //紧急报警脚有上升电平，即PC3脚有上升电平
		 unsigned int Shake_Flag : 1;   //有震动标志
		 unsigned int Upload_flag : 1;  //上传位置信息间隔时间到，需要上传位置信息
		 unsigned int CEND_Flag :1 ;    //接收到^CEND:1,6,67,16 表示通话结束
		 unsigned int NO_CARRIER : 1;   //接收到NO CARRIER，表示通话结束
		 unsigned int CONN_Flag : 1;    //接收到CONN,表示通话接通
		 unsigned int CALL_Flag : 1;    //拨号标志，表示终端正在进行拨号操作
		 unsigned int ORIG_Flag : 1;    //表示GSM模块已经在做拨号的动作了
		 unsigned int NO_CARRIER_Flag : 1;    //通话，拨号，结束
		 unsigned int CONF_Flag : 1;    //通话，拨号，结束
		 unsigned int Check_Net_Flag : 1; //检测网络通路是否正常，用于通话结束后的网络状态检测
		 unsigned int NO_ANSWER :1; //超时无人接听，
		 unsigned int Check_Net_Send :1; //在Check_Net()子函数中设置
		 unsigned int GM301_CPMS :1 ;//在发送AT+CPMS?\r\n
		 unsigned int SM_FLAG : 1;//有短信需要接受
		 unsigned int GM301_CMGR : 1; //读取短信
		 unsigned int GM301_CMGR_UNread : 1; //短信未读,0:已读,不需要处理; 1:未读，需要处理。
		 unsigned int UART1_RING :2;//GSM的RING脚
} GLOBALstruct_Flag;

extern GLOBALstruct_Flag  Flag_all;  

typedef struct 
{
//  u8  ben_ji_hao[6] ;//本机号,以字节为单位，放在上面，内存对齐的时候可以节省一定的空间
	u16 liu_shui ; //流水号
  u16 message_ID ; //消息ID
  u16 message_property ; //消息体属性	
	//后期应该会加消息封装项的  总包数  和 包序号
} Head;

extern Head Head_mesg;

typedef struct 
{
	u8 pliu_shui[2] ; //流水号
  u8 pmessage_ID[2] ; //消息ID
  u8 type ; //消息体属性	
	//后期应该会加消息封装项的  总包数  和 包序号
} REsponse;

extern REsponse TongYongResp;





#endif



