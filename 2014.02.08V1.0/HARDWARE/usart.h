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

#ifndef __USART_H
#define __USART_H

#include <string.h>
#include <ctype.h>

#include "sys.h" 
#include "stm32f10x_usart.h" 
#include "timer2.h"
#include "IWDG.h"
#include "port.h"
#include "OBD.h"
#include "sys_struct.h"

#define  latitude_GPS   0x00   //纬度
#define  longitude_GPS  0x04   //经度
#define  elevation_GPS  0x08   //高程
#define  velocity_GPS   0x0A   //速度
#define  direction_GPS  0x0C   //方向
#define  UTC_GPS        0x0E   //时间



   

/********sirf4 GPS初始化指令*******************************/
//const    unsigned char sirf4_cmd00[] = {"$PSRF103,00,00,00,01*24\r\n"};    //$PSRF103,00,00,00,01*24
//const    unsigned char sirf4_cmd01[] = {"$PSRF103,01,00,00,01*25\r\n"};    //$PSRF103,01,00,00,01*25
//const    unsigned char sirf4_cmd02[] = {"$PSRF103,02,00,00,01*26\r\n"};    //$PSRF103,02,00,00,01*26
//const    unsigned char sirf4_cmd03[] = {"$PSRF103,03,00,00,01*27\r\n"};    //$PSRF103,03,00,00,01*27
//const    unsigned char sirf4_cmd04[] = {"$PSRF103,04,00,00,01*20\r\n"};    //$PSRF103,04,00,00,01*20
//const    unsigned char sirf4_cmd05[] = {"$PSRF103,05,00,00,01*21\r\n"};    //$PSRF103,05,00,00,01*21
//const    unsigned char sirf4_cmd06[] = {"$PSRF103,06,00,00,01*22\r\n"};    //$PSRF103,06,00,00,01*22
/********sirf4 GPS初始化指令*******************************/
//const  u8 AT[4]={"AT\r\n"};

void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void Usart1_SendData(u8 * psend, u8 length);
void Usart2_SendData(u8 * psend, u8 length);
void Usart3_SendData(u8 * psend, u8 length);
void GPS_Flag_Clear (void); //GPS标志清空
void sirf4Config(void);//sirf4 GPS初始化函数

extern void wait(u32 count); //最小1ms

u8 change_gpsdata(void);
u32 shiliutoshi(u32 ret,u8 *p);

extern volatile u8 sendlengthU1;
extern u8 * psendU1;
extern volatile u8 sendlengthU2;
extern u8 * psendU2;
extern volatile u8 sendlengthU3;
extern u8 * psendU3;

extern u8 usart1_Rx_buffer[166] ; //GSM数据暂定只有140个，
extern u8 usart1_Rx_length ;    //当前接收的数据长度，单位是字节
extern u8 OBD_Package_len ;     //OBD数据包里面的包长度信息
extern u8 usart1_Rx_backup[50] ; //这个数组用来存放串口3缓存中的数据，以备后面处理用。
extern u8 USART1_Rx_timer ;      //定时30ms，用来清空串口1接收缓存，防止干扰，和一些不必要的返回数据


extern u8 usart2_Rx_buffer[90];//GPS接收缓存
extern u8 usart2_Rx_number;

extern u8 usart3_Rx_buffer[1100] ; //GSM数据暂定只有140个，
extern u16 usart3_Rx_length ;
extern u8 usart3_Rx_backup[1100]; //这个数组用来存放串口3缓存中的数据，以备后面处理用。
extern u16 usart3_Rx_backup_length;
extern u8 usart3_Rx_SM[150] ;  //短信缓存
extern u8 usart3_Rx_SM_len ;     //接收短信长度
extern u8 USART3_Rx_timer ; //定时30ms，用来清空串口3接收缓存，防止干扰，和一些不必要的返回数据

extern u8 GPS_DATA[20] ; //用来存放正在解析的数据
extern u8 GPS_DATA_BACKUP[20] ;//用来备份解析好的数据 
extern u8 GSM_Reciev[1100];//GSM接收到来自服务器的数据
extern u16 GSM_Reciev_len ;//GSM接收到来自服务器的数据长度
extern u8  GSM_Ring_Number[16];//来电显示号码
extern u8  GSM_Ring_Number_len; //计算来电显示的号码长度

/*
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
} GLOBALstruct_Flag;

extern GLOBALstruct_Flag  Flag_all;  

*/
#endif

