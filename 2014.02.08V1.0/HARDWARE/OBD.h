/******************************************************************************************
*  File Name                             :prot.h
*  Copyright                             :2014.8 wei
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
#ifndef __OBD_H
#define __OBD_H


#include "sys.h" 
#include "usart.h"
#include "GSM.h"
#include "ring.h"

#define LOCK    1
#define UNLOCK  0

#define BD_compile_data_len        6
#define OBD_Versionsi_len          6
#define OBD_Read_VIN_len           6
#define OBD_Read_SN_len            6
#define Read_Sleep_Parameter_len   6
#define Close_Sleep_Parameter_len  8
#define OBD_mileage_len            7
#define Read_oil_mass_len          7
#define Read_oil_LperH_len         7
#define Read_Body_state_len        6



#define OBD_compile_datai       0x33      //0x33OBD模块软件的编译日期
#define Versionsi               0x40      //0x40 OBD模块版本信息
#define Support_PIDi            0x41      //0x41OBD支持的PID数据流
#define OBDII_PIDi              0x42      //0x42读取OBDII PID数据流
#define Custom_PIDi             0x43      //0x43读取自定义PID数据流
#define Read_Body_Statei        0x44      //0x44读车身状态信息
#define Support_Freeze_Framei   0x45      //0x45读取支持的冻结帧
#define Read_Freeze_Framei      0x46  	  //0x46读取冻结帧
#define Read_Fault_Codei        0x47      //0x47读取故障码
#define Clear_Fault_Codei       0x48      //0x48接收到可发送数据的返回
#define Read_VINi               0x49      //0x49读取汽车VIN码
#define Set_Vehicle_Modeli      0x4B      //0x4B标定车型
#define Read_SNi                0x50      //0x50 读取产品序列号(SN),MCU的唯一标识
#define Read_Main_Faulti        0x56      //0x56全车主要系统故障扫描
#define Read_Faulti             0x57      //0x57读取某一个系统的故障码
#define Clear_Faulti            0x58      //0x58清除某一个系统的故障码
#define Body_Controli           0x23      //0x23车辆部件控制，灯，升窗，中控等
#define Set_Sleep_Parameteri    0x24      //0x24设置睡眠参数
#define Read_Sleep_Parameteri   0x25      //0x25读取睡眠参数

#define OBD_compile_data_len  0x06  //读取编译时间指令长度

#define H_TO_L 1
#define L_TO_H 2

#define OBD_BUF_LEN 521


typedef struct
{
     unsigned int OBD_compile_data : 1 ; //0x33OBD模块软件的编译日期
     unsigned int Versions : 1 ; //0x40 OBD模块版本信息
	   unsigned int Support_PID : 1 ;  //0x41OBD支持的PID数据流
     unsigned int OBDII_PID : 1 ; //0x42读取OBDII PID数据流
	   unsigned int Custom_PID : 1; //0x43读取自定义PID数据流
	   unsigned int Read_Body_State  : 1; //0x44读车身状态信息
	   unsigned int Support_Freeze_Frame : 1; //0x45读取支持的冻结帧
     unsigned int Read_Freeze_Frame : 1;  	//0x46读取冻结帧
	   unsigned int Read_Fault_Code : 1; //0x47读取故障码
	   unsigned int Clear_Fault_Code : 1; //0x48接收到可发送数据的返回
	   unsigned int Read_VIN : 1; //0x49读取汽车VIN码
	   unsigned int Set_Vehicle_Model : 1; //0x4B标定车型
	   unsigned int Read_SN : 1;//0x50 读取产品序列号(SN),MCU的唯一标识
	   unsigned int Read_Main_Fault : 1;//0x56全车主要系统故障扫描
	   unsigned int Read_Fault   : 1;//0x57读取某一个系统的故障码
	   unsigned int Clear_Fault   : 1;//0x58清除某一个系统的故障码
	   unsigned int Body_Control : 1;//0x23车辆部件控制，灯，升窗，中控等
	   unsigned int Set_Sleep_Parameter   : 1;//0x24设置睡眠参数
	   unsigned int Read_Sleep_Parameter   : 1;//0x25读取睡眠参数
	   unsigned int PID_UpMode  : 2;  //PID数据流上传方式，0,默认（全部不上传）; 1,推荐常用PID项上传; 2,全开 3,全关
	   unsigned int OBD_Head : 1; //串口中有没有接收到 0x55 0xAA的标志 1,已接收到0x55 0xAA;  0,未接收到0x55 0xAA; 
		 unsigned int OBD_Received_Complete :1 ; //OBD问答数据接收完成
		 unsigned int OBD_Received_Body : 1; //接收到主动上报的车身状态信息
		 unsigned int OBD_Error : 1 ;  //返回的7F错误指令
	   unsigned int alarm :1;  //警戒标志，1:进入警戒;  0:退出警戒
		 unsigned int door : 1; //非法开门标志
		 unsigned int brake : 1;//非法刹车标志
		 unsigned int ACC_alarm :1; //非法ACC标志
		 unsigned int ACC_state :1; //非法ACC标志
		 unsigned int OBD_error_type :2; //OBD返回错误指令，0x00,校验或发送格式错误; 0x01,命令字不支持; 0x02,参数设置不正确或不支持, 0x03,与汽车通信失败
		 unsigned int OBD_Mileage_Support_FLag : 1 ;//支持里程读取的标志，0:支持, 1:不支持 ,默认都是0，即支持
		 unsigned int OBD_Mileage_Receivded_FLag : 1 ;//里程读取完成标志  1:读取完成   0:读取未完成
		 unsigned int OBD_oil_massSupport_FLag : 1 ;//支持剩余读取的标志，0:支持, 1:不支持 ,默认都是0，即支持
		 unsigned int OBD_oil_massReceivded_FLag : 1 ;//剩余油量读取完成标志  1:读取完成   0:读取未完成
		 unsigned int OBD_ACC_Turn : 2; //H_TO_L 由开变成关,  L_TO_H 由关变成开
		 unsigned int OBD_ReadBodyState_Flag : 1 ; // OBD读取车身状态信息，MCU主动去读标志
		 unsigned int OBD_BodyState_Turn_Flag : 1 ; // OBD主动上报车辆状态，这时判断车身状态有变化
		 unsigned int Have_alarm :  1 ; //有警情标志
		 unsigned int FDoor_State_Flag : 1 ; //门状态
		 unsigned int MDoor_State_Flag : 1 ; //门状态
		 unsigned int BDoor_State_Flag : 1 ; //门状态
		 unsigned int ACC_State_Flag : 1 ; //ACC状态
		 unsigned int Brake_State_Flag : 1 ; //刹车状态
		 unsigned int Shake_State_Flag : 1 ; //震动状态
		 //unsigned int Door_State_Flag : 1 ; //有警情出现，
		 unsigned int OBD_Received_PID : 1; //收到的是PID数据流
		 unsigned int OBD_PID_complete : 1; //PID数据流接收完成
} OBD_Flag;  //OBD数据索取标志

extern OBD_Flag  OBD_Com_Flag; 

extern void OBD_init(void); //OBD初始化程序
extern void OBD_Data_Deal(void);
extern void OBD_Error_Judge(void);   //处理OBD返回7F错误信息，判读错误类型
extern void OBD_Error_Judge_zi(void);  //处理OBD返回7F错误信息，判读错误类型00,01,02,03
extern u8 Mark_Car(u8 high, u8 low);  //标定车型函数
extern u8 Body_Control(u8 comm, u8 parameter);  //车身控制函数，
extern u8 OBD_Sleep(u8 type,u8 com,u8 parameter);//睡眠设置函数
extern void Body_state_deal(void);//车身状态提取，报警标志提取。
extern void mileage(void) ;//提取车辆里程，即里程表读数
extern void Read_oil_mass(void); //读取剩余流量，处理两个字节的变量oil_mass
extern u8 Read_oil_LperH(void); //读取L/H瞬时油耗，这个是根据一段之间内累加后推算出来的每小时油耗
extern void oil_average_LperH(void);  //平均油耗计算程序
extern void OBD_Read_Body_state(void); //读取车身状态信息

//读OBDII PID数据流函数
//参数：
//p_PID:PID存放地址
//len_PID:读取PID长度
extern void Read_PID(u8 *p_in,u8 len_in);

extern u16 oil_sampling_time ;  //L/H,采样时间，设为3秒

extern u8 OBD_mileage_buffer_arry[4]; //里程存储数组，读取的里程是四个char类型的数据，直接转义到这里就行了
extern u8 OBD_oil_mass_buffer_arry[2];//剩余油量缓存数组，读取的剩余油量值是2个char类型的数据，直接转移放到这里就行了

extern u8 OBD_PID_Buffer[OBD_BUF_LEN];
extern RING_T ring_obd_buffer; //OBD缓存环
#endif

