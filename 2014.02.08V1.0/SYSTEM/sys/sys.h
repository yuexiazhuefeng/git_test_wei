#ifndef __SYS_H
#define __SYS_H	
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.7
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	 

//0,不支持ucos
//1,支持ucos
#define SYSTEM_SUPPORT_UCOS		0		//定义系统文件夹是否支持UCOS
																	    
	 
//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

/*******************************************************************************/
//this is the address table map of the SPI-FLASH
//W25Q32 地址范围：0X00--0X400000
//#define wr_psw              0x00         //1 byte write state            0x00        //wu bytenumber

//#define cmd_word            0x02         //2 byte command                0x02 - 0x03 //wu bytenumber

//#define ck_addr             0x04         //12 byte upload cycle          0x04 - 0x0f  2 byte for bytenumber

//#define sd_addr             0x10         //16 byte speed value           0x10 - 0x1f  2 byte for bytenumber

//#define relay_addr          0x20         //1 byte relay state            0x20         wu bytenumber

#define user_addr           0x30         //16 byte user name for GPRS    0x30 - 0x3f  2 byte for bytenumber

#define password_addr       0x40         //16 byte password for GPRS     0x2b - 0x34  2 byte for bytenumber

#define PortNumber_addr     0x50         //16 byte port number for GPRS  0x35 - 0x3a  2 byte for bytenumber

#define IPaddress_addr      0x60         //16 byte IP address for GPRS   0x40 - 0x4f  2 byte for bytenumber

#define apn_addr            0x70         //16 byte apn name for GPRS     0x50 - 0x5f  2 byte for bytenumber

#define PhoneNumber1_addr   0x80         //16 byte telephone number1     0x60 - 0x6f  2 byte for bytenumber

#define PhoneNumber2_addr   0x90         //16 byte telephone number2     0x70 - 0x7f  2 byte for bytenumber

#define MID_addr            0xa0         //6 byte MID                    0x80 - 0x85  wu bytenumber       

//#define TEST_psw          0xb0         //1 byte test eeprom            0x86         wu bytenumber  

#define GPSuploadtime       0xc0

#define Terminal_auth_Flag_addr       0xc4 //终端注册成功标志，0x55:成功;  其他值：失败

#define JianQuanMa_addr     0xc7     //0xc7 - 0xdf鉴权码存储地址，第一个字节是长度，后面23个字节存的是鉴权码

#define BenJiHao_addr       0xE0     //本机号存储地址，不需要存储长度信息，直接存储本机号

#define upheartime_addr     0xF0     //存储心跳发送时间

#define upheart_timeout_addr 0xFA    //存储心跳应答超时时间

#define JinJi_addr 0x0100    //存储上传紧急报警的时间间隔

#define ACCOFF_GPSuploadtime_add 0x010A  //存储在ACC关的情况下，位置信息上报间隔

#define R_rail_addr 0x0110 ////围栏半径，非法移位半径

#define Over_Speed_addr 0x0115 //超速值

#define Over_Speed_XDTime_addr 0x011A //超速持续时间，单位为秒(s)

#define Province_ID_addr  0x0120 //车辆所在省域ID

#define City_ID_addr 0x0125  //车辆所在市域ID 2个字节

#define  Vehicle_license_Plate_colour_addr 0x012A  //车牌颜色 1个字节

#define  Vehicle_license_Plate_Len_addr  0x0130 //车牌照长度 1个字节

#define  Vehicle_license_Plate_addr   0x0131//车牌照*/  14个字节

#define  GNSS_MODE_addr  0x0140 //bit0,0:禁用GPS定位, 1:启用GPS定位;   bit1, 0:禁用北斗定位, 1:启用北斗定位

#define  GNSS_Baud_addr  0x0142 //这里给他初始化为0xFF,因为，0x00有具体的含义; 0x00:4800; 0x01:9600; 0x02:19200; 0x03:38400; 0x04:57600; 0x05:115200

#define  GNSS_Hz_addr    0x0144  //GNSS模块详细定位数据输出频率, 0x00:500ms; 0x01:1000ms(默认值); 0x02:2000ms; 0x03:3000ms; 0x04:4000ms

#define CAR_TYPE_ADDR    0x0150  //车型，该项为FF时默认为0x04 0x00(凯美瑞)

#define PID_addr  0x0155  //112个字节

//#define     _addr  0x01c5


/**********************************************************************************/

void NVIC_Configuration(void);



#endif
