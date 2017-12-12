/******************************************************************************************
*  File Name                             :prot.h
*  Copyright                             :2014.2 wei
*  Module Name                           :Init port
*
*  CPU                                   :STM32F103RC
*  RTOS                                  :NULL
*
*  Create Date                           :2014/7/3
*  Author                                :wei
*  Abstract Dscription                   :
*
*
*----------------------Revision History-----------------------------
*  No   Version   Date        Revised By          Item           Description
*
*
*******************************************************************************/
#include "GSM.h"
const u8 AT[4]={"AT\r\n"};
const u8 ATE0[6]={"ATE0\r\n"};
const u8 CPIN[10]={"AT+CPIN?\r\n"};
const u8 CMGD[13]={"AT+CMGD=1,4\r\n"};
const u8 CLIP[11]={"AT+CLIP=1\r\n"};
const u8 CMIC[12]={"AT+CMIC=7\r\n"};  //MG323=10; MG301=7
const u8 CREG[10]={"AT+CREG?\r\n"};
const u8 CGREG[11]={"AT+CGREG?\r\n"};

const u8 COPS[10]={"AT+COPS?\r\n"};
const u8 CSQ[8]={"AT+CSQ\r\n"};
const u8 SICS_con[29]={"AT^SICS=0,\"conType\",\"GPRS0\"\r\n"};
//const u8 SISS_conId[19]={"AT^SISS=0,conId,0\r\n"};  //MG323
const u8 SISS_conId[21]={"AT^SISS=0,\"conId\",0\r\n"};  //MG301
//const u8 SISS_srv[26]={"AT^SISS=0,srvType,Socket\r\n"}; //MG323
const u8 SISS_srv[30]={"AT^SISS=0,\"srvType\",\"Socket\"\r\n"}; //MG301
const u8 IOMODE[15]={"AT^IOMODE=1,1\r\n"};  //1,对接收数据进行转换，当对端以hex形势发送数据时; 1，不使用接收缓存
const u8 SISO[11]={"AT^SISO=0\r\n"};
const u8 SISR[16]={"AT^SISR=0,1500\r\n"};
const u8 GM301_CMGF[11]={"AT+CMGF=1\r\n"};
const u8 GM301_CPMS[11]={"AT+CPMS?\r\n"};
const u8 POD_OBD_PID_Tabl[13][8]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
																	0x08,0x09,0x0A,0x0b,0x0c,0x0d,0x0e,0x0f,
																	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
																	0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
																	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
																	0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
																	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
																	0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
																	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
																	0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
																	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,
																	0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
																	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0xff                             
																	}; //PID
u8 PID_user_defined[103]={0x00};  //PID用户自定义
u8 PID_user_defined_number =0 ;//PID用户自定义项目数
u8 PID_default_arry[9] ={0x05,0x0c,0x0d,0x0f,0x1f,0x33,0x46,0x5a,0x5c};

u8 	GSM_command[140] ={0};//GSM命令打包数组
u8 	GSM_Reciev[1100] ={0};//GSM接收到来自服务器的数据
u16  GSM_Reciev_len = 0;//GSM接收到来自服务器的数据的长度
u8  GSM_Ring_Number[16]={0};//来电显示号码
u8  GSM_Ring_Number_len =0; //计算来电显示的号码长度
u32 Body_state =0; //车身状态位，四个字节
u32 alarm_state =0; //报警标志，四个字节
//u32 mileage_km=0; //里程表读数，部分车型可能不支持
//u16 oil_mass=0; //剩余油量，即油箱的剩余油量，用两个字节表示，大于0x8000为%表示，小于0x8000为L表示
u8 oil_LperH_average=0; //L/H,用来算平均值,最大值是 0xDF.
u8 oil_LperH=0; //L/H,当前采样值,最大值是 0xDF
u16 LocationUptime =0; //上传位置信息的时间间隔
u16 ACCOFF_LocationUptime = 0;//ACC 关的状态下位置信息上报时间间隔
u32 upheartime =0; //心跳上传间隔
u16 upheart_timeout =0;//心跳应答超时
u8 oil_LperH_F=0; //上传间隔内每小时油耗的采样次数

u16 upheartime_count=0; //心跳间隔计时器
u16 upheart_timeout_count =0;  //心跳应答超时计时器，默认设为5min

u16 LocationUptime_count =0; //位置信息上传计时器，
 
u16 JinJI_timeout =0; //紧急报警时间间隔
u16 JinJI_count =0; //紧急报警 计时器
u16 JinJi_LianXucount = 0; //连续按下紧急报警时,连续间隔

u8 ms100; //100ms定时器，将清狗动作放到while外面。
void delay_Nx100ms(u8 n) //100ms定时器，将清狗动作放到while外面。
{
 u8 i;
	IWDG_Feed();
	for(i=0;i<n;i++)
	{    
		ms100 = 0;
		while(ms100 <10);
		IWDG_Feed();
  }
}

u8 GSM_startup(void) //GSM开机程序,包括AT指令验证开机是否成功  成功:返回1;   失败:返回0;
{
  Usart2_SendData("Power on...\r\n",13);
	power_on;
	delay_Nx100ms(5);//GSM上电后延时0.5s
	TERM_ON = 1;//
  delay_Nx100ms(15);//GSM上电后将TERM_ON脚拉低1.5s,开机。
	TERM_ON = 0;  
	delay_Nx100ms(30); //GSM开机后，延时3s，给模块初始化时间
	if(GSM_Send_ATCommnd((u8 *)AT,4,3))
	{
    Usart2_SendData("Power on success...\r\n",21);		
		return 1;
  }		
	else
	{
    Usart2_SendData("Power on Failed...\r\n",20);
		return 0;
  }
		
}
u8 GSM_shutdown(void) //GSM关机程序,成功:返回1;   失败:返回0;
{
  Usart2_SendData("Power off...\r\n",14);
	TERM_ON = 1;	
  delay_Nx100ms(15);//GSM将TERM_ON脚拉低1.5s,关机
	TERM_ON = 0;  
	delay_Nx100ms(2);
	if(GSM_Send_ATCommnd((u8 *)AT,4,2)) //判断AT指令有没有返回OK
	{
		Usart2_SendData("Power off Failed...\r\n",21);		
		return 0;
	}
	else
	{
    Usart2_SendData("Power off success...\r\n",22);
		power_off;//关机断电
		delay_Nx100ms(75);//断电后延时7.5S MG323下电最短时间为6S
		return 1;	
  }	
}

u8 GSM_Send_ATCommnd( u8 *p,u8 length,u8 delay_count) //指令成功返回1; 不成功返回0
{ //GSM AT指令发送子函数，发送不成功的话，有五次机会
	//*p:待发送命令的地址; length:发送指令的长度; delay_count:发送指令后到判断OK的等待时间,单位100ms
  u8 *pAT_commnd = p;
	u8 send_length = length;
	u8 delay = delay_count;
	u8 i = 0;	
	Flag_all.OK = 0;
	for(i=0;i<5;i++)
	{
     if(Flag_all.OK)  //标志位判断，以后可以再加一个形成编号对应不同标志位。
		 {
       Flag_all.OK = 0;				
			 return 1;
     }
		 if(Flag_all.SISW) //可以向GPRS缓存发送数据
		 {
       Flag_all.SISW = 0;	
       //if((usart3_Rx_backup[11]==GSM_command[10])&&(usart3_Rx_backup[12]==GSM_command[11])&&(usart3_Rx_backup[13]==GSM_command[12]))			 
			  return SISW_OK;
     }
/*		 
		 if(Flag_all.SISR) //收到GPRS通道的数据，通常是服务器下发的部标协议的消息包。
		 {
       Flag_all.SISR = 0;	
       //if((usart3_Rx_backup[11]==GSM_command[10])&&(usart3_Rx_backup[12]==GSM_command[11])&&(usart3_Rx_backup[13]==GSM_command[12]))			 
			  return SISR_7E_OK;
     }*/
		 memset(usart3_Rx_backup,0x00,1100);//发送AT指令之前，先把串口3的接收缓存备份清空。
		 memset(usart3_Rx_buffer,0x00,1100);//发送AT指令之前，先把串口3的接收缓存清空。
		 usart3_Rx_length =0 ;
		 Usart3_SendData(pAT_commnd,send_length);
		 delay_Nx100ms(delay);
  }
	Flag_all.OK = 0;
	memset(usart3_Rx_buffer,0x00,1100);
	usart3_Rx_length =0 ;
	return 0;	
}

u8 GSM_INIT(void)
{
	//u8 i =0;
	
  Usart2_SendData("GSM init...\r\n",13);
	if(!GSM_Send_ATCommnd((u8 *)ATE0,ATE0_Len,2)) //ATE0
	{
    Usart2_SendData("Restart GSM...\r\n",16);
		while(1)
		{			
			if(GSM_startup())
			{				
				if(GSM_Send_ATCommnd((u8 *)ATE0,ATE0_Len,2))
				{
					Usart2_SendData("Restart GSM success...\r\n",24);
					 memset(usart3_Rx_buffer,0x00,1100);
			     usart3_Rx_length =0 ;
					break;
        }					
			}
			GSM_shutdown();
			delay_Nx100ms(70);//断电后延迟7S，MG323断电最短时间需要6S。      		
    }		
  }
//0D 0A 2B 43 50 49 4E 3A 20 52 45 41 44 59 0D 0A 0D 0A 4F 4B 0D 0A 	
//\r \n +  C  P  I  N  :     R  E  A  D  Y   \r \n \r \n O  K  \r \n
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21
	Usart2_SendData("Check SIM Card...\r\n",19);
  if(GSM_Send_ATCommnd((u8 *)CPIN,CPIN_Len,3)) //AT+CPIN?
	{
    if((usart3_Rx_backup[9]=='R')&&(usart3_Rx_backup[10]=='E')&&(usart3_Rx_backup[11]=='A')\
			&&(usart3_Rx_backup[12]=='D')&&(usart3_Rx_backup[13]=='Y'))
		{
      Usart2_SendData("SIM card on throne...\r\n",23);
			memset(usart3_Rx_backup,0x00,140);
    }
    else
		{
      Usart2_SendData("Check SIM Card...\r\n",19);
			return 0;
    }
  }
	else
	{
    Usart2_SendData("SIM Card Eorr!!!\r\n",18);
		return 0;
  }
	
	if(!GSM_Send_ATCommnd((u8 *)CLIP,CLIP_Len,3))//AT+CLIP =1 设置来电显示
	{
    if(!GSM_Send_ATCommnd((u8 *)CLIP,CLIP_Len,3))
			return 0;
  }
	if(!GSM_Send_ATCommnd((u8 *)CMIC,CMIC_Len,3))//AT+CMIC =10 设置MIC增益
	{
    if(!GSM_Send_ATCommnd((u8 *)CMIC,CMIC_Len,3))
			return 0;
  }
  if(!CREG_CGREG())
		return 0;	
	Usart2_SendData("GSM Init Completed\r\n",20);
	return 1;
}

u8 CREG_CGREG(void)
{
  u8 i =0; 
	//delay_Nx100ms(150);//延时15s等待GSM模块注册网络
//0D 0A 2B 43 52 45 47 3A 20 30 2C 31 0D 0A 0D 0A 4F 4B 0D 0A	
//\r \n +  C  R  E  G  :     0  ,  1	\r \n \r \n O  K  \r \n
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19	
//	if(GSM_Send_ATCommnd((u8 *)CREG,CREG_Len,2))//AT+CREG? 检查GSM网络是否注册成功
//	{
	Usart2_SendData("Checking GSM registration status...\r\n",38);
	for(i=0;i<10;i++)
	{
		if(GSM_Send_ATCommnd((u8 *)CREG,CREG_Len,2))//AT+CGREG? 检查GPRS网络是否注册成功
		{
			if((usart3_Rx_backup[2]=='+')&&(usart3_Rx_backup[3]=='C')&&(usart3_Rx_backup[4]=='R')\
					&&(usart3_Rx_backup[5]=='E')&&(usart3_Rx_backup[6]=='G')&&(usart3_Rx_backup[7]==':'))
			{
				if(usart3_Rx_backup[11] == '1')
				{
					 Usart2_SendData("registered local network...\r\n",29);
						break;
				}
				else if(usart3_Rx_backup[11] == '5')
				{
					 Usart2_SendData("registered roaming...\r\n",23);
						break;
				}
				else if(usart3_Rx_backup[11] == '0')
				{
					//Usart2_SendData("not registered no operator...\r\n",31);
					delay_Nx100ms(10);//等待1S 
					if(i == 99)
					{
						Usart2_SendData("GSM registered Faild, Restart GSM...\r\n",40);
						return 0;
					}					
				}
				else if(usart3_Rx_backup[11] == '2')
				{
					//Usart2_SendData("not registered,searching the operator...\r\n",42);
					delay_Nx100ms(10);//等待1S 
					if(i == 99)
					{
						Usart2_SendData("GSM registered Faild, Restart GSM...\r\n",40);
						return 0;
					}					
				}
				else if(usart3_Rx_backup[11] == '3')
				{
					//Usart2_SendData("registration is refused!!!\r\n",28);
					delay_Nx100ms(10);//等待1S 
					if(i == 99)
					{
						Usart2_SendData("GSM registered Faild, Restart GSM...\r\n",40);
						return 0;
					}					
				}
				else if(usart3_Rx_backup[11] == '4')
				{
					//Usart2_SendData("unknown state!!!\r\n",18);
					delay_Nx100ms(10);//等待1S 
					if(i == 99)
					{
						Usart2_SendData("GSM registered Faild, Restart GSM...\r\n",40);
						return 0;
					}					
				}
			}
		}		
		else
		{
			delay_Nx100ms(10);//等待1S 
			if(i == 9)
			{
				Usart2_SendData("GSM registered Faild, Restart GSM...\r\n",40);
				return 0;
			}					
		}
	}
 
// }
//0D 0A 2B 43 47 52 45 47 3A 20 30 2C 31 0D 0A 0D 0A 4F 4B 0D 0A
//\r \n +  C  G  R  E  G   :    0  ,  1  \r \n \r \n O  K  \r \n
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20	
//	if(GSM_Send_ATCommnd((u8 *)CGREG,CGREG_Len,2))//AT+CGREG? 检查GPRS网络是否注册成功
//	{
/******AT+CGREG?****************************************/	
	Usart2_SendData("Checking GPRS registration status...\r\n",39);
	for(i=0;i<15;i++)
	{
		if(GSM_Send_ATCommnd((u8 *)CGREG,CGREG_Len,2))//AT+CGREG? 检查GPRS网络是否注册成功
		{
			if((usart3_Rx_backup[2]=='+')&&(usart3_Rx_backup[3]=='C')&&(usart3_Rx_backup[4]=='G')&&(usart3_Rx_backup[5]=='R')\
				 &&(usart3_Rx_backup[6]=='E')&&(usart3_Rx_backup[7]=='G')&&(usart3_Rx_backup[8]==':'))
			{
				if(usart3_Rx_backup[12] == '1')
				{
					 Usart2_SendData("GPRS registered local network...\r\n",34);
						break;
				}
				else if(usart3_Rx_backup[12] == '5')
				{
					 Usart2_SendData("GPRS registered roaming...\r\n",28);
						break;
				}
				else if(usart3_Rx_backup[12] == '0')
				{
					//Usart2_SendData("GPRS not registered no operator...\r\n",36);
					delay_Nx100ms(10);//等待1S 
					if(i == 14)
					{
						Usart2_SendData("GPRS registered Faild, Restart GSM...\r\n",40);
						return 0;
					}		
				}
				else if(usart3_Rx_backup[12] == '2')
				{
					//Usart2_SendData("GPRS not registered,searching the operator...\r\n",47);
					delay_Nx100ms(10);//等待1S 
					if(i == 14)
					{
						//Usart2_SendData("GPRS registered Faild, Restart GSM...\r\n",40);
						return 0;
					}		
				}
				else if(usart3_Rx_backup[12] == '3')
				{
					//Usart2_SendData("GPRS registration is refused!!!\r\n",33);
					delay_Nx100ms(10);//等待1S 
					if(i == 14)
					{
						Usart2_SendData("GPRS registered Faild, Restart GSM...\r\n",40);
						return 0;
					}		
				}
				else if(usart3_Rx_backup[12] == '4')
				{
					//Usart2_SendData("GPRS unknown state!!!\r\n",23);
					delay_Nx100ms(10);//等待1S 
					if(i == 14)
					{
						Usart2_SendData("GPRS registered Faild, Restart GSM...\r\n",40);
						return 0;
					}
				}
			}
		}					
		else
		{
			delay_Nx100ms(10);//等待1S 
			if(i == 14)
			{
				Usart2_SendData("GPRS registered Faild, Restart GSM...\r\n",40);
				return 0;
			}					
		}
	}
	return 1;	
/************AT+CGREG?*******************************************/		
//  }
}

u8 sm_used=0; //GSM接收到的短信数目
u8 sm_total =0;//gsm存储器可接收的总短信数目
u8 MG301_SM_Init(void)
{
	u8 i = 0;
	
	Usart2_SendData((u8 *)"Init short message...\r\n",sizeof("Init short message...\r\n"));
//	for(i= 0;i<100;i++)
	//{
	if(!GSM_Send_ATCommnd((u8 *)GM301_CMGF,GM301_CMGF_Len,3))//GM301_CMGF 设置短消息模式为TEXT
		if(!GSM_Send_ATCommnd((u8 *)GM301_CMGF,GM301_CMGF_Len,3))//AT+COPS? 检查网络运营商
			return 0;	
	//if(GSM_Send_ATCommnd((u8 *)GM301_CPMS,GM301_CPMS_Len,3))//GM301_CMGF 设置短消息模式为TEXT
	//  sm_used = GM301_CPMS_deal(); //读取短信缓存状态，返回收到的短信数目
	  SM_Deal(); //短信处理
	//	Read_SM(14);  //
  //}
#if 0 
  if(!GSM_Send_ATCommnd((u8 *)CMGD,CMGD_Len,3))//AT+CMGD=1,4 删除首选缓存的全部短信
	{
    if(!GSM_Send_ATCommnd((u8 *)CMGD,CMGD_Len,3))
			return 0;
  }
#endif
	return 1;	
}
//AT+CPMS?
//+CPMS: "SM",3,50,"SM",3,50,"SM",3,50

//OK
//参数说明：
//p:
u8 GM301_CPMS_deal(void)  //AT+CPMS?  查询短信存储缓存状态，是否有短信
{
  u8 *pStart;
	u8 *pEnd;
	u8 char_len = 0;	
	u8 i = 0;
	u8 temp_sm_used = 0;
	Flag_all.GM301_CPMS = 1;
	IWDG_Feed();
	if(GSM_Send_ATCommnd((u8 *)GM301_CPMS,GM301_CPMS_Len,3))//GM301_CMGF 设置短消息模式为TEXT
	{
    pStart = (u8 *)memchr((const char*)usart3_Rx_SM,',',usart3_Rx_SM_len);
		pEnd   = (u8 *)memchr((const char*)(pStart+1),',',usart3_Rx_SM_len);
		char_len = pEnd-pStart-1;
		temp_sm_used = 0;
		for(i=0;i<char_len;i++) //计算短信条数
		{
      temp_sm_used = temp_sm_used*10+((*(pStart+1+i))-0x30);
    }
  }
	memset(usart3_Rx_SM,0x00,usart3_Rx_SM_len);
	usart3_Rx_SM_len =0;
	return temp_sm_used; //返回短信条数
}

//短信读取函数，AT+CMGR=sm_num
//char *p,读取短信的地址，直接用字符串表示，省去转换
//参数：sm_num:短信存储位置字符串型
//void Read_SM(char *p,u8 param_len)
void Read_SM(u8 add)
{
	u8 i_len;
	u8 test_i;
	strcpy(GSM_command,"AT+CMGR=");
	if((0<add)&&(add<=9))
	{
    GSM_command[sizeof("AT+CMGR=")-1] =add+0x30;
		//strcpy(GSM_command+sizeof("AT+CMGR=")-1,p);
		//strcpy(GSM_command+sizeof("AT+CMGR=")-1+param_len,"\r\n");
		strcpy(GSM_command+sizeof("AT+CMGR="),"\r\n");
		Flag_all.GM301_CMGR = 1;
		if(GSM_Send_ATCommnd((u8 *)GSM_command,sizeof("AT+CMGR=")+2,5))//GM301_CMGF 设置短消息模式为TEXT
		{
			return ;
		}
  }
	else if((add>9)&&(add<100))
	{
    GSM_command[sizeof("AT+CMGR=")-1] =(add/10)+0x30;
		GSM_command[sizeof("AT+CMGR=")] =add-((add/10)*10)+0x30;
		//strcpy(GSM_command+sizeof("AT+CMGR=")-1,p);
		//strcpy(GSM_command+sizeof("AT+CMGR=")-1+param_len,"\r\n");
		strcpy(GSM_command+sizeof("AT+CMGR=")+1,"\r\n");
		Flag_all.GM301_CMGR = 1;
		if(GSM_Send_ATCommnd((u8 *)GSM_command,sizeof("AT+CMGR=")+1+2,5))//GM301_CMGF 设置短消息模式为TEXT
		{
			return ;
		}
  }
}
//短信处理总函数，包括：短信条数读取，每条短信的处理，短信全部删除
void SM_Deal()
{
	u8 i = 0;
  sm_used = GM301_CPMS_deal(); //读取有多少条短信需要处理
  for(i=1;i<=sm_used;i++)
	{
    Read_SM(i);
		if(Flag_all.GM301_CMGR_UNread)
		{
			Flag_all.GM301_CMGR_UNread = 0;
      SM_Data_Deal();
    }
  }
  Delete_AllSM();	
}

//短信内容处理函数，处理具体的某条短信内容的函数，譬如: "SOBDPIDA*0#"
void SM_Data_Deal(void)
{
	u8 i=0;
	u8 j=0;
	u8 *p;
	u8 temp_pid[26] = {0x00};
  //OBD PID数据流设置
  if(NULL==strstr((const char*)(usart3_Rx_SM), "#")) return ; //防止没有结尾符'#'

	if(NULL!=strstr((const char*)(usart3_Rx_SM), "SOBDPID*")) //自定义设置
	{
   //SOBDPID*C0*C0*00*00*00*00*00*00*00*00*00*00*00#
    p=strstr((const char*)(usart3_Rx_SM), "SOBDPID*");
		for(i=0;p[8+i]!='#';i++)  //去除*
		{
      if(p[8+i]!='*')
			 temp_pid[j++] = p[8+i];
    }
		ASCII2HEX(temp_pid,temp_pid,26);
		PID_user_defined_number =0 ;
		memset(PID_user_defined,0x00,103);
   	for(i=0;i<13;i++)
		{
      for(j=0;j<8;j++)
			{
        if((temp_pid[i]<<j)&0x80)
					PID_user_defined[PID_user_defined_number++]=POD_OBD_PID_Tabl[i][j];  //PID用户自定义
      }
    }
		SPI_Flash_SafeWrite((u8 *)(&PID_user_defined_number),PID_addr,0x01);
		SPI_Flash_SafeWrite(PID_user_defined,PID_addr+1,PID_user_defined_number);
    memset(usart3_Rx_SM,0x00,usart3_Rx_SM_len);
	  usart3_Rx_SM_len =0;
		//SPI_Flash_SafeRead((u8 *)(&(PID_user_defined_number)),PID_addr,0x01);
		//SPI_Flash_SafeRead(PID_user_defined,PID_addr+1,PID_user_defined_number);
  }
	if(NULL!=strstr((const char*)(usart3_Rx_SM), "SOBDPIDA*0#")) //全关，默认
	{
	//	SOBDPIDA*0#
    memset(PID_user_defined,0x00,103);
	  PID_user_defined_number =0;
		SPI_Flash_SafeWrite((u8 *)(&PID_user_defined_number),PID_addr,0x01);
		SPI_Flash_SafeWrite(PID_user_defined,PID_addr+1,103); //讲PID存储位置全部清零
		memset(usart3_Rx_SM,0x00,usart3_Rx_SM_len);
	  usart3_Rx_SM_len =0;
  }
	if(NULL!=strstr((const char*)(usart3_Rx_SM), "SOBDPIDA*1#")) //全开
	{
		//SOBDPIDA*1#   
		PID_user_defined_number = 103;
    memcpy(PID_user_defined,POD_OBD_PID_Tabl,103);		
		SPI_Flash_SafeWrite((u8 *)(&PID_user_defined_number),PID_addr,0x01);
		SPI_Flash_SafeWrite(PID_user_defined,PID_addr+1,PID_user_defined_number);
    memset(usart3_Rx_SM,0x00,usart3_Rx_SM_len);
	  usart3_Rx_SM_len =0;
  }
	if(NULL!=strstr((const char*)(usart3_Rx_SM), "SOBDPIDA*2#")) //推荐常用设置项
	{
		//SOBDPIDA*2#
		PID_user_defined_number = 9;
		memcpy(PID_user_defined,PID_default_arry,PID_user_defined_number);
		SPI_Flash_SafeWrite((u8 *)(&PID_user_defined_number),PID_addr,0x01);
		SPI_Flash_SafeWrite(PID_user_defined,PID_addr+1,PID_user_defined_number);
    memset(usart3_Rx_SM,0x00,usart3_Rx_SM_len);
	  usart3_Rx_SM_len =0;
  }
}

//'AF'-->0xAF;
void ASCII2HEX(u8 *p_in,u8 *p_out,u8 len) //
{
  u8 i,j;
	u8 temp_len=0;
	u8 *pin=p_in;
	u8 *pout =p_out;
	for(i=0;i<len;i++)
	{
    if(('0'<=pin[i])&&(pin[i] <='9'))
		{
      pout[i]=pin[i]-'0';
    }
    else if(('A'<=pin[i])&&(pin[i] <='F'))
		{
      pout[i]=pin[i]-'A'+10;
    }
  }
  for(temp_len=0,j=0;j<i;j+=2)
	{
    pout[temp_len++] =pout[j]<<4|pout[j+1];
  }
}

//短信删除函数，删除缓存中全部短信
void Delete_AllSM(void)
{
  if(!GSM_Send_ATCommnd((u8 *)CMGD,CMGD_Len,10))//AT+CMGD=1,4 删除首选缓存的全部短信,短信比较多时需要花费时间较长，这里用1s的等待时间
	{
    if(!GSM_Send_ATCommnd((u8 *)CMGD,CMGD_Len,10))
			return ;
  }
}

void GSM_Module_Start(void)
{
	u8 i = 0;
/*****上电********************************/		
	while(1)
	{
    while(1)  //GSM模块开机程序
		{
			if(GSM_startup())
			{
				break;
			}
			else
			{
				i++;
				if(i==2)
				{
					i = 0;
					if(GSM_shutdown());				
					else
						GSM_shutdown();
				}
			}			
		}
/*****GSM_INIT********************************/		
		
		if((GSM_INIT())&&(MG301_SM_Init())&&(GPRS_Connect()))
		{
      GPIO_SetBits(GPIOC,GPIO_Pin_8);  //GSM灯亮
			break;
    }
			
		else
		{
      Usart2_SendData("GSM Init Faild, Restart1...\r\n",29);
			delay_Nx100ms(20);
			if((GSM_INIT())&&(MG301_SM_Init())&&(GPRS_Connect()))
			{
        GPIO_SetBits(GPIOC,GPIO_Pin_8);  //GSM灯亮
				break;
      }		
			else
			{
        Usart2_SendData("GSM Init Faild, Restart2...\r\n",29);
			  //delay_Nx100ms(20);	
        if(GSM_shutdown());				
				else
				GSM_shutdown();			
      }
    }
		
/*************************************/		
  }
  
}

u8 GPRS_Connect(void)
{
	u8 i= 0;
	u8 bytenumbertemp[2] = {0};
  u8 bytetemp= 0;
  u8 bytetemp_i=0;
	//AT+COPS?
//0D 0A 2B 43 4F 50 53 3A 20 30 2C 30 2C 22 43 48 49 4E 41 20 4D 4F 42 49 4C 45 22 0D 0A 0D 0A 4F 4B 0D 0A	
//\r \n +  C  O  P  S  :     0  ,  0  ,  "  C  H  I  N  A     M  O  B  I  L  E  "  \r \n \r \n O  K  \r \n
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34
//\r \n +  C  O  P  S  :     0  ,  0  ,  "  C  H  N  -  C  U  G  S  M  "  \r \n \r \n O  K	\r \n	
//0D 0A 2B 43 4F 50 53 3A 20 30 2C 30 2C 22 43 48 4E 2D 43 55 47 53 4D 22 0D 0A 0D 0A 4F 4B 0D 0A 
	Usart2_SendData("Checking Operator Information...\r\n",35);
	for(i= 0;i<100;i++)
	{
    Flag_all.COPS =1;
		if(GSM_Send_ATCommnd((u8 *)COPS,COPS_Len,2))//AT+COPS? 检查网络运营商
	  { 
     // if((usart3_Rx_backup[20]=='M')&&(usart3_Rx_backup[21]=='O')&&(usart3_Rx_backup[22]=='B')\
			//	&&(usart3_Rx_backup[23]=='I')&&(usart3_Rx_backup[24]=='L')&&(usart3_Rx_backup[25]=='E'))
			//MG301:  +COPS: 0,0,"China Unicom"
			//+COPS: 0,0,"China Mobile"
      //
      //OK
			if(NULL!=strstr((const char*)(usart3_Rx_backup), "China Mobile"))
			{
        Usart2_SendData("China Mobile...\r\n",17);
				Flag_all.COPS =0;
				break;
      }
			//else if((usart3_Rx_backup[17]=='-')&&(usart3_Rx_backup[18]=='C')&&(usart3_Rx_backup[19]=='U')\
			//	&&(usart3_Rx_backup[20]=='G')&&(usart3_Rx_backup[21]=='S')&&(usart3_Rx_backup[22]=='M'))
			else if(NULL!=strstr((const char*)(usart3_Rx_backup), "China Unicom"))
			{
        Usart2_SendData("China Unicom...\r\n",17);
				Flag_all.COPS =0;
				break;
      }
			else
			{
				//Usart2_SendData("GPRS not registered no operator...\r\n",36);
				delay_Nx100ms(10);//等待1S 
				if(i == 99)
				{
					Usart2_SendData("Check Operator Information Faild, Restart GSM...\r\n",50);
					Flag_all.COPS =0;
					return 0;
				}
			}
    }
		else
		{
      delay_Nx100ms(10);//等待1S 
			if(i == 99)
			{
				Usart2_SendData("Check Operator Information Faild, Restart GSM...\r\n",50);
				Flag_all.COPS =0;
				return 0;
			}
    }		
  }  
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20
//\r \n +  C  S  Q  :     1  4  ,  9  9  \r \n \r \n O  K  \r \n 
//0D 0A 2B 43 53 51 3A 20 31 34 2C 39 39 0D 0A 0D 0A 4F 4B 0D 0A 	
	Usart2_SendData("checking the signal...\r\n",24);
	for(i= 0;i<20;i++)
	{
    if(GSM_Send_ATCommnd((u8 *)CSQ,CSQ_Len,2))//AT+CSQ? 检测信号强度
	  { 
      Usart2_SendData("The Signal is:",14);
			Usart2_SendData(usart3_Rx_backup+8,5);
			Usart2_SendData("\r\n",2);
      break;			
    }
		else
		{
      delay_Nx100ms(10);//等待1S 
			if(i >= 19)
			{
				Usart2_SendData("The Signal Error, Restart GSM...\r\n",50);
				return 0;
			}
    }		
  }
/*	
///////////////////////AT^IOMODE=0,1  IOMODE_Len
	Usart2_SendData("Setting IOMODE...\r\n",19);
	for(i= 0;i<2;i++)
	{    
		if(GSM_Send_ATCommnd((u8 *)IOMODE,IOMODE_Len,2))//AT+CSQ? 检测信号强度
	  { 
      Usart2_SendData("OK\r\n",4);		
      break;			
    }
		else
		{
      delay_Nx100ms(10);//等待1S 
			if(i >1)
			{
				Usart2_SendData("Set IOMODE Erorr, Reset GSM...\r\n",32);
				Flag_all.COPS =0;
				return 0;
			}
    }		
  }
*/
///////////////////////////AT^SICS?	
	Usart2_SendData("Setting conType...\r\n",20);
	for(i= 0;i<20;i++)
	{
    if(GSM_Send_ATCommnd((u8 *)SICS_con,SICS_con_Len,2))//AT^SICS? 检查网络运营商
	  { 
      Usart2_SendData("conType : GPRS0\r\n",18);			
      break;			
    }
		else
		{
      delay_Nx100ms(10);//等待1S 
			if(i >= 19)
			{
				Usart2_SendData("Set conType Error, Restart GSM...\r\n",35);
				return 0;
			}
    }		
  }
	Usart2_SendData("Setting APN...\r\n",16); //APN
	for(i= 0;i<20;i++)
	{
    if((i==0)||(i==5)||(i==10)||(i==15)||(i==19))
		{
      SPI_Flash_SafeRead(bytenumbertemp,apn_addr,2);	
			bytenumbertemp[0] = (bytenumbertemp[0] - 0x30);
			bytenumbertemp[1] = (bytenumbertemp[1] - 0x30);
			bytetemp = (bytenumbertemp[0] * 10 + bytenumbertemp[1]);
			if((bytetemp == 0) || (bytetemp >= 15))
			{
				Usart2_SendData("APN Erorr...\r\n",14);
         return 0 ;				
			}
			//SPI_Flash_SafeRead(bytenumbertemp+2,apn_addr+2,bytetemp);
			//Usart2_SendData("The APN is: ",12);
			//Usart2_SendData(bytenumbertemp+2,bytetemp);
			//Usart2_SendData("\r\n",2);
      memset(GSM_command,0x00,140);		
#if 0  //MG323			
			strcpy(GSM_command,"AT^SICS=0,apn,");			
			SPI_Flash_SafeRead(GSM_command+14,apn_addr+2,bytetemp);			
    	strcpy(GSM_command+14+bytetemp,"\r\n");
#endif
			//MG301			
			strcpy(GSM_command,"AT^SICS=0,\"apn\",\""); //MG301
			SPI_Flash_SafeRead(GSM_command+17,apn_addr+2,bytetemp);			
    	strcpy(GSM_command+17+bytetemp,"\"\r\n");
    }
		if(GSM_Send_ATCommnd((u8 *)GSM_command,bytetemp+20,2))//
	  { 
      Usart2_SendData("The APN is: ",12);
			Usart2_SendData(bytenumbertemp+2,bytetemp);
			Usart2_SendData("\r\n",2);			
      break;			
    }
		else
		{
      delay_Nx100ms(10);//等待1S 
			if(i >= 19)
			{
				Usart2_SendData("APN Erorr, Reset GSM...\r\n",25);
				return 0;
			}
    }
  }
#if 0 //MG323
	/////////////////////SISS_conId_Len
	Usart2_SendData("Setting 0 conId is 0...\r\n",25); //APN
	for(i= 0;i<20;i++)
	{   
		if(GSM_Send_ATCommnd((u8 *)SISS_conId,SISS_conId_Len,2))//AT^SISS=0,conId,0检查网络运营商
	  { 
      Usart2_SendData("Setting 0 conId success\r\n",25);			
      break;			
    }
		else
		{
      delay_Nx100ms(10);//等待1S 
			if(i >= 19)
			{
				Usart2_SendData("Set conId erorr, Reset GSM...\r\n",31);
				return 0;
			}
    }		
  }
/////////////////////SISS_srv_Len
	Usart2_SendData("Setting srvType is socket...\r\n",30); //APN
	for(i= 0;i<20;i++)
	{   
		if(GSM_Send_ATCommnd((u8 *)SISS_srv,SISS_srv_Len,2))//AT^SISS=0,conId,0检查网络运营商
	  { 
      Usart2_SendData("Set srvType is socket success\r\n",31);			
      break;			
    }
		else
		{
      delay_Nx100ms(10);//等待1S 
			if(i >= 19)
			{
				Usart2_SendData("Set srvType is socket erorr, Reset GSM...\r\n",31);
				return 0;
			}
    }		
  }
#endif  //MG323
//MG301	
	/////////////////////SISS_srv
	Usart2_SendData("Setting srvType is socket...\r\n",30); //AT^SISS=0,"srvType","Socket"
	for(i= 0;i<20;i++)
	{   
		if(GSM_Send_ATCommnd((u8 *)SISS_srv,SISS_srv_Len,2))//AT^SISS=0,conId,0检查网络运营商
	  { 
      Usart2_SendData("Set srvType is socket success\r\n",31);			
      break;			
    }
		else
		{
      delay_Nx100ms(10);//等待1S 
			if(i >= 19)
			{
				Usart2_SendData("Set srvType is socket erorr, Reset GSM...\r\n",31);
				return 0;
			}
    }		
  }
	/////////////////////SISS_conId
	Usart2_SendData("Setting 0 conId is 0...\r\n",25); //
	for(i= 0;i<20;i++)
	{   
		if(GSM_Send_ATCommnd((u8 *)SISS_conId,SISS_conId_Len,2))//AT^SISS=0,\"conId\",0\r\n
	  { 
      Usart2_SendData("Setting 0 conId success\r\n",25);			
      break;			
    }
		else
		{
      delay_Nx100ms(10);//等待1S 
			if(i >= 19)
			{
				Usart2_SendData("Set conId erorr, Reset GSM...\r\n",31);
				return 0;
			}
    }		
  }
	//////////AT^SISS=0,address,"sockudp://"
	Usart2_SendData("Setting server IP and port...\r\n",31); //IP APN
	for(i= 0;i<20;i++)
	{
    if((i==0)||(i==5)||(i==10)||(i==15)||(i==19))
		{
////IP			
      SPI_Flash_SafeRead(bytenumbertemp,IPaddress_addr,1);
			//if((bytenumbertemp[0] > '0') && (bytenumbertemp[0] <= '9'))
			//	bytetemp = (bytenumbertemp[0] - 0x30);
			//else if((bytenumbertemp[0] >= 'A') && (bytenumbertemp[0] <= 'F'))
			//bytetemp = (bytenumbertemp[0] - 0x37);
			bytetemp = bytenumbertemp[0];			
			if(bytetemp >0x0F)//else 
			{
				Usart2_SendData("EEpropm error,reset SR\r\n",24);		
        return 0;				
			}
			memset(GSM_command,0x00,140);
			strcpy(GSM_command,"AT^SISS=0,address,\"sockudp://");
		  SPI_Flash_SafeRead(GSM_command+29,IPaddress_addr+1,bytetemp);
			GSM_command[29+bytetemp]=':';
			Usart2_SendData("The IP is:",10);
			Usart2_SendData(GSM_command+29,bytetemp);//把IP发出来
			Usart2_SendData("\r\n",2);
			bytetemp_i =bytetemp;
			bytetemp = 0;
			memset(bytenumbertemp,0x00,2);
////PORT			
			SPI_Flash_SafeRead(bytenumbertemp,PortNumber_addr,2);	 
			//bytenumbertemp[0] = (bytenumbertemp[0] - 0x30);
		 // bytenumbertemp[1] = (bytenumbertemp[1] - 0x30);
			bytetemp = bytenumbertemp[0];//(bytenumbertemp[0] * 10 + bytenumbertemp[1]);
			if((bytetemp == 0) || (bytetemp >= 15))
			{				
				Usart2_SendData("EEpropm error,reset PT\r\n",24);				
			}
			SPI_Flash_SafeRead(GSM_command+30+bytetemp_i,PortNumber_addr+2,bytetemp);
			strcpy(GSM_command+bytetemp_i+bytetemp+30,"\"\r\n");
			Usart2_SendData("The PORT is:",12);
			Usart2_SendData(GSM_command+30+bytetemp_i,bytetemp);//把port发出来
		  Usart2_SendData("\r\n",2);
    }
		if(GSM_Send_ATCommnd((u8 *)GSM_command,bytetemp_i+bytetemp+32,2))//AT+COPS? 检查网络运营商
	  { 
      Usart2_SendData("Set server IP and port success...\r\n",35); //APN
      break;			
    }
		else
		{
      delay_Nx100ms(10);//等待1S 
			if(i >= 19)
			{
				Usart2_SendData("APN Erorr, Reset GSM...\r\n",25);
				return 0;
			}
    }
  }
//////////打开链路
	Usart2_SendData("Open the 0 srvProfileld...\r\n",28); //APN
	for(i= 0;i<6;i++)
	{   
		if(GSM_Send_ATCommnd((u8 *)SISO,SISO_Len,30))//AT^SISS=0,conId,0检查网络运营商
	  { 
      Usart2_SendData("Open the 0 srvProfileld success\r\n",33);
			Usart2_SendData("GSM molel power on DONE\r\n",25);
      break;			
    }
		else
		{
      delay_Nx100ms(10);//等待1S 
			if(i >= 5)
			{
				Usart2_SendData("Open the 0 srvProfileld erorr, Reset GSM...\r\n",45);
				
				return 0;
			}
    }		
  }	
 return 1;
}

//u8 GSM_Send_Data(u8 *p,u8 sendlen,u8 *pid,u8 idlen)//*p:要发送到服务器的数据， *pid:打印到串口2的数据，后面都是对应的数据长度
u8 GSM_Send_Data(u8 *p,u8 sendlen)//*p:要发送到服务器的数据
{
	u8 sendlen_s =sendlen;
//	u8 idlen_s =idlen;
	u8 *Sendp = p;
//	u8 *pSendU2 = pid;
	
  memset(GSM_command,0x00,140);
	strcpy(GSM_command,"AT^SISW=0,");
	GSM_command[10]=sendlen/100+0x30;
	GSM_command[11]=(sendlen/10-(sendlen/100)*10)+0x30;
	GSM_command[12]=(sendlen-(sendlen/10)*10)+0x30;
	strcpy(GSM_command+13,"\r\n");
	GSM_command[15]= 0;
	if(SISW_OK==GSM_Send_ATCommnd((u8 *)GSM_command,15,3))
	{    
		if(GSM_Send_ATCommnd((u8 *)Sendp,sendlen,3))//发送数据
		  return 1;		
  }
  return 0;	
}

u8 temp_GSM_Reciev[1024] ={0}; //临时存放转义后的数据
u16 temp_len =0;  //转义后的数据长度
u8 GSM_Recivdata_deal(void)
{//直接处理GSM_Reciev[]数组中的数据，该数组中的数据是进过提取后得到的纯指令数据。
     
	//zhuanyi	
	
  
  u8 check_number =0; //计算接收到的消息验证码
  u16 temp_mesg_ID =0; //提取消息ID
  u16 temp_mese_Len =0; //消息体的长度，从消息体属性中提取。
  u16 temp_respmesg_ID =0;
  u8 temp_liushui_arry[2]={0};//临时存放平台过来的消息流水号
  u16 response_liushui = 0u;
  u8 i=0;
  u8 call_back =0;

  temp_len = Bubiao_Reci_7E_Analysis(GSM_Reciev,temp_GSM_Reciev,GSM_Reciev_len);//调用反转义函数，返回转义后数据的长度  
  memset(GSM_Reciev,0x00,GSM_Reciev_len); //上面处理完转义后，即可清空GSM_Reciev[]缓存
  GSM_Reciev_len = 0;
  Flag_all.haveGSMdata = SISR_7E_UN; //清除收到7E数据标志，GSM_Reciev[]数组可以开始新一轮接收

  if(temp_len > 1024)  return 0;//防止要处理的参数，超出范围，只是为了增强程序的健壮性
	
  check_number = add_check_number(temp_GSM_Reciev,temp_len);		//计算校验码
  if(check_number ==temp_GSM_Reciev[temp_len-2]) //比较校验码，如果校验码检验正确则进入
	{
//7E 81 00 00 09 01 83 56 95 78 28 0D 2A 00 02 00 42 42 49 54 51 4F BF 7E
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 
    temp_mesg_ID = (temp_GSM_Reciev[1]<<8)|temp_GSM_Reciev[2]; //提取ID，ID位于temp_GSM_Reciev的第1和第2个元素
		//temp_mese_Len =(temp_GSM_Reciev[3]&0x03)?((temp_GSM_Reciev[3]&0x03)*256+temp_GSM_Reciev[4]):temp_GSM_Reciev[4];  //提取消息体的长度，在消息体属性中。
		temp_mese_Len =(temp_GSM_Reciev[3]&0x03)*256+temp_GSM_Reciev[4];  //提取消息体的长度，在消息体属性中。
		temp_liushui_arry[0] = temp_GSM_Reciev[11];
		temp_liushui_arry[1] = temp_GSM_Reciev[12]; //提取平台过来的消息流水号
		if(temp_GSM_Reciev[3]&(0x01<<5)) //判断消息有没有分包
			Flag_all.Subpackage = SUB;
		else
			Flag_all.Subpackage = UNSUB;
		for(i=0;i<6;i++)//比较本机号
		{
			if(ben_ji_hao[i]!= temp_GSM_Reciev[5+i])
				return 0 ; //本机号不对，直接返回。
    }
/**以上是对消息头的提取，包括：消息头，消息体长度，消息是否分包，本机号是否正确,************/
		if(Flag_all.Subpackage == UNSUB) //如果消息没有分包，进入此分支
		{
      if(temp_mesg_ID == 0x8100)//终端注册应答
			{
				//提取鉴权码
				JianQuanMa_Len = temp_mese_Len-3;//计算鉴权码的长度
				memcpy(JianQuanMa,(temp_GSM_Reciev+16),JianQuanMa_Len);
				return (temp_GSM_Reciev[15]+5);   //为了和TRUE=1,FALSE=0冲突，这里给他加上5，结果处理函数中会再减去五进行还原。返回注册结果，0：成功；1：车辆已被注册；2，数据库中无该车辆；
                                      //3，终端已被注册；4，数据库中无该终端				
      }
			if(temp_mesg_ID == 0x8001) //平台通用应答
			{
        //PPP包:[7E 80 01 00 05 01 32 05 14 48 12 10 3D 00 02 01 02 00 D0 7E ]-11:47:51
				//       0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19
				temp_respmesg_ID = (temp_GSM_Reciev[15]<<8)|temp_GSM_Reciev[16]; //提取应答ID，ID位于temp_GSM_Reciev的第15和第16个字节
				response_liushui = (temp_GSM_Reciev[13]<<8)|temp_GSM_Reciev[14]; //提取8001通用应答的应答流水号
				if(temp_respmesg_ID==0x0102)//接收到终端鉴权的平台通用应答
				{
          if(temp_GSM_Reciev[17]==0x00)//鉴权成功 平台通用应答返回结果：0,成功/失败； 1:失败； 2,消息有误； 3:不支持； 4:报警处理确认
					{
            Flag_all.Terminal_JianQuan_Flag = TRUE;
						return TRUE;
          }
					else//终端鉴权失败
					{
            Flag_all.Terminal_JianQuan_Flag = FALSE;
						return FALSE;
          }
        }
				/*******鉴权 END*************************/
					/*******心跳应答 START*************************/
				if(temp_respmesg_ID==0x0002)//接收到心跳的平台通用应答
				{
          if(temp_GSM_Reciev[17]==0x00)//平台通用应答返回结果：0,成功/失败； 1:失败； 2,消息有误； 3:不支持； 4:报警处理确认
					{
            upheart_timeout_count = 0;  //心跳超时定时器清零。
						Flag_all.SendHeart_Flag = 0;  //心跳上传标志清零，表示收到应答
						return TRUE;
          }
					else//终端鉴权失败
					{
            Flag_all.Terminal_JianQuan_Flag = FALSE;
						return FALSE;
          }
        }
			/*******心跳应答 END*************************/
			/********位置信息应答 START**********************/
				if(temp_respmesg_ID==0x0200)//接收到位置信息的平台通用应答
				{
          if(temp_GSM_Reciev[17]==0x00)//位置信息上传成功 平台通用应答返回结果：0,成功/失败； 1:失败； 2,消息有误； 3:不支持； 4:报警处理确认
					{
            if(Flag_all.JinJi_Flag)  //如果,有紧急报警标志，判断是不是紧急报警的应答
						{
              if(response_liushui == JinJi_liushui)//通过对比流水号，判断是不是紧急报警消息的应答
							{
                Flag_all.JinJi_Flag = 0; //将紧急报警标志清零，
								alarm_state =alarm_state&(~((u32)0x01)); //将紧急报警的报警位清零
								JinJi_Num =0;//将紧急报警次数清零，下次报警时又从0开始计时
								return TRUE;
              }
            }
						if(Flag_all.Shake_Flag)
						{
              if(response_liushui == Shake_liushui)//通过对比流水号，判断是不是紧急报警消息的应答
							{
                Flag_all.Shake_Flag = 0; //将紧急报警标志清零，
								alarm_state =alarm_state&(~(((u32)0x01)<<16)); //将紧急报警的报警位清零
								Shake_sendNum =0;//震动报警次数清零，下次报警时又从0开始计时
								return TRUE;
              }
            }
						//Flag_all.SendHeart_Flag =1;  //心跳上传标志清零，表示收到应答
						return TRUE;
          }
					else//终端鉴权失败
					{
            Flag_all.Terminal_JianQuan_Flag = FALSE;
						return FALSE;
          }
        }
		  /********位置信息应答 END*************************/
				
      }
//请求位置信息:7E 82 01 00 00 01 78 10 00 00 00 00 02 E8 7E		
      if(temp_mesg_ID == 0x8201)	//请求位置信息
      {  
        response_upload(temp_liushui_arry); //调用位置信息查询应答函数
      }	
//标定车型7E 0F 0A 00 03 01 78 10 00 00 00 00 02 55 04 00 E8 7E	
//        0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
      if(temp_mesg_ID == 0x0F0A)	//标定车型
      {  
        if(1==Mark_Car(temp_GSM_Reciev[14],temp_GSM_Reciev[15])) //调用车型标定函数，
				{
          memcpy(TongYongResp.pliu_shui,&temp_GSM_Reciev[11],2) ;//提取流水号
					memcpy(TongYongResp.pmessage_ID,&temp_GSM_Reciev[1],2) ;//提取流水号
					TongYongResp.type = 0x00; //添加结果
          SPI_Flash_SafeWrite(temp_GSM_Reciev+14,CAR_TYPE_ADDR,0x02);	
        }
				else
				{
          memcpy(TongYongResp.pliu_shui,&temp_GSM_Reciev[11],2) ;//提取流水号
					memcpy(TongYongResp.pmessage_ID,&temp_GSM_Reciev[1],2) ;//提取流水号
					TongYongResp.type = 0x01; //添加结果
        }
      }
			
//接收到设置终端参数命令0x8103	
//PPP包:[7E 81 03 00 3C 01 78 20 00 00 09 00 1A 07 00 00 00 57 04 00 00 00 00 00 00 00 58 04 00 00 00 00 00 00 00 59 04 00 00 00 00 00 00 00 5A 04 00 00 00 00 00 00 00 5B 02 00 00 00 00 00 5C 02 00 00 00 00 00 80 04 00 00 00 00 7C 7E ]-15:06:14
//       0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17			
			if(temp_mesg_ID == 0x8103) //设置终端参数的消息ID是: 0x8103
			{ //使用通用应答
        Parameter_Set(temp_GSM_Reciev+13, 0);
				
				memcpy(TongYongResp.pliu_shui,&temp_GSM_Reciev[11],2) ;//提取流水号
				memcpy(TongYongResp.pmessage_ID,&temp_GSM_Reciev[1],2) ;//提取流水号
				TongYongResp.type = 0x00; //添加结果
				Terminal_TongYong_response(TongYongResp) ; //使用通用应答，进行应答
				
				SystemReset();
      }
 //监听，平台下发号码，终端来拨打
 //PPP?:[7E 84 00 00 0C 01 78 10 00 00 00 00 07 01 31 38 33 35 36 39 35 37 38 32 38 D7 7E ]-15:10:21
 //      0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17	
			if(temp_mesg_ID == 0x8400)
			{
				if(temp_GSM_Reciev[13] ==0x01) //监听类型    0:普通通话， 1:监听
				{ 
          Flag_all.SendHeart_Flag = 0; //开始拨打电话之前，把有心跳上传标志清零，这是防止有心跳发送时还没来的及收到心跳应答，退出通话时可能造成不必要的复位					
          for(i= 0;i<6;i++) //在这个循环里面进行拨号的动作
					{ 
            call_back = GSM_CALL(temp_GSM_Reciev+14, temp_mese_Len-1);   //调用拨打电话程序						
						if(call_back == CEND_NORMAL)  //呼叫接通后，正常挂断，
						{ 
							Flag_all.Check_Net_Flag = 1;  //发送信号，让主程序去检测终端跟服务器的连接状态
							Check_Net();
							break;			
						}
						else
						{
							delay_Nx100ms(10);//等待1S 
							if(i >= 5)
							{
								Usart2_SendData("Open the 0 srvProfileld erorr, Reset GSM...\r\n",45);								
								return 0;
							}
						}		
					}
        }
        
      }
			
    }
  }		
	//	Flag_all.SISR = 0;
   // memset(GSM_Reciev,0x00,GSM_Reciev_len);
		//GSM_Reciev_len = 0;
		//else if
  	return FALSE;
}

void GSM_Ring(void)
{
  u8 Phone_number[16]={0};
  u8 Phone_number_Len = 0;
  u8 i = 0;
	if(Flag_all.RING)
	{
		Flag_all.RING = 0;
		if(!Flag_all.jieting) //jie ting le ma phone number is the same!
		{
		//memcpy(GSM_Ring_Number,(pStart+1),GSM_Ring_Number_len);
			SPI_Flash_SafeRead(Phone_number,PhoneNumber1_addr,2);//提取存储号码1的长度
		  Phone_number[0] = Phone_number[0]-0x30;
			Phone_number[1] = Phone_number[1]-0x30;
			Phone_number_Len =Phone_number[0]*10+Phone_number[1];
			if(Phone_number_Len ==GSM_Ring_Number_len)
			{
        SPI_Flash_SafeRead(Phone_number+2,PhoneNumber1_addr+2,Phone_number_Len);
				for(i=0;i<Phone_number_Len;i++)
				{
          if(Phone_number[2+i]!=GSM_Ring_Number[i])
						break ;
					if(i==Phone_number_Len-1)
					{
            memset(GSM_Ring_Number,0x00,16);
						GSM_Ring_Number_len = 0;
						Usart3_SendData("ATA\r\0",5);//发送接听命令
						Flag_all.jieting = 1;
						Flag_all.guaji = 0;	
						return;
          }
        }
      }					
			memset(Phone_number,0x00,16);
			Phone_number_Len = 0;
			SPI_Flash_SafeRead(Phone_number,PhoneNumber2_addr,2);//提取存储号码1的长度
			Phone_number[0] = Phone_number[0]-0x30;
			Phone_number[1] = Phone_number[1]-0x30;
			Phone_number_Len =Phone_number[0]*10+Phone_number[1];
			if(Phone_number_Len ==GSM_Ring_Number_len)
			{
				SPI_Flash_SafeRead(Phone_number+2,PhoneNumber2_addr+2,Phone_number_Len);
				for(i=0;i<Phone_number_Len;i++)
				{
					if(Phone_number[2+i]!=GSM_Ring_Number[i])
					{
						memset(GSM_Ring_Number,0x00,16);
						GSM_Ring_Number_len = 0;
						Usart3_SendData("ATH\r\n",5);
						Flag_all.guaji = 1;							
						break ;
					}
						
					if(i==Phone_number_Len-1)
					{
						memset(GSM_Ring_Number,0x00,16);
						GSM_Ring_Number_len = 0;
						Usart3_SendData("ATA\r\0",5);//发送接听命令
						Flag_all.jieting = 1;
						Flag_all.guaji = 0;	
					}
				}
			}
			else
			{
				memset(GSM_Ring_Number,0x00,16);
				GSM_Ring_Number_len = 0;
				Usart3_SendData("ATH\r\0",5);
				Flag_all.guaji = 1;					
			}		
	 }
 }
}

// AT^SISR=0,1500
//返回参数：1：接收到7E数据，需要处理
//          0：没有接收到7E数据
u8 Read_GSM(void) //读取GSM数据函数，当定时器到2.5s或有收到GSM求读取标志
{
	u8 i =0;
	u8 temp_flag =0;
  for(i= 0;i<2;i++) //这里如果有错误的话，就只要读两遍，不然延时太长导致程序的效率变低。
	{  
		temp_flag = GSM_Read_AT_SISR((u8 *)SISR,SISR_Len,5);//读GSM接收缓存，并返回接收到的数据标志
		if(SISR_7E_OK == temp_flag)//收到有7E数据需要处理
	  { 
      //Usart2_SendData("Setting 0 conId success\r\n",25);	
      //GSM_Recivdata_deal();			
      return 1;			
      //break;			
    }
		else if(SISR_7E_EMPTY == temp_flag)  //缓存里面没有数据，不需要处理
			return 0;			
		else
		{
      delay_Nx100ms(3);//等待300ms
			if(i >= 1)
			{
				Usart2_SendData("read error...\r\n",15);
				return 0;
			}
    }		
  }
	return 0;
}

u8 GSM_Read_AT_SISR( u8 *p,u8 length,u8 delay_count) //指令成功返回1; 不成功返回0
{ //GSM AT指令发送子函数，发送不成功的话，有五次机会
	//*p:待发送命令的地址; length:发送指令的长度; delay_count:发送指令后到判断OK的等待时间,单位100ms
  u8 *pAT_commnd = p;
	u8 send_length = length;
	u8 delay = delay_count;
	u8 i = 0;	
	Flag_all.OK = 0;
	for(i=0;i<5;i++)
	{
		 if(Flag_all.haveGSMdata) //收到GPRS通道的数据，通常是服务器下发的部标协议的消息包。
		 {			 
       if(SISR_7E_OK==Flag_all.haveGSMdata)
				  return SISR_7E_OK; //2
			 if(SISR_7E_EMPTY==Flag_all.haveGSMdata)
			 {
         Flag_all.haveGSMdata = SISR_7E_UN ; //0
				 return SISR_7E_EMPTY;  //1
       }
				  
			 Flag_all.haveGSMdata = SISR_7E_UN;		 //0
     }
		 
//		 memset(usart3_Rx_backup,0x00,140);//发送AT指令之前，先把串口3的接收缓存备份清空。
		 memset(usart3_Rx_buffer,0x00,1100);//发送AT指令之前，先把串口3的接收缓存清空。
		 usart3_Rx_length =0 ;
		 Flag_all.haveGSMdata = SISR_7E_UN;
		 Usart3_SendData(pAT_commnd,send_length);
		// while(!Flag_all.haveGSMdata)
		 delay_Nx100ms(delay);
  }
  return SISR_7E_UN;
}

//电话回拨程序
/*
ATD18356957828;

^ORIG: 1,0

^CONF: 1

+COLP: "",129,""

^CONN: 1,0

OK

^CEND: 1,6, 67, 16

NO CARRIER

*/
u8 CALL_Timer =0 ; //打电话过程中用到的定时器
u16 CEND_time = 0; //提取CEND返回上来的通话时间，如果是0说明通话没有被接通
u8 GSM_CALL(u8 * p_number, u8 number_len)
{
	u8 call_arry[18]= {0};
  CEND_time = 0;//把通话时长清零，在串口程序中处理
  Flag_all.ORIG_Flag =0;
  Flag_all.CALL_Flag =0;
  Flag_all.CONF_Flag =0;
  Flag_all.CONN_Flag =0;
  Flag_all.CEND_Flag =0;
  Flag_all.NO_CARRIER = 0;

  strcpy(call_arry,"ATD");
  memcpy(call_arry+3,p_number,number_len);
  call_arry[3+number_len]=';' ;
  call_arry[4+number_len]='\r' ;
  call_arry[5+number_len]='\n' ;
  Flag_all.CALL_Flag =1; //将拨号操作标志，置1，方便在串口中检测想要的命令
  Usart3_SendData(call_arry,6+number_len);
  CALL_Timer = 0;
	while(1)//(!Flag_all.CEND_Flag)&&(!Flag_all.NO_CARRIER))
	{
    IWDG_Feed();
		if(CALL_Timer >10) //超过10s都没有ORIG和CONF回来，则此次呼叫可能异常
		{
      if((!Flag_all.ORIG_Flag)&&(!Flag_all.CONF_Flag))
      {
        return CALL_ERROR;
      }			
    }
		if(!Flag_all.ORIG_Flag)
		{
      
    }		
		
		if(!Flag_all.CONF_Flag) //接收到回铃音
		{  //长时间没有接到回铃声，认为有错误，这个时间定义为10s(个人猜测的值，有待确认)
			
      
    }
		
		if(!Flag_all.CONN_Flag)//电话还没有接通
		{
      
    }
		
		if(Flag_all.CEND_Flag)
		{
      if(CEND_time >1)  //电话接通过，可正常退出通话状态，转到GPRS状态
			{
				Flag_all.CALL_Flag = 0; //呼叫动作标志清零
        return CEND_NORMAL; //通话正常结束，调用函数收到这个后回去判断GPRS连接状态，

      }
			else
			{
        if(!Flag_all.CONF_Flag) //没有收到回铃声，说明拨号异常
				{
          Flag_all.CALL_Flag = 0; //呼叫动作标志清零
					return CEND_UNUSUAL; //通话时间为0，没有接通就被挂断了，这个时候，调用程序可能会重新进行拨号
        }
        else //电话正常呼叫，但是没人接听
        {
          Flag_all.CALL_Flag = 0; //呼叫动作标志清零
          return CEND_NORMAL; //通话正常结束，调用函数收到这个后回去判断GPRS连接状态，
        }					
      }				
    }
    
    if(Flag_all.NO_CARRIER)
		{

    }	
		
	  if(Flag_all.NO_ANSWER ) //返回：NO ANSWER 无人接听，有这条指令，以后可根据需要检测
    {

    }
  }
	IWDG_Feed();
}

u8 Check_Net_timeover = 0; //网络检测，超时复位
void Check_Net()
{
	static cnt = 0;
  if(Flag_all.SendHeart_Flag)   //&&(!Flag_all.Check_Net_Send))
	{
    if(Check_Net_timeover >10) //10S定时器
		{
      cnt++;
			Check_Net_timeover = 0; //10s定时器清零
			Flag_all.SendHeart_Flag =1;
			Flag_all.Check_Net_Send = 1;
			upheart_timeout_count = 0; //将心跳超时5min，计时器清零，因为通话的过程中该计时器还在工作
			Send_Heart(); //发送心跳
    }
  }
	if(!Flag_all.SendHeart_Flag) //心跳标志为0，有两种可能
	{
		if(Flag_all.Check_Net_Send)
		{
      Flag_all.Check_Net_Send = 0; //在Check_Net()函数中发送了心跳标志
			Flag_all.Check_Net_Flag = 0; //检测网络状态标志
    }
	  else
		{
      Flag_all.SendHeart_Flag =1;
			Flag_all.Check_Net_Send = 1;
			Send_Heart(); //发送心跳
    }    
  }
	if(cnt>3)
	{
    Usart2_SendData((u8 *)"Net Bad, Reset MCU...\r\n",sizeof("Net Bad, Reset MCU...\r\n"));
		SystemReset();
  }
}


































