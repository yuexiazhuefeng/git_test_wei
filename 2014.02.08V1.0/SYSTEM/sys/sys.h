#ifndef __SYS_H
#define __SYS_H	
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.7
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	 

//0,��֧��ucos
//1,֧��ucos
#define SYSTEM_SUPPORT_UCOS		0		//����ϵͳ�ļ����Ƿ�֧��UCOS
																	    
	 
//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
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
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

/*******************************************************************************/
//this is the address table map of the SPI-FLASH
//W25Q32 ��ַ��Χ��0X00--0X400000
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

#define Terminal_auth_Flag_addr       0xc4 //�ն�ע��ɹ���־��0x55:�ɹ�;  ����ֵ��ʧ��

#define JianQuanMa_addr     0xc7     //0xc7 - 0xdf��Ȩ��洢��ַ����һ���ֽ��ǳ��ȣ�����23���ֽڴ���Ǽ�Ȩ��

#define BenJiHao_addr       0xE0     //�����Ŵ洢��ַ������Ҫ�洢������Ϣ��ֱ�Ӵ洢������

#define upheartime_addr     0xF0     //�洢��������ʱ��

#define upheart_timeout_addr 0xFA    //�洢����Ӧ��ʱʱ��

#define JinJi_addr 0x0100    //�洢�ϴ�����������ʱ����

#define ACCOFF_GPSuploadtime_add 0x010A  //�洢��ACC�ص�����£�λ����Ϣ�ϱ����

#define R_rail_addr 0x0110 ////Χ���뾶���Ƿ���λ�뾶

#define Over_Speed_addr 0x0115 //����ֵ

#define Over_Speed_XDTime_addr 0x011A //���ٳ���ʱ�䣬��λΪ��(s)

#define Province_ID_addr  0x0120 //��������ʡ��ID

#define City_ID_addr 0x0125  //������������ID 2���ֽ�

#define  Vehicle_license_Plate_colour_addr 0x012A  //������ɫ 1���ֽ�

#define  Vehicle_license_Plate_Len_addr  0x0130 //�����ճ��� 1���ֽ�

#define  Vehicle_license_Plate_addr   0x0131//������*/  14���ֽ�

#define  GNSS_MODE_addr  0x0140 //bit0,0:����GPS��λ, 1:����GPS��λ;   bit1, 0:���ñ�����λ, 1:���ñ�����λ

#define  GNSS_Baud_addr  0x0142 //���������ʼ��Ϊ0xFF,��Ϊ��0x00�о���ĺ���; 0x00:4800; 0x01:9600; 0x02:19200; 0x03:38400; 0x04:57600; 0x05:115200

#define  GNSS_Hz_addr    0x0144  //GNSSģ����ϸ��λ�������Ƶ��, 0x00:500ms; 0x01:1000ms(Ĭ��ֵ); 0x02:2000ms; 0x03:3000ms; 0x04:4000ms

#define CAR_TYPE_ADDR    0x0150  //���ͣ�����ΪFFʱĬ��Ϊ0x04 0x00(������)

#define PID_addr  0x0155  //112���ֽ�

//#define     _addr  0x01c5


/**********************************************************************************/

void NVIC_Configuration(void);



#endif
