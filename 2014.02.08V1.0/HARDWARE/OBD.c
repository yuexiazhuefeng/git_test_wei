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
#include "OBD.h"


const u8 OBD_compile_data_arry[6]={0x55,0xAA,0x00,0x02,0x33,0x35};
const u8 OBD_Versionsi_arry[6]={0x55,0xAA,0x00,0x02,0x40,0x42};
const u8 OBD_Read_VIN_arry[6]={0x55,0xAA,0x00,0x02,0x49,0x4B};
const u8 OBD_Read_SN_arry[6]={0x55,0xAA,0x00,0x02,0x50,0x52};
const u8 Read_Sleep_Parameter_arry[6]={0x55,0xAA,0x00,0x02,0x25,0x27};
const u8 Close_Sleep_Parameter_arry[8]={0x55,0xAA,0x00,0x04,0x24,0x00,0x00,0x28};
const u8 OBD_mileage_arry[7]={0x55,0xAA,0x00,0x03,0x43,0x8A,0xD0};
const u8 Read_oil_mass_arry[7]={0x55,0xAA,0x00,0x03,0x43,0x8B,0xD1};
const u8 Read_oil_LperH_arry[7]={0x55,0xAA,0x00,0x03,0x43,0x88,0xCE};
const u8 Read_Body_state_arry[6]={0x55,0xAA,0x00,0x02,0x44,0x46};
//const u8 Read_Sleep_Parameter_arry[6]={0x55,0xAA,0x00,0x02,0x25,0x27};

u8 OBD_mileage_buffer_arry[4]={0}; //里程存储数组，读取的里程是四个char类型的数据，直接转移到这里就行了
u8 OBD_oil_mass_buffer_arry[2]={0};//剩余油量缓存数组，读取的剩余油量值是2个char类型的数据，直接转移放到这里就行了

u8 OBD_init_timer = 0;
u8 * add =0;

u8 OBD_PID_Buffer[OBD_BUF_LEN] = {0x00};
RING_T ring_obd_buffer;

u8 OBD_Send_Commnd( u8 *p,u8 length,u8 delay_count) //指令成功返回1; 不成功返回0
{ //GSM AT指令发送子函数，发送不成功的话，有五次机会
	//*p:待发送命令的地址; length:发送指令的长度; delay_count:发送指令后到判断OK的等待时间,单位100ms
  u8 *pOBD_commnd = p;
	u8 send_length = length;
	u8 delay = delay_count;
	u8 i = 0;
	OBD_Com_Flag.OBD_Received_Complete = 0;
	OBD_Com_Flag.OBD_Received_Body = 0;
	OBD_Com_Flag.OBD_Error = 0;
	OBD_Com_Flag.OBD_Received_PID =0;
	for(i=0;i<5;i++)
	{
     if(OBD_Com_Flag.OBD_Received_Complete) //完成OBD的数据接收
		 {
       OBD_Com_Flag.OBD_Received_Complete = 0;
			 return 1;
     }
		 if(OBD_Com_Flag.OBD_Received_Body)//接收到车身状态信息是的处理方法
		 {
       OBD_Com_Flag.OBD_Received_Body = 0;	
      	//diaoyongcheshenzhuangtaichuli();
			  return 2;
     }
		 if(OBD_Com_Flag.OBD_Error)//接收到车身状态信息是的处理方法
		 {
       //OBD_Com_Flag.OBD_Received_Body = 0;	
      	//diaoyongcheshenzhuangtaichuli();
			  return 3;
     }
		 if(OBD_Com_Flag.OBD_Received_PID)  //接收到PID数据流
		 { 
       return 4 ;
     }
		 memset(usart1_Rx_backup,0x00,50);//发送AT指令之前，先把串口3的接收缓存备份清空。
		 memset(usart1_Rx_buffer,0x00,50);//发送AT指令之前，先把串口3的接收缓存清空。
		 usart1_Rx_length =0 ;
		 Usart1_SendData(pOBD_commnd,send_length);		 
		 delay_Nx100ms(delay);
  }
	OBD_Com_Flag.OBD_Received_Complete = 0;
	OBD_Com_Flag.OBD_Received_Body = 0;
	OBD_Com_Flag.OBD_Error = 0;
	memset(usart1_Rx_buffer,0x00,50);
	usart1_Rx_length =0 ;
	return 0;	
}
///const u8 ascii_arry[]=
void OBD_init(void) //OBD初始化程序，主要是
{
  u8 i = 0;
	u8 j=0;
	u8 temp[17]={0};
  ring_obd_buffer.size = OBD_BUF_LEN;
  ring_obd_buffer.data = OBD_PID_Buffer;
  RING_INIT(&ring_obd_buffer,ring_obd_buffer.size);
//读取版本信息55 AA 00 02 40 42
//55 AA 00 08 C0 50 04 00 01 01 00 1E 
//0  1	2  3  4  5  6  7  8  9  10 11
	Usart2_SendData("Read OBD Mode Versionsi...\r\n",28);
	for(i=0;i<5;i++)
	{
		add = (u8 *)OBD_Versionsi_arry;
		if(OBD_Send_Commnd((u8 *)OBD_Versionsi_arry,OBD_Versionsi_len,3))//读取编译日期
		{
			if(OBD_Com_Flag.OBD_Error)
			{
				OBD_Error_Judge();      //调用错误判断函数
				OBD_Com_Flag.OBD_Error = 0;
				break;
			}
			if(usart1_Rx_backup[4]==0xC0)//收到编译时间
			{
				memset(temp,0x00,17);
				for(j=0;j<6;j++)
				{
          temp[2*j]=((usart1_Rx_backup[5+j]>>4)&0x0f)+0x30;
					temp[2*j+1]=(usart1_Rx_backup[5+j]&0x0f)+0x30;					
        }
				Usart2_SendData("OBD Mode Versionsi is :",23);
				Usart2_SendData(temp,12);
				Usart2_SendData("\r\n",2);
				break;
			}
		}
		else
		{
			delay_Nx100ms(10);//等待1S 
			if(i == 4)
			{
				Usart2_SendData("OBD Error...\r\n",14);
				break;
				//return 0;
			}					
		}
	}
//读取编译日期55 AA 00 02 33 35
//55 AA 00 06 B3 20 13 08 30 24
//0  1  2  3  4  5  6  7  8  9	
	Usart2_SendData("Read OBD Mode Compile Data...\r\n",31);
	for(i=0;i<5;i++)
	{			
		if(OBD_Send_Commnd((u8 *)OBD_compile_data_arry,BD_compile_data_len,3))//读取编译日期
		{
			if(OBD_Com_Flag.OBD_Error)
			{
				OBD_Error_Judge();      //调用错误判断函数
				OBD_Com_Flag.OBD_Error = 0;
				break;
			}
			else if(usart1_Rx_backup[4]==0xB3)//收到编译时间
			{
				for(j=0;j<4;j++)
				{
          temp[2*j]=((usart1_Rx_backup[5+j]>>4)&0x0f)+0x30;
					temp[2*j+1]=(usart1_Rx_backup[5+j]&0x0f)+0x30;					
        }
				Usart2_SendData("OBD Mode Compile Data is :",26);
				Usart2_SendData(temp,8);
				Usart2_SendData("\r\n",2);				
				break;
			}
		}		
		else
		{
			delay_Nx100ms(10);//等待1S 
			if(i == 4)
			{
				Usart2_SendData("OBD Error...\r\n",14);
				break;
				//return 0;
			}					
		}
	}

//读取汽车的VIN码55 AA 00 02 49 4B
//55 AA 00 13 C9 31 32 33 34 35 36 37 38 39 41 42 43 44 45 46 47 48 DD
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22
	Usart2_SendData("Read VIN...\r\n",13);
	for(i=0;i<5;i++)
	{
		if(OBD_Send_Commnd((u8 *)OBD_Read_VIN_arry,OBD_Read_VIN_len,3))//读取编译日期
		{
			if(OBD_Com_Flag.OBD_Error)
			{
				OBD_Error_Judge();      //调用错误判断函数
				OBD_Com_Flag.OBD_Error = 0;
				break;
			}
			if(usart1_Rx_backup[4]==0xC9)//收到编译时间
			{
				memset(temp,0x00,17);
				for(j=0;j<17;j++)
				{
          temp[j]=usart1_Rx_backup[5+j];  //(usart1_Rx_backup[5+j]>=0x41)?(usart1_Rx_backup[5+j]+0x41):(usart1_Rx_backup[5+j]+0x30);//((usart1_Rx_backup[5+j]>>4)&0x0f)+0x30;
					//temp[2*j+1]=(usart1_Rx_backup[5+j]&0x0f)+0x30;					
        }
				Usart2_SendData("The VIN is :",12);
				Usart2_SendData(temp,17);
				Usart2_SendData("\r\n",2);
				break;
			}
		}		
		else
		{
			delay_Nx100ms(10);//等待1S 
			if(i == 4)
			{
				Usart2_SendData("OBD Error...\r\n",14);
				break;
				//return 0;
			}					
		}
	}
//读产品序列号SN:55 AA 00 02 50 52
//55 AA 00 0E D0 05 D5 FF 30 37 30 4D 41 43 21 46 48 CE 
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 
	Usart2_SendData("Read SN...\r\n",12);
	for(i=0;i<5;i++)
	{
		if(OBD_Send_Commnd((u8 *)OBD_Read_SN_arry,OBD_Read_SN_len,3))//读取编译日期
		{
			if(OBD_Com_Flag.OBD_Error)
			{
				OBD_Error_Judge();      //调用错误判断函数
				OBD_Com_Flag.OBD_Error = 0;
				break;
			}
			if(usart1_Rx_backup[4]==0xD0)//收到编译时间
			{
				memset(temp,0x00,17);
				for(j=0;j<12;j++)
				{
          temp[2*j]=(((usart1_Rx_backup[5+j]>>4)&0x0f)>=0x0A)?(((usart1_Rx_backup[5+j]>>4)&0x0f)+0x37):(((usart1_Rx_backup[5+j]>>4)&0x0f)+0x30);//((usart1_Rx_backup[5+j]>>4)&0x0f)+0x30;
					temp[2*j+1]=((usart1_Rx_backup[5+j]&0x0f)>=0x0A)?((usart1_Rx_backup[5+j]&0x0f)+0x37):((usart1_Rx_backup[5+j]&0x0f)+0x30);					
        }
				Usart2_SendData("The SN is :",11);
				Usart2_SendData(temp,24);
				Usart2_SendData("\r\n",2);
				break;
			}
		}		
		else
		{
			delay_Nx100ms(10);//等待1S 
			if(i == 4)
			{
				Usart2_SendData("OBD Error...\r\n",14);
				break;
				//return 0;
			}					
		}
	}
//读取睡眠模式和睡眠时间55 AA 00 02 25 27
//55 AA 00 04 A5 01 3C E6 
//0  1  2  3  4  5  6  7
	Usart2_SendData("Read Sleep Mode...\r\n",20);
	for(i=0;i<5;i++)
	{
		if(OBD_Send_Commnd((u8 *)Read_Sleep_Parameter_arry,Read_Sleep_Parameter_len,3))//读取睡眠信息
		{
			if(OBD_Com_Flag.OBD_Error)
			{
				OBD_Error_Judge();      //调用错误判断函数
				OBD_Com_Flag.OBD_Error = 0;
				break;
			}
			if(usart1_Rx_backup[4]==0xA5)//收到编译时间
			{
				memset(temp,0x00,17);
				if(usart1_Rx_backup[5]==0x01)
				   Usart2_SendData("OBD Sleep Mode : OPEN\r\n",23);
				if(usart1_Rx_backup[5]==0x00)
				{
          Usart2_SendData("OBD Sleep Mode : CLOSED\r\n",25);
					break;
        }				   				
				if(usart1_Rx_backup[6]<10)
				{
					temp[0]=usart1_Rx_backup[6]+0x30;
					i=1;
				}
				if((usart1_Rx_backup[6]>9)&&(usart1_Rx_backup[6]<100))
				{
					temp[0]=usart1_Rx_backup[6]/10+0x30;
					temp[1]=(usart1_Rx_backup[6]-(usart1_Rx_backup[6]/10)*10)+0x30;
					i=2;
				}
				if(usart1_Rx_backup[6]>99)
				{
					temp[0]=usart1_Rx_backup[6]/100+0x30;
					temp[1]=(usart1_Rx_backup[6]/10-(usart1_Rx_backup[6]/100)*10)+0x30;
					temp[2]=(usart1_Rx_backup[6]-(usart1_Rx_backup[6]/10)*10)+0x30;
					i=3;
				}									
				Usart2_SendData("Sleep Time is :",15);
				Usart2_SendData(temp,i);
				Usart2_SendData("\r\n",2);
				break;
			}
		}		
		else
		{
			delay_Nx100ms(10);//等待1S 
			if(i == 4)
			{
				Usart2_SendData("OBD Error...\r\n",14);
				break;
				//return 0;
			}					
		}
	}
//读取车身状态信息
//55 AA 00 02 44 46
//0  1  2  3  4  5	
	Usart2_SendData("Read Body State...\r\n",20);
	for(i=0;i<5;i++)
	{
		if(0x02==OBD_Send_Commnd((u8 *)Read_Body_state_arry,Read_Body_state_len,3))//读取睡眠信息
		  break ;
		else
		{
			delay_Nx100ms(10);//等待1S 
			if(i == 4)
			{
				Usart2_SendData("OBD Error...\r\n",14);
				break;
				//return 0;
			}					
		}
	}
	
	SPI_Flash_SafeRead(temp,CAR_TYPE_ADDR,0x02);
	if((temp[0]==0xff)||(temp[1]==0xff))
	  Mark_Car(04,00);
	else
	  Mark_Car(temp[0],temp[1]);
	//Body_Control(01,01);
	//OBD_Sleep(0x01,0x00,0x00);
	//OBD_Sleep(0x00,0x00,0x00);
	//OBD_Sleep(0x00,0x01,0x70);
	//OBD_Sleep(0x01,0x00,0x00);
	 mileage();
	 Read_oil_mass();
	 Read_oil_LperH();
}

void OBD_Data_Deal(void)
{
  u8 i = 0;
//55 AA 00 06 B3 20 13 08 30 24
//0  1  2  3  4  5  6  7  8  9	
	for(i=0;1<5;i++)
	{
    if(OBD_Send_Commnd((u8 *)OBD_compile_data_arry,OBD_compile_data_len,3))
	  {
      
    }
  }
	if(OBD_Send_Commnd((u8 *)OBD_compile_data_arry,OBD_compile_data_len,3))
	{

  }
	if(OBD_Com_Flag.OBD_compile_data)
		OBD_Com_Flag.OBD_compile_data =0;
}

void OBD_Error_Judge(void)  //处理OBD返回7F错误信息，判读错误类型
{
  switch (usart1_Rx_backup[5])
	{
    case 0xB3: OBD_Error_Judge_zi(); break; 			  
				
    case 0xC0: OBD_Error_Judge_zi(); break; 
			  
    case 0xC1: OBD_Error_Judge_zi(); break; 
			 
    case 0xC2: OBD_Error_Judge_zi(); break; 
			  
		case 0xC3: OBD_Error_Judge_zi(); break; 
			  
		case 0xC4: OBD_Error_Judge_zi(); break; 
			  
		case 0xC5: OBD_Error_Judge_zi(); break; 
			  
		case 0xC6: OBD_Error_Judge_zi(); break; 
			 
		case 0xC7: OBD_Error_Judge_zi(); break; 
			  
		case 0xC8: OBD_Error_Judge_zi(); break; 		  
				
		case 0xC9: OBD_Error_Judge_zi(); break; 
		
		case 0xCB: OBD_Error_Judge_zi(); break; 			 
				
		case 0xD0: OBD_Error_Judge_zi(); break; 
			  
		case 0xD6: OBD_Error_Judge_zi(); break; 
			  
		case 0xD7: OBD_Error_Judge_zi(); break;

    case 0xD8: OBD_Error_Judge_zi(); break;

    case 0xA3: OBD_Error_Judge_zi(); break;

    case 0xA4: OBD_Error_Judge_zi(); break;

    case 0xA5: OBD_Error_Judge_zi(); break;

    case 0xA6: OBD_Error_Judge_zi(); break;   
			  
  }
}

void OBD_Error_Judge_zi(void)  //处理OBD返回7F错误信息，判读错误类型00,01,02,03
{
  switch (usart1_Rx_backup[6])
	{
    case 0x00: Usart2_SendData("OBD Error Type : 00\r\n",sizeof("OBD Error Type : 00\r\n")); OBD_Com_Flag.OBD_error_type =0x00; break;
		case 0x01: Usart2_SendData("OBD Error Type : 01\r\n",sizeof("OBD Error Type : 00\r\n")); OBD_Com_Flag.OBD_error_type =0x01; break;
		case 0x02: Usart2_SendData("OBD Error Type : 02\r\n",sizeof("OBD Error Type : 00\r\n")); OBD_Com_Flag.OBD_error_type =0x02; break;
		case 0x03: Usart2_SendData("OBD Error Type : 03\r\n",sizeof("OBD Error Type : 00\r\n")); OBD_Com_Flag.OBD_error_type =0x03; break;					
  }
}


//标定车型函数，参数：high,车型代码的高字节; low,车型代码的低字节
u8 Mark_Car(u8 high, u8 low)
{
  u8 i =0;
	u8 temp[8] ={0x55,0xAA,0x00,0x04,0x4B};
  temp[5]=high;
  temp[6]=low;
  temp[7]=temp[2]+temp[3]+temp[4]+temp[5]+temp[6];
//标定车型55 AA 00 04 4B 04 00 53 (例：标定为丰田的车型，)
//55 AA 00 04 CB 04 00 D3 
//0  1  2  3  4  5  6  7 
	Usart2_SendData("Mark The Car...\r\n",17);
	for(i=0;i<5;i++)
	{
		if(OBD_Send_Commnd((u8 *)temp,8,3))//读取编译日期
		{
			if(OBD_Com_Flag.OBD_Error)
			{
				OBD_Error_Judge();      //调用错误判断函数
				OBD_Com_Flag.OBD_Error = 0;
				//break;
				return 0;
			}
			if(usart1_Rx_backup[4]==0xCB)//收到编译时间
			{
				if((temp[5]==usart1_Rx_backup[5])&&(temp[6]==usart1_Rx_backup[6]))
				{
					Usart2_SendData("success\r\n",9);
          return 1;
        }
			}
		}		
		else
		{
			delay_Nx100ms(10);//等待1S 
			if(i == 4)
			{
				Usart2_SendData("Failed...\r\n",11);
				//break;
				return 0;
			}					
		}
	}
	return 0;
}

u8 Body_Control(u8 comm, u8 parameter)  //车身控制函数，
{
  u8 i =0;
	u8 temp[8] ={0x55,0xAA,0x00,0x04,0x23};
  temp[5]=comm;
  temp[6]=parameter;
  temp[7]=temp[2]+temp[3]+temp[4]+temp[5]+temp[6];
//车身控制55 AA 00 04 23 03 01 2B (例，升窗)
//        0  1  2  3  4  5  6  7
//返回55 AA 00 03 A3 00 A6 
//    0  1  2  3  4  5  6 
  if(temp[6]==1)
	{
    if(temp[5]==0x01)
			Usart2_SendData("Open The Door...\r\n",18);
		else if(temp[5]==0x02)
			Usart2_SendData("Open The Trumpet...\r\n",21);
		else if(temp[5]==0x03)
			Usart2_SendData("Open The Hazard Light...\r\n",26);
		else if(temp[5]==0x04)
			Usart2_SendData("Rolling Up Window...\r\n",22);
  }
	else if(temp[6]==0x00)
	{
    if(temp[5]==0x01)
			Usart2_SendData("Close The Door...\r\n",19);
		else if(temp[5]==0x02)
			Usart2_SendData("Close The Trumpet...\r\n",22);
		else if(temp[5]==0x03)
			Usart2_SendData("Close The Hazard Light...\r\n",27);
		else if(temp[5]==0x04)
			Usart2_SendData("Stop Rolling Up Window...\r\n",27);
  }
	for(i=0;i<5;i++)
	{
		if(OBD_Send_Commnd((u8 *)temp,8,3))//读取编译日期
		{
			if(OBD_Com_Flag.OBD_Error)
			{
				OBD_Error_Judge();      //调用错误判断函数
				OBD_Com_Flag.OBD_Error = 0;
				//break;
				return 0;
			}
			if(usart1_Rx_backup[4]==0xA3)//收到编译时间
			{
				if(usart1_Rx_backup[5]==0x01)
				{
					Usart2_SendData("success\r\n",9);
          return 1;
        }
				else if(usart1_Rx_backup[5]==0x00)
				{
					Usart2_SendData("Failed\r\n",8);
          return 1;
        }
			}
		}		
		else
		{
			delay_Nx100ms(10);//等待1S 
			if(i == 4)
			{
				Usart2_SendData("Failed...\r\n",11);
				//break;
				return 0;
			}					
		}
	}
	return 0;
}

//三个参数,
//type:0x00,设置睡眠; 0x01,读取睡眠状态;
//com: 0x00,禁用睡眠功能; 0x01,启用睡眠功能
//parameter: 睡眠时间，默认60s，即系统监控汽车进入休眠后或者在休眠时被外界唤醒后60s后睡眠，时间可以修改为10--255s
u8 OBD_Sleep(u8 type,u8 com,u8 parameter)
{
  u8 i =0;
	u8 typei =type;
	u8 comi = com;
	u8 parameteri = parameter;
	u8 temp[8] ={0};
//发送：55 AA 00 02 25 27
//返回：55 AA 00 04 A5 01 3C E6 
//      0  1  2  3  4  5  6  7
	if(typei==0x01) //读取睡眠状态 0x25命令
	{
    Usart2_SendData("Read Sleep Mode...\r\n",20);
		for(i=0;i<5;i++)
		{
			if(OBD_Send_Commnd((u8 *)Read_Sleep_Parameter_arry,Read_Sleep_Parameter_len,3))//读取睡眠信息
			{
				if(OBD_Com_Flag.OBD_Error)
				{
					OBD_Error_Judge();      //调用错误判断函数
					OBD_Com_Flag.OBD_Error = 0;
					break;
				}
				if(usart1_Rx_backup[4]==0xA5)//返回收到读取时间命令
				{
					memset(temp,0x00,8);
					if(usart1_Rx_backup[5]==0x01)
						 Usart2_SendData("OBD Sleep Mode : OPEN\r\n",23);
					if(usart1_Rx_backup[5]==0x00)
					{
            Usart2_SendData("OBD Sleep Mode : CLOSED\r\n",25);	
						break;
          }						 			
					if(usart1_Rx_backup[6]<10)
					{
						temp[0]=usart1_Rx_backup[6]+0x30;
						i=1;
					}
					if((usart1_Rx_backup[6]>9)&&(usart1_Rx_backup[6]<100))
					{
						temp[0]=usart1_Rx_backup[6]/10+0x30;
						temp[1]=(usart1_Rx_backup[6]-(usart1_Rx_backup[6]/10)*10)+0x30;
						i=2;
					}
					if(usart1_Rx_backup[6]>99)
					{
						temp[0]=usart1_Rx_backup[6]/100+0x30;
						temp[1]=(usart1_Rx_backup[6]/10-(usart1_Rx_backup[6]/100)*10)+0x30;
					  temp[2]=(usart1_Rx_backup[6]-(usart1_Rx_backup[6]/10)*10)+0x30;
						i=3;
					}									
					Usart2_SendData("Sleep Time is :",15);
					Usart2_SendData(temp,i);
					Usart2_SendData("\r\n",2);
					break;
				}
			}		
			else
			{
				delay_Nx100ms(10);//等待1S 
				if(i == 4)
				{
					Usart2_SendData("OBD Error...\r\n",14);
					break;
					//return 0;
				}					
			}
		}
  }
//Set_Sleep_Parameter_arry[8]={0x55,0xAA,0x00,0x04,0x24,0x00,0x00,0x28};
//发送关闭睡眠：55 AA 00 04 24 00 00 28
//关闭睡眠返回：55 AA 00 02 A4 A6 	(成功)
//              0  1  2  3  4  5  6  7
	else if(typei==0x00) //设置睡眠时间
	{    
		if(comi ==0x00)
		{
      Usart2_SendData("Closing Sleep Mode...\r\n",23);
			for(i=0;i<5;i++)
			{
				if(OBD_Send_Commnd((u8 *)Close_Sleep_Parameter_arry,Close_Sleep_Parameter_len,3))//读取睡眠信息
				{
					if(OBD_Com_Flag.OBD_Error)
					{
						OBD_Error_Judge();      //调用错误判断函数
						OBD_Com_Flag.OBD_Error = 0;
						break;
					}
					if(usart1_Rx_backup[4]==0xA4)//收到关闭睡眠指令的返回
					{						
						Usart2_SendData("success\r\n",9);					
						break;
					}
				}		
				else
				{
					delay_Nx100ms(10);//等待1S 
					if(i == 4)
					{
						Usart2_SendData("OBD Error...\r\n",14);
						break;
						//return 0;
					}					
				}
			}
    }
		if(comi ==0x01)  //设置睡眠时间
		{
//发送：55 AA 00 04 24 01 60 89	(打开睡眠,并设置睡眠时间是60s)	
//      55 AA 00 02 A4 A6 (成功返回)
//      0  1  2  3  4  5  6  7
      Usart2_SendData("Setting Sleep Mode...\r\n",23);
			temp[0]=0x55;
			temp[1]=0xAA;
			temp[2]=0x00;
			temp[3]=0x04;
			temp[4]=0x24;
			temp[5]=0x01;
			temp[6]=parameteri;
			temp[7]=temp[2]+temp[3]+temp[4]+temp[5]+temp[6];
			for(i=0;i<5;i++)
			{
				if(OBD_Send_Commnd((u8 *)temp,8,3))//设置睡眠时间
				{
					if(OBD_Com_Flag.OBD_Error)
					{
						OBD_Error_Judge();      //调用错误判断函数
						OBD_Com_Flag.OBD_Error = 0;
						break;
					}
					if(usart1_Rx_backup[4]==0xA4)//收到设置时间成功返回
					{
            if(temp[6]<10)
						{
							temp[0]=temp[6]+0x30;
							i=1;
						}
						if((temp[6]>9)&&(temp[6]<100))
						{
							temp[0]=temp[6]/10+0x30;
							temp[1]=(temp[6]-(temp[6]/10)*10)+0x30;
							i=2;
						}
						if(temp[6]>99)
						{
							temp[0]=temp[6]/100+0x30;
							temp[1]=(temp[6]/10-(temp[6]/100)*10)+0x30;
					    temp[2]=(temp[6]-(temp[6]/10)*10)+0x30;
							i=3;
						}									
						Usart2_SendData("Set Time is :",13);
						Usart2_SendData(temp,i);
						Usart2_SendData("\r\n",2);						
						Usart2_SendData("success\r\n",9);							
						break;
					}
				}		
				else
				{
					delay_Nx100ms(10);//等待1S 
					if(i == 4)
					{
						Usart2_SendData("OBD Error...\r\n",14);
						break;
						//return 0;
					}					
				}
			}
    }		
  }
	return 1;
}

//usart1_Rx_backup里收到的上报：55 AA 00 0A C4 00 01 00 00 00 00 00 00 CF
//                              0  1  2  3  4  5  6  7  8  9  10 11 12 13
//                                             1  2  3  4  5  6
void Body_state_deal(void)
{
  if(usart1_Rx_backup[5]&0x03) //0000 0011前左或前右，有一个门开了即前门开 
	{
    Body_state =Body_state|(((u32)0x01)<<13);
		if((!OBD_Com_Flag.FDoor_State_Flag)&&(!OBD_Com_Flag.OBD_ReadBodyState_Flag)) //门状态发生改变且状态改变标志为0
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //将状态标志置1
		OBD_Com_Flag.FDoor_State_Flag =1;
  }  
  else
	{
    Body_state =Body_state&(~(((u32)0x01)<<13)); 
		if((OBD_Com_Flag.FDoor_State_Flag)&&(!OBD_Com_Flag.OBD_ReadBodyState_Flag)) //门状态发生改变且状态改变标志为0
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //将状态标志置1
		OBD_Com_Flag.FDoor_State_Flag =0;
  }
	   
  if(usart1_Rx_backup[5]&(0x03<<2))//0000 1100后右或后左，有一个门开了即后门开
	{
    Body_state =Body_state|(((u32)0x01)<<14);
		if((!OBD_Com_Flag.MDoor_State_Flag)&&(!OBD_Com_Flag.OBD_ReadBodyState_Flag)) //门状态发生改变且状态改变标志为0
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //将状态标志置1
		OBD_Com_Flag.MDoor_State_Flag =1;
  }		  
  else
	{
    Body_state =Body_state&(~(((u32)0x01)<<14)); 
		if((OBD_Com_Flag.MDoor_State_Flag)&&(!OBD_Com_Flag.OBD_ReadBodyState_Flag)) //门状态发生改变且状态改变标志为0
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //将状态标志置1
		OBD_Com_Flag.MDoor_State_Flag =0;
  }
	    
	if(usart1_Rx_backup[5]&(((u32)0x01)<<4))//0001 0000尾箱开
	{
    Body_state =Body_state|(((u32)0x01)<<15);
		if((!OBD_Com_Flag.BDoor_State_Flag)&&(!OBD_Com_Flag.OBD_ReadBodyState_Flag)) //门状态发生改变且状态改变标志为0
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //将状态标志置1
		OBD_Com_Flag.BDoor_State_Flag =1;
  }		  
  else
	{
    Body_state =Body_state&(~(((u32)0x01)<<15)); 	
		if((OBD_Com_Flag.BDoor_State_Flag)&&(!OBD_Com_Flag.OBD_ReadBodyState_Flag)) //门状态发生改变且状态改变标志为0
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //将状态标志置1
		OBD_Com_Flag.BDoor_State_Flag =0;		  
  }
	    
	if(usart1_Rx_backup[6])//ACC开
	{
    Body_state =Body_state|((u32)0x01);
		if(!OBD_Com_Flag.ACC_state)  //如果之前ACC为关
		{
      OBD_Com_Flag.OBD_ACC_Turn = L_TO_H;  //2
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //有状态改变标志，置1
    }		  
		OBD_Com_Flag.ACC_state =1;//ACC开
  }		
	else
	{
    Body_state =Body_state&(0xFFFFFFFE);
		if(OBD_Com_Flag.ACC_state)  //如果之前ACC为开
		{
      OBD_Com_Flag.OBD_ACC_Turn = H_TO_L;  //1
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //有状态改变标志，置1
    }		  
		OBD_Com_Flag.ACC_state =0;//ACC关
		OBD_Com_Flag.OBD_Mileage_Receivded_FLag = 0; //将里程读取完成标志清零
  }		
	if((usart1_Rx_backup[7]==0x01)||(!(usart1_Rx_backup[10]&0x10)))//有遥控器开锁，出警戒，不用考虑ACC状态。遥控开锁和钥匙开锁是有区别的
	{
    OBD_Com_Flag.alarm = UNLOCK;//把警戒标志清零，出警戒。
		Body_state = Body_state&(~(((u32)0x01)<<31)); //将第31位，警戒状态位清零，表示退出警戒
  }
//55 AA 00 0A C4 00 00 00 00 00 10 00 00 DE 
//0  1  2  3  4  5  6  7  8  9  10 11 12 13	
  else if(((usart1_Rx_backup[7]==0x02)&&(!(usart1_Rx_backup[6])))||(usart1_Rx_backup[10]&0x10))//遥控器按下锁门，且ACC处于关闭状态，进入警戒。
	{
    OBD_Com_Flag.alarm = LOCK;       //把警戒标志置位，进入警戒。
		Body_state = Body_state|(((u32)0x01)<<31); //将警戒状态位置1，表示进入警戒
  }

/******报警标志位判断***************************/
//报警类型
//1-门开类报警	OBD_Com_Flag.door = 1;
//2-刹车报警
//3-ACC报警
//
	if(OBD_Com_Flag.alarm == LOCK)  //提取报警标志
	{//门，刹车，ACC.还少震动，震动在IO检测中做。
		if(usart1_Rx_backup[5]&0x1F) //0001 1111前门后门后备箱，有一个开则开。 
		{ 
			alarm_state =alarm_state|(((u32)0x01)<<31);//非法开门报警，是第31位
			if(!OBD_Com_Flag.door)
				OBD_Com_Flag.Have_alarm = 1; //将有警情标志置1
      OBD_Com_Flag.door = 1;  //有门开报警
			
    }
		else
		{
      alarm_state =alarm_state&(~(((u32)0x01)<<31)); //没有非法开门
			if(OBD_Com_Flag.door)
				OBD_Com_Flag.Have_alarm = 1; //将有警情标志置1
			OBD_Com_Flag.door = 0;
    }	 
		
		if(usart1_Rx_backup[6])//ACC开
		{
      alarm_state =alarm_state|(((u32)0x01)<<27); 
			if(!OBD_Com_Flag.ACC_alarm)
				OBD_Com_Flag.Have_alarm = 1; //将有警情标志置1
			OBD_Com_Flag.ACC_alarm=1; //有ACC报警			
    }		  
	  else
		{
      //alarm_state =(~(0x01<<27));
			alarm_state = alarm_state&(~(((u32)0x01)<<27));
			if(OBD_Com_Flag.ACC_alarm)
				OBD_Com_Flag.Have_alarm = 1; //将有警情标志置1
			OBD_Com_Flag.ACC_alarm=0;
    }
		if(usart1_Rx_backup[10]&(0x1<<0x03)) //0000 1000判断刹车是否踩下
		{
      alarm_state =alarm_state|(((u32)0x01)<<15);//非法刹车
			//alarm_state =alarm_state|(((u32)0x01)<<31);//非法开门报警，是第31位
			if(!OBD_Com_Flag.brake)
				OBD_Com_Flag.Have_alarm = 1; //将有警情标志置1
			OBD_Com_Flag.brake =1;  //有踩刹车报警			
    }	    
    else
		{
      alarm_state =alarm_state&(~(((u32)0x01)<<15)); //没有非法刹车    
		//	alarm_state =alarm_state&(~(((u32)0x01)<<31)); //没有非法开门
			if(OBD_Com_Flag.brake)
				OBD_Com_Flag.Have_alarm = 1; //将有警情标志置1
			OBD_Com_Flag.brake =0;
    }
 /*   if(OBD_Com_Flag.OBD_ReadBodyState_Flag)
		{
      
			OBD_Com_Flag.Have_alarm = 0; //为了避免每次读状态的时候，一直操作这位，导致报警频繁
    }*/
  }
/*
	if(OBD_Com_Flag.OBD_ReadBodyState_Flag) //如果有主动读取标志，这步只是为了以防万一OBD_Com_Flag.Have_alarm 被置位
	{
    //OBD_Com_Flag.OBD_ReadBodyState_Flag = 0; //在这里将主动读取标志清零
		OBD_Com_Flag.Have_alarm = 0; //为了避免每次读状态的时候，一直操作这位，导致报警频繁
  } */
/*	else  //没有主动读取标志，说明是OBD模块主动上报的，此时认为有车身状态发生改变
	{
    //OBD_Com_Flag.OBD_BodyState_Turn_Flag = 1;  //车身状态发生改变，将改变标志位置1 ，该位在上传过位置信息后会被相应为清零
		OBD_Com_Flag.OBD_ReadBodyState_Flag = 0;
  }	*/

}
	

//读取里程,汽车里程表的读数
//发送55 AA 00 03 43 8A D0
//返回55 AA 00 07 C3 8A 00 08 05 E3 44 
//    0  1  2  3  4  5  6  7  8  9  10
void mileage(void)
{
  u8 i =0;
	
	if(OBD_Com_Flag.ACC_state) //如果ACC是开，才会进行下面的动作
	{	
		//Usart2_SendData("Read mileage...\r\n",17);
		for(i=0;i<5;i++) //如果读取失败的话，会读5次
		{
			if(OBD_Send_Commnd((u8 *)OBD_mileage_arry,OBD_mileage_len,3))//读取里程信息
			{
				if(OBD_Com_Flag.OBD_Error)
				{
					OBD_Error_Judge();      //调用错误判断函数
					if(OBD_Com_Flag.OBD_error_type==1 ||OBD_Com_Flag.OBD_error_type==2)
					{
						OBD_Com_Flag.OBD_Mileage_Support_FLag = 1;
						memcpy(OBD_mileage_buffer_arry,0x00,4);//清空里程读取缓存
					}					
					OBD_Com_Flag.OBD_Error = 0; //把收到7F应答错误清零。
					break;
				}
				if((usart1_Rx_backup[4]==0xC3)&&(usart1_Rx_backup[5]==0x8A))//收到里程信息
				{						
					//mileage_km=0 ;//stm32采用大端模式，即高位放在低地址中，RAM地址分配的时候是从低到高一次分配的，即先用低地址
					memcpy(OBD_mileage_buffer_arry,&usart1_Rx_backup[6],4);//将串口中的里程数据，转移到里程接收缓存数组里面
								 //经模拟确认，OBD_mileage_buffer_arry[0]存放的是里程最高位，OBD_mileage_buffer_arry[3]存放的是里程最低位8位
					//mileage_km = mileage_km|usart1_Rx_backup[9];
					//mileage_km = mileage_km|(usart1_Rx_backup[8]<<8);
					//mileage_km = mileage_km|(usart1_Rx_backup[7]<<16);
					//mileage_km = mileage_km|(usart1_Rx_backup[6]<<24);
					OBD_Com_Flag.OBD_Mileage_Receivded_FLag =1;//里程读取完成标志置1
					Usart2_SendData("success\r\n",9);					
					break;
				}
			}		
			else
			{
				delay_Nx100ms(3);//等待300ms
				if(i == 4)
				{
					Usart2_SendData("OBD Error...\r\n",14);
					break;
					//return 0;
				}					
			}
		}
	}
}

//发送55 AA 00 03 43 8B D1
//返回55 AA 00 07 C3 8B 00 00 00 87 DC 
//    0  1  2  3  4  5  6  7  8  9  10
void Read_oil_mass(void)
{
  u8 i =0;
  Usart2_SendData("Read oil mass...\r\n",18);
	if(OBD_Com_Flag.ACC_state)  //ACC为开的状态才去读取油表的剩余油量
	{	
		for(i=0;i<5;i++)
		{
			if(OBD_Send_Commnd((u8 *)Read_oil_mass_arry,Read_oil_mass_len,3))//读取睡眠信息
			{
				if(OBD_Com_Flag.OBD_Error)
				{
					OBD_Error_Judge();      //调用错误判断函数
					if(OBD_Com_Flag.OBD_error_type==1 ||OBD_Com_Flag.OBD_error_type==2)
					{
						OBD_Com_Flag.OBD_oil_massSupport_FLag = 1; //不支持剩余油量读取，标志置1
						memset(OBD_oil_mass_buffer_arry,0x00,2);//清空剩余油量读取缓存
					}					
					OBD_Com_Flag.OBD_Error = 0;
					break;
				}
				if((usart1_Rx_backup[4]==0xC3)&&(usart1_Rx_backup[5]==0x8B))//收到剩余油量返回
				{
					
					memcpy(OBD_oil_mass_buffer_arry,&usart1_Rx_backup[8],2); //将读取到的剩余油量值，直接转移到剩余油量缓存数组里面
						//经模拟确认，OBD_oil_mass_buffer_arry[0]存放的是里程最高位，OBD_oil_mass_buffer_arry[3]存放的是里程最低位8位
					//oil_mass=0 ;//stm32采用大端模式，即高位放在低地址中，RAM地址分配的时候是从低到高一次分配的，即先用低地址
					//oil_mass = oil_mass|usart1_Rx_backup[9];
					//oil_mass = oil_mass|(usart1_Rx_backup[8]<<8);
					OBD_Com_Flag.OBD_oil_massReceivded_FLag = 1;//剩余油量读取完成标志，此时可以访问剩余油量缓存
					Usart2_SendData("success\r\n",9); 
					break;
				}
			}
			else
			{
				delay_Nx100ms(10);//等待1S 
				if(i == 4)
				{
					Usart2_SendData("OBD Error...\r\n",14);
					break;
					//return 0;
				}
			}
		}
	}
}

//发送55 AA 00 03 43 88 CE
//返回55 AA 00 07 C3 88 00 00 00 0C 5E 
//    0  1  2  3  4  5  6  7  8  9  10
u8 Read_oil_LperH(void) //读取L/H瞬时油耗，这个是根据一段之间内累加后推算出来的每小时油耗
{
  u8 i =0;
	if(OBD_Com_Flag.ACC_state) //ACC开的状态下才去读油耗，不然没有意义
	{
    //Usart2_SendData("Read oil LperH...\r\n",19);
		for(i=0;i<5;i++)
		{
			if(OBD_Send_Commnd((u8 *)Read_oil_LperH_arry,Read_oil_LperH_len,3))//读取睡眠信息
			{
				if(OBD_Com_Flag.OBD_Error)
				{
					OBD_Error_Judge();           //调用错误判断函数
					OBD_Com_Flag.OBD_Error = 0;  //
					return 0xFF;
					//break;
				}
				if((usart1_Rx_backup[4]==0xC3)&&(usart1_Rx_backup[5]==0x88))//收到关闭睡眠指令的返回
				{ 
					oil_LperH=0 ;//stm32采用大端模式，即高位放在低地址中，RAM地址分配的时候是从低到高一次分配的，即先用低地址
					oil_LperH = oil_LperH|usart1_Rx_backup[9]; //L/H,用来算平均值,最大值是 0xDF
					//oil_LperH = oil_LperH|(usart1_Rx_backup[8]<<8);					
					//Usart2_SendData("success\r\n",9);
					return oil_LperH;
					//break;
				}
			}
			else
			{
				delay_Nx100ms(10);//等待1S 
				if(i == 4)
				{
					Usart2_SendData("OBD Error...\r\n",14);
					
					//break;
					return 0xFE;
				}
			}
		}
  }
  else 
  { 
   return 0x00;
  }  
}

void oil_average_LperH(void)  //平均油耗计算程序
{
  static u8 oil_average_LperH_arry[50] ={0}; //50采样的存放数组，
  static u8 temp_oil_LperH_F =0; //采样次数计数
  u8 temp_LperH=0;  //临时存放采样值，判断正确后放到缓存数组中
  u8 i =0;
  u16 temp_oil_LperH_average =0;

  if(oil_sampling_time == 300) //3秒采一次，判断3s时间到了没
	{
		oil_sampling_time =0; //给采样时间清零
    temp_LperH = Read_oil_LperH(); //读取采样值
		if((temp_LperH ==0xFF)||(temp_LperH ==0xFE))
		{
			//油耗读取不正确
		}
		else
			oil_average_LperH_arry[temp_oil_LperH_F++]=temp_LperH; //把采样得到的值放到缓存数组里面
		
		if(temp_oil_LperH_F > oil_LperH_F)  //完成一个采样周期，一个采样周期,使用大于号，
		{			
			oil_LperH_average = 0;
			
			for(i=0;i<oil_LperH_F;i++)  //i是从0开始的，所以到oil_LperH_F-1就行了
			   temp_oil_LperH_average = temp_oil_LperH_average+oil_average_LperH_arry[i]; //求和
			
			oil_LperH_average = temp_oil_LperH_average/oil_LperH_F; //求平均
			Flag_all.oil_LperH_average_Flag = 1;//采样完成
			//oil_LperH_average = temp_oil_LperH_average/temp_oil_LperH_F; //求平均
			temp_oil_LperH_F =0;
		}
  }
}

//读取车身状态信息
//刹车踩下主动上报：55 AA 00 0A C4 00 00 00 00 00 08 00 00 D6
//读取状态下发指令：55 AA 00 02 44 46
void OBD_Read_Body_state(void)
{
	u8 i =0;
//读取车身状态信息
//55 AA 00 02 44 46
//0  1  2  3  4  5	
	//Usart2_SendData("Read Body State...\r\n",20);
	for(i=0;i<3;i++)
	{
		if(0x02==OBD_Send_Commnd((u8 *)Read_Body_state_arry,Read_Body_state_len,3))//读取睡眠信息
		  return ;
		else
		{
			delay_Nx100ms(5);//等待1S 
			if(i == 2)
			{
				//Usart2_SendData("OBD Error...\r\n",14);
				break;
				//return 0;
			}					
		}
	}
}


//读OBDII PID数据流函数
//参数：
//p_PID:PID存放地址
//len_PID:读取PID长度
void Read_PID(u8 *p_in,u8 len_in)
{
  u8 OBDComm_arry[38]={0x00};
  u8 temp_arry[5]={0x55,0xAA,0x00,0x22,0x42};
  u8 *p_PID=p_in;
  u8 len_PID = len_in;
  u8 i=0;
  while(len_PID)
	{
    if(len_PID>32)
		{
      len_PID-=32;
			memcpy(OBDComm_arry,temp_arry,0x05); //0x55,0xAA,0x00,0x22,0x42
			memcpy(OBDComm_arry+5,p_PID,32);
			OBDComm_arry[37] =0x00; //将校验位清零，方便下面计算
			for(i=0;i<35;i++) //计算校验位
			  OBDComm_arry[37]+=OBDComm_arry[2+i];
			//if(4==OBD_Send_Commnd((u8 *)OBDComm_arry,32,5))
			OBD_Send_Commnd((u8 *)OBDComm_arry,32+6,10); //读PID
			p_PID+=32;
    }
		else
		{			
      memcpy(OBDComm_arry,temp_arry,0x05);
			OBDComm_arry[3]=len_PID+2;
			memcpy(OBDComm_arry+5,p_PID,len_PID);
			OBDComm_arry[len_PID+5] =0x00; //把校验位所在的位置清零，方便下面的计算
			for(i=0;i<len_PID+3;i++)
			  OBDComm_arry[len_PID+5]+=OBDComm_arry[2+i];
			OBD_Send_Commnd((u8 *)OBDComm_arry,len_PID+6,10); //读PID
			len_PID = 0;
			OBD_Com_Flag.OBD_PID_complete =1;//PID数据流接收完成
    }
  }  
}





