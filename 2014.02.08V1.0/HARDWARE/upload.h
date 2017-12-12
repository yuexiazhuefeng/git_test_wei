/******************************************************************************************
*  File Name                             :uplod.h
*  Copyright                             :2014.8 wei
*  Module Name                           :Init port
*
*  CPU                                   :STM32F103RC
*  RTOS                                  :NULL
*
*  Create Date                           :2014/8/24
*  Author                                :wei
*  Abstract Dscription                   :
*
*
*----------------------Revision History-----------------------------
*  No   Version   Date        Revised By          Item           Description
*
*
*******************************************************************************/
#ifndef __UPLOAD_H
#define __UPLOAD_H
#include "stm32f10x.h"
#include "GSM.h"
#include "sys_struct.h"
#include "flash.h"
//#include "sys.h"
//#include "main.h"

//#define TRUE   1
//#define FALSE  0


extern void Terminal_authentication(void); //终端注册函数
extern void change_ben_ji_hao(u8 *in_p);  //把本机号加到待上传的包里
extern void add_liu_shui_hao(u8 *in_p);    //添加流水号子程序
extern u8 add_check_number(u8 *in_p, u16 len); //添加包校验码。参数：u8 *in_p:起始指针; u16 len:处理数据的长度
//打包上传
//参数：
//u8 *out_p处理后的包数据存放地址; u8 *in_p:待打包起始指针; u16 len:待打包长度
//返回：打包后的数据长度，
extern u16 package(u8 *out_p,u8 *in_p, u16 len); //打包上传 参数：u8 *out_p处理后的包数据存放地址; u8 *in_p:待打包起始指针; u16 len:待打包长度 
                                                 //返回：打包后的数据长度，
																								 
																								 //对收到的部标数据解析，
//0x7d 0x02-->0x7e
//0x7d 0x01-->0x7d
//参数，*p_in:解析前的数据位置的起始地址，  *p_out:解析后数据的存放位置的起始地址， len_in:解析前数据的长度
//返回值，
extern u16 Bubiao_Reci_7E_Analysis(u8 *p_in,u8 *p_out,u16 len_in);
extern u8  Send_authentication(void); //发送注册信息
extern void Terminal_JianQuan(void); //终端鉴权函数
extern u8  Send_JianQuan(void); //终端鉴权程序
extern void Terminal_JianQuan_authentication(void);  //对终端进行注册和鉴权
extern u8  Head_deal(Head in_head_mesg,u8 *p_in); //消息头处理函数

extern u8 Upload_Heart(void);//上传心跳程序
extern u8 Send_Heart(void);  //发送心跳函数
extern u8 Upload_Location(void); ////上传位置信息，不包含报警信息

//32位数，转换成4个8位数放到数组里面
//参数：u32 in, 需要转换的32位数,
//      u8 *out_p, 接收转后后的4个字节的数组
//说明, 输入的数是4字节32位的数，输出的是四个字节的数组，将32位数的低位放在数组的高元素中，这样看起来是直接分割的。
extern void u32_to_u8arry(u32 in, u8 *out_p);

//发送位置信息的发送函数
//函数返回值：TRUE:发送成功，这个成功指的是收到GSM模块返回的OK
extern u8 send_upload(u8 type);

//车身状态发生变化，立即上传位置信息
//是对这个变量的操作OBD_Com_Flag.OBD_BodyState_Turn_Flag
extern void BodyChang_UpLoad(void);

//位置信息查询应答 ID
//函数返回值：TRUE:发送成功，这个成功指的是收到GSM模块返回的OK
//输入参数：u8 *p_in:应答流水号的地址
extern u8 response_upload(u8 *p_in);

//终端通用应答函数
//输入参数是个结构体，包含了应答流水号，应答ID，应答结果
//type: 0:成功/确认;  1:失败;  2:消息有误;  3:不支持
extern u8 Terminal_TongYong_response(REsponse TongYongResp);


extern volatile u8 ben_ji_hao[6] ;//数据类型BCD[6]
extern volatile u8 upload_arry[512] ;//上传消息打包。
extern volatile u8 xiaoxi_head[13] ;//消息头处理数组
extern volatile u8 JianQuanMa[20] ;  //存放鉴权码.鉴权码的长度是动态的，这里设其最大长度是20字节，我们一般用6个字节。
extern volatile u8 JianQuanMa_Len ;   //收到的鉴权码的长度
extern volatile u16 liu_shui  ;//流水号
extern volatile u16 message_ID ;
extern volatile u16 message_property;

extern volatile u32 alarm_flag ; //位置信息中的，报警标志
extern volatile u32 state_flag ; //位置信息中的，状态位

extern u16 R_rail ; //围栏半径，非法移位半径
extern u16 Over_Speed ;//超速值
extern u16 Over_Speed_XDTime ; //超速消抖时间，即超速维持时间超过此值时，会有超速报警
extern u8 Province_ID[2] ; //车辆所在省域ID
extern u8 City_ID[2] ; //车辆所在市域ID


extern u8 Vehicle_license_Plate_colour  ;  //车牌颜色，按照JT/T415-2006的5.4.12
extern u8 Vehicle_license_Plate_Len  ; //车牌照长度
extern u8 Vehicle_license_Plate[14] ; //车牌照

extern u8 GNSS_MODE ; //GNSS定位模式 bit0,0:禁用GPS定位, 1:启用GPS定位;   bit1, 0:禁用北斗定位, 1:启用北斗定位
extern u8 GNSS_Baud  ; //GNSS波特率  这里给他初始化为0xFF,因为，0x00有具体的含义; 0x00:4800; 0x01:9600; 0x02:19200; 0x03:38400; 0x04:57600; 0x05:115200
extern u8 GNSS_Hz  ;  //GNSS模块详细定位数据输出频率 GNSS模块详细定位数据输出频率, 0x00:500ms; 0x01:1000ms(默认值); 0x02:2000ms; 0x03:3000ms; 0x04:4000ms

#endif
















