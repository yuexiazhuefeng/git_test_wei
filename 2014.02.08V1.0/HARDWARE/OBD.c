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

u8 OBD_mileage_buffer_arry[4]={0}; //��̴洢���飬��ȡ��������ĸ�char���͵����ݣ�ֱ��ת�Ƶ����������
u8 OBD_oil_mass_buffer_arry[2]={0};//ʣ�������������飬��ȡ��ʣ������ֵ��2��char���͵����ݣ�ֱ��ת�Ʒŵ����������

u8 OBD_init_timer = 0;
u8 * add =0;

u8 OBD_PID_Buffer[OBD_BUF_LEN] = {0x00};
RING_T ring_obd_buffer;

u8 OBD_Send_Commnd( u8 *p,u8 length,u8 delay_count) //ָ��ɹ�����1; ���ɹ�����0
{ //GSM ATָ����Ӻ��������Ͳ��ɹ��Ļ�������λ���
	//*p:����������ĵ�ַ; length:����ָ��ĳ���; delay_count:����ָ����ж�OK�ĵȴ�ʱ��,��λ100ms
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
     if(OBD_Com_Flag.OBD_Received_Complete) //���OBD�����ݽ���
		 {
       OBD_Com_Flag.OBD_Received_Complete = 0;
			 return 1;
     }
		 if(OBD_Com_Flag.OBD_Received_Body)//���յ�����״̬��Ϣ�ǵĴ�����
		 {
       OBD_Com_Flag.OBD_Received_Body = 0;	
      	//diaoyongcheshenzhuangtaichuli();
			  return 2;
     }
		 if(OBD_Com_Flag.OBD_Error)//���յ�����״̬��Ϣ�ǵĴ�����
		 {
       //OBD_Com_Flag.OBD_Received_Body = 0;	
      	//diaoyongcheshenzhuangtaichuli();
			  return 3;
     }
		 if(OBD_Com_Flag.OBD_Received_PID)  //���յ�PID������
		 { 
       return 4 ;
     }
		 memset(usart1_Rx_backup,0x00,50);//����ATָ��֮ǰ���ȰѴ���3�Ľ��ջ��汸����ա�
		 memset(usart1_Rx_buffer,0x00,50);//����ATָ��֮ǰ���ȰѴ���3�Ľ��ջ�����ա�
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
void OBD_init(void) //OBD��ʼ��������Ҫ��
{
  u8 i = 0;
	u8 j=0;
	u8 temp[17]={0};
  ring_obd_buffer.size = OBD_BUF_LEN;
  ring_obd_buffer.data = OBD_PID_Buffer;
  RING_INIT(&ring_obd_buffer,ring_obd_buffer.size);
//��ȡ�汾��Ϣ55 AA 00 02 40 42
//55 AA 00 08 C0 50 04 00 01 01 00 1E 
//0  1	2  3  4  5  6  7  8  9  10 11
	Usart2_SendData("Read OBD Mode Versionsi...\r\n",28);
	for(i=0;i<5;i++)
	{
		add = (u8 *)OBD_Versionsi_arry;
		if(OBD_Send_Commnd((u8 *)OBD_Versionsi_arry,OBD_Versionsi_len,3))//��ȡ��������
		{
			if(OBD_Com_Flag.OBD_Error)
			{
				OBD_Error_Judge();      //���ô����жϺ���
				OBD_Com_Flag.OBD_Error = 0;
				break;
			}
			if(usart1_Rx_backup[4]==0xC0)//�յ�����ʱ��
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
			delay_Nx100ms(10);//�ȴ�1S 
			if(i == 4)
			{
				Usart2_SendData("OBD Error...\r\n",14);
				break;
				//return 0;
			}					
		}
	}
//��ȡ��������55 AA 00 02 33 35
//55 AA 00 06 B3 20 13 08 30 24
//0  1  2  3  4  5  6  7  8  9	
	Usart2_SendData("Read OBD Mode Compile Data...\r\n",31);
	for(i=0;i<5;i++)
	{			
		if(OBD_Send_Commnd((u8 *)OBD_compile_data_arry,BD_compile_data_len,3))//��ȡ��������
		{
			if(OBD_Com_Flag.OBD_Error)
			{
				OBD_Error_Judge();      //���ô����жϺ���
				OBD_Com_Flag.OBD_Error = 0;
				break;
			}
			else if(usart1_Rx_backup[4]==0xB3)//�յ�����ʱ��
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
			delay_Nx100ms(10);//�ȴ�1S 
			if(i == 4)
			{
				Usart2_SendData("OBD Error...\r\n",14);
				break;
				//return 0;
			}					
		}
	}

//��ȡ������VIN��55 AA 00 02 49 4B
//55 AA 00 13 C9 31 32 33 34 35 36 37 38 39 41 42 43 44 45 46 47 48 DD
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22
	Usart2_SendData("Read VIN...\r\n",13);
	for(i=0;i<5;i++)
	{
		if(OBD_Send_Commnd((u8 *)OBD_Read_VIN_arry,OBD_Read_VIN_len,3))//��ȡ��������
		{
			if(OBD_Com_Flag.OBD_Error)
			{
				OBD_Error_Judge();      //���ô����жϺ���
				OBD_Com_Flag.OBD_Error = 0;
				break;
			}
			if(usart1_Rx_backup[4]==0xC9)//�յ�����ʱ��
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
			delay_Nx100ms(10);//�ȴ�1S 
			if(i == 4)
			{
				Usart2_SendData("OBD Error...\r\n",14);
				break;
				//return 0;
			}					
		}
	}
//����Ʒ���к�SN:55 AA 00 02 50 52
//55 AA 00 0E D0 05 D5 FF 30 37 30 4D 41 43 21 46 48 CE 
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 
	Usart2_SendData("Read SN...\r\n",12);
	for(i=0;i<5;i++)
	{
		if(OBD_Send_Commnd((u8 *)OBD_Read_SN_arry,OBD_Read_SN_len,3))//��ȡ��������
		{
			if(OBD_Com_Flag.OBD_Error)
			{
				OBD_Error_Judge();      //���ô����жϺ���
				OBD_Com_Flag.OBD_Error = 0;
				break;
			}
			if(usart1_Rx_backup[4]==0xD0)//�յ�����ʱ��
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
			delay_Nx100ms(10);//�ȴ�1S 
			if(i == 4)
			{
				Usart2_SendData("OBD Error...\r\n",14);
				break;
				//return 0;
			}					
		}
	}
//��ȡ˯��ģʽ��˯��ʱ��55 AA 00 02 25 27
//55 AA 00 04 A5 01 3C E6 
//0  1  2  3  4  5  6  7
	Usart2_SendData("Read Sleep Mode...\r\n",20);
	for(i=0;i<5;i++)
	{
		if(OBD_Send_Commnd((u8 *)Read_Sleep_Parameter_arry,Read_Sleep_Parameter_len,3))//��ȡ˯����Ϣ
		{
			if(OBD_Com_Flag.OBD_Error)
			{
				OBD_Error_Judge();      //���ô����жϺ���
				OBD_Com_Flag.OBD_Error = 0;
				break;
			}
			if(usart1_Rx_backup[4]==0xA5)//�յ�����ʱ��
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
			delay_Nx100ms(10);//�ȴ�1S 
			if(i == 4)
			{
				Usart2_SendData("OBD Error...\r\n",14);
				break;
				//return 0;
			}					
		}
	}
//��ȡ����״̬��Ϣ
//55 AA 00 02 44 46
//0  1  2  3  4  5	
	Usart2_SendData("Read Body State...\r\n",20);
	for(i=0;i<5;i++)
	{
		if(0x02==OBD_Send_Commnd((u8 *)Read_Body_state_arry,Read_Body_state_len,3))//��ȡ˯����Ϣ
		  break ;
		else
		{
			delay_Nx100ms(10);//�ȴ�1S 
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

void OBD_Error_Judge(void)  //����OBD����7F������Ϣ���ж���������
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

void OBD_Error_Judge_zi(void)  //����OBD����7F������Ϣ���ж���������00,01,02,03
{
  switch (usart1_Rx_backup[6])
	{
    case 0x00: Usart2_SendData("OBD Error Type : 00\r\n",sizeof("OBD Error Type : 00\r\n")); OBD_Com_Flag.OBD_error_type =0x00; break;
		case 0x01: Usart2_SendData("OBD Error Type : 01\r\n",sizeof("OBD Error Type : 00\r\n")); OBD_Com_Flag.OBD_error_type =0x01; break;
		case 0x02: Usart2_SendData("OBD Error Type : 02\r\n",sizeof("OBD Error Type : 00\r\n")); OBD_Com_Flag.OBD_error_type =0x02; break;
		case 0x03: Usart2_SendData("OBD Error Type : 03\r\n",sizeof("OBD Error Type : 00\r\n")); OBD_Com_Flag.OBD_error_type =0x03; break;					
  }
}


//�궨���ͺ�����������high,���ʹ���ĸ��ֽ�; low,���ʹ���ĵ��ֽ�
u8 Mark_Car(u8 high, u8 low)
{
  u8 i =0;
	u8 temp[8] ={0x55,0xAA,0x00,0x04,0x4B};
  temp[5]=high;
  temp[6]=low;
  temp[7]=temp[2]+temp[3]+temp[4]+temp[5]+temp[6];
//�궨����55 AA 00 04 4B 04 00 53 (�����궨Ϊ����ĳ��ͣ�)
//55 AA 00 04 CB 04 00 D3 
//0  1  2  3  4  5  6  7 
	Usart2_SendData("Mark The Car...\r\n",17);
	for(i=0;i<5;i++)
	{
		if(OBD_Send_Commnd((u8 *)temp,8,3))//��ȡ��������
		{
			if(OBD_Com_Flag.OBD_Error)
			{
				OBD_Error_Judge();      //���ô����жϺ���
				OBD_Com_Flag.OBD_Error = 0;
				//break;
				return 0;
			}
			if(usart1_Rx_backup[4]==0xCB)//�յ�����ʱ��
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
			delay_Nx100ms(10);//�ȴ�1S 
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

u8 Body_Control(u8 comm, u8 parameter)  //������ƺ�����
{
  u8 i =0;
	u8 temp[8] ={0x55,0xAA,0x00,0x04,0x23};
  temp[5]=comm;
  temp[6]=parameter;
  temp[7]=temp[2]+temp[3]+temp[4]+temp[5]+temp[6];
//�������55 AA 00 04 23 03 01 2B (��������)
//        0  1  2  3  4  5  6  7
//����55 AA 00 03 A3 00 A6 
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
		if(OBD_Send_Commnd((u8 *)temp,8,3))//��ȡ��������
		{
			if(OBD_Com_Flag.OBD_Error)
			{
				OBD_Error_Judge();      //���ô����жϺ���
				OBD_Com_Flag.OBD_Error = 0;
				//break;
				return 0;
			}
			if(usart1_Rx_backup[4]==0xA3)//�յ�����ʱ��
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
			delay_Nx100ms(10);//�ȴ�1S 
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

//��������,
//type:0x00,����˯��; 0x01,��ȡ˯��״̬;
//com: 0x00,����˯�߹���; 0x01,����˯�߹���
//parameter: ˯��ʱ�䣬Ĭ��60s����ϵͳ��������������ߺ����������ʱ����绽�Ѻ�60s��˯�ߣ�ʱ������޸�Ϊ10--255s
u8 OBD_Sleep(u8 type,u8 com,u8 parameter)
{
  u8 i =0;
	u8 typei =type;
	u8 comi = com;
	u8 parameteri = parameter;
	u8 temp[8] ={0};
//���ͣ�55 AA 00 02 25 27
//���أ�55 AA 00 04 A5 01 3C E6 
//      0  1  2  3  4  5  6  7
	if(typei==0x01) //��ȡ˯��״̬ 0x25����
	{
    Usart2_SendData("Read Sleep Mode...\r\n",20);
		for(i=0;i<5;i++)
		{
			if(OBD_Send_Commnd((u8 *)Read_Sleep_Parameter_arry,Read_Sleep_Parameter_len,3))//��ȡ˯����Ϣ
			{
				if(OBD_Com_Flag.OBD_Error)
				{
					OBD_Error_Judge();      //���ô����жϺ���
					OBD_Com_Flag.OBD_Error = 0;
					break;
				}
				if(usart1_Rx_backup[4]==0xA5)//�����յ���ȡʱ������
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
				delay_Nx100ms(10);//�ȴ�1S 
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
//���͹ر�˯�ߣ�55 AA 00 04 24 00 00 28
//�ر�˯�߷��أ�55 AA 00 02 A4 A6 	(�ɹ�)
//              0  1  2  3  4  5  6  7
	else if(typei==0x00) //����˯��ʱ��
	{    
		if(comi ==0x00)
		{
      Usart2_SendData("Closing Sleep Mode...\r\n",23);
			for(i=0;i<5;i++)
			{
				if(OBD_Send_Commnd((u8 *)Close_Sleep_Parameter_arry,Close_Sleep_Parameter_len,3))//��ȡ˯����Ϣ
				{
					if(OBD_Com_Flag.OBD_Error)
					{
						OBD_Error_Judge();      //���ô����жϺ���
						OBD_Com_Flag.OBD_Error = 0;
						break;
					}
					if(usart1_Rx_backup[4]==0xA4)//�յ��ر�˯��ָ��ķ���
					{						
						Usart2_SendData("success\r\n",9);					
						break;
					}
				}		
				else
				{
					delay_Nx100ms(10);//�ȴ�1S 
					if(i == 4)
					{
						Usart2_SendData("OBD Error...\r\n",14);
						break;
						//return 0;
					}					
				}
			}
    }
		if(comi ==0x01)  //����˯��ʱ��
		{
//���ͣ�55 AA 00 04 24 01 60 89	(��˯��,������˯��ʱ����60s)	
//      55 AA 00 02 A4 A6 (�ɹ�����)
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
				if(OBD_Send_Commnd((u8 *)temp,8,3))//����˯��ʱ��
				{
					if(OBD_Com_Flag.OBD_Error)
					{
						OBD_Error_Judge();      //���ô����жϺ���
						OBD_Com_Flag.OBD_Error = 0;
						break;
					}
					if(usart1_Rx_backup[4]==0xA4)//�յ�����ʱ��ɹ�����
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
					delay_Nx100ms(10);//�ȴ�1S 
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

//usart1_Rx_backup���յ����ϱ���55 AA 00 0A C4 00 01 00 00 00 00 00 00 CF
//                              0  1  2  3  4  5  6  7  8  9  10 11 12 13
//                                             1  2  3  4  5  6
void Body_state_deal(void)
{
  if(usart1_Rx_backup[5]&0x03) //0000 0011ǰ���ǰ�ң���һ���ſ��˼�ǰ�ſ� 
	{
    Body_state =Body_state|(((u32)0x01)<<13);
		if((!OBD_Com_Flag.FDoor_State_Flag)&&(!OBD_Com_Flag.OBD_ReadBodyState_Flag)) //��״̬�����ı���״̬�ı��־Ϊ0
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //��״̬��־��1
		OBD_Com_Flag.FDoor_State_Flag =1;
  }  
  else
	{
    Body_state =Body_state&(~(((u32)0x01)<<13)); 
		if((OBD_Com_Flag.FDoor_State_Flag)&&(!OBD_Com_Flag.OBD_ReadBodyState_Flag)) //��״̬�����ı���״̬�ı��־Ϊ0
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //��״̬��־��1
		OBD_Com_Flag.FDoor_State_Flag =0;
  }
	   
  if(usart1_Rx_backup[5]&(0x03<<2))//0000 1100���һ������һ���ſ��˼����ſ�
	{
    Body_state =Body_state|(((u32)0x01)<<14);
		if((!OBD_Com_Flag.MDoor_State_Flag)&&(!OBD_Com_Flag.OBD_ReadBodyState_Flag)) //��״̬�����ı���״̬�ı��־Ϊ0
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //��״̬��־��1
		OBD_Com_Flag.MDoor_State_Flag =1;
  }		  
  else
	{
    Body_state =Body_state&(~(((u32)0x01)<<14)); 
		if((OBD_Com_Flag.MDoor_State_Flag)&&(!OBD_Com_Flag.OBD_ReadBodyState_Flag)) //��״̬�����ı���״̬�ı��־Ϊ0
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //��״̬��־��1
		OBD_Com_Flag.MDoor_State_Flag =0;
  }
	    
	if(usart1_Rx_backup[5]&(((u32)0x01)<<4))//0001 0000β�俪
	{
    Body_state =Body_state|(((u32)0x01)<<15);
		if((!OBD_Com_Flag.BDoor_State_Flag)&&(!OBD_Com_Flag.OBD_ReadBodyState_Flag)) //��״̬�����ı���״̬�ı��־Ϊ0
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //��״̬��־��1
		OBD_Com_Flag.BDoor_State_Flag =1;
  }		  
  else
	{
    Body_state =Body_state&(~(((u32)0x01)<<15)); 	
		if((OBD_Com_Flag.BDoor_State_Flag)&&(!OBD_Com_Flag.OBD_ReadBodyState_Flag)) //��״̬�����ı���״̬�ı��־Ϊ0
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //��״̬��־��1
		OBD_Com_Flag.BDoor_State_Flag =0;		  
  }
	    
	if(usart1_Rx_backup[6])//ACC��
	{
    Body_state =Body_state|((u32)0x01);
		if(!OBD_Com_Flag.ACC_state)  //���֮ǰACCΪ��
		{
      OBD_Com_Flag.OBD_ACC_Turn = L_TO_H;  //2
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //��״̬�ı��־����1
    }		  
		OBD_Com_Flag.ACC_state =1;//ACC��
  }		
	else
	{
    Body_state =Body_state&(0xFFFFFFFE);
		if(OBD_Com_Flag.ACC_state)  //���֮ǰACCΪ��
		{
      OBD_Com_Flag.OBD_ACC_Turn = H_TO_L;  //1
			OBD_Com_Flag.OBD_BodyState_Turn_Flag =1; //��״̬�ı��־����1
    }		  
		OBD_Com_Flag.ACC_state =0;//ACC��
		OBD_Com_Flag.OBD_Mileage_Receivded_FLag = 0; //����̶�ȡ��ɱ�־����
  }		
	if((usart1_Rx_backup[7]==0x01)||(!(usart1_Rx_backup[10]&0x10)))//��ң���������������䣬���ÿ���ACC״̬��ң�ؿ�����Կ�׿������������
	{
    OBD_Com_Flag.alarm = UNLOCK;//�Ѿ����־���㣬�����䡣
		Body_state = Body_state&(~(((u32)0x01)<<31)); //����31λ������״̬λ���㣬��ʾ�˳�����
  }
//55 AA 00 0A C4 00 00 00 00 00 10 00 00 DE 
//0  1  2  3  4  5  6  7  8  9  10 11 12 13	
  else if(((usart1_Rx_backup[7]==0x02)&&(!(usart1_Rx_backup[6])))||(usart1_Rx_backup[10]&0x10))//ң�����������ţ���ACC���ڹر�״̬�����뾯�䡣
	{
    OBD_Com_Flag.alarm = LOCK;       //�Ѿ����־��λ�����뾯�䡣
		Body_state = Body_state|(((u32)0x01)<<31); //������״̬λ��1����ʾ���뾯��
  }

/******������־λ�ж�***************************/
//��������
//1-�ſ��౨��	OBD_Com_Flag.door = 1;
//2-ɲ������
//3-ACC����
//
	if(OBD_Com_Flag.alarm == LOCK)  //��ȡ������־
	{//�ţ�ɲ����ACC.�����𶯣�����IO���������
		if(usart1_Rx_backup[5]&0x1F) //0001 1111ǰ�ź��ź��䣬��һ�����򿪡� 
		{ 
			alarm_state =alarm_state|(((u32)0x01)<<31);//�Ƿ����ű������ǵ�31λ
			if(!OBD_Com_Flag.door)
				OBD_Com_Flag.Have_alarm = 1; //���о����־��1
      OBD_Com_Flag.door = 1;  //���ſ�����
			
    }
		else
		{
      alarm_state =alarm_state&(~(((u32)0x01)<<31)); //û�зǷ�����
			if(OBD_Com_Flag.door)
				OBD_Com_Flag.Have_alarm = 1; //���о����־��1
			OBD_Com_Flag.door = 0;
    }	 
		
		if(usart1_Rx_backup[6])//ACC��
		{
      alarm_state =alarm_state|(((u32)0x01)<<27); 
			if(!OBD_Com_Flag.ACC_alarm)
				OBD_Com_Flag.Have_alarm = 1; //���о����־��1
			OBD_Com_Flag.ACC_alarm=1; //��ACC����			
    }		  
	  else
		{
      //alarm_state =(~(0x01<<27));
			alarm_state = alarm_state&(~(((u32)0x01)<<27));
			if(OBD_Com_Flag.ACC_alarm)
				OBD_Com_Flag.Have_alarm = 1; //���о����־��1
			OBD_Com_Flag.ACC_alarm=0;
    }
		if(usart1_Rx_backup[10]&(0x1<<0x03)) //0000 1000�ж�ɲ���Ƿ����
		{
      alarm_state =alarm_state|(((u32)0x01)<<15);//�Ƿ�ɲ��
			//alarm_state =alarm_state|(((u32)0x01)<<31);//�Ƿ����ű������ǵ�31λ
			if(!OBD_Com_Flag.brake)
				OBD_Com_Flag.Have_alarm = 1; //���о����־��1
			OBD_Com_Flag.brake =1;  //�в�ɲ������			
    }	    
    else
		{
      alarm_state =alarm_state&(~(((u32)0x01)<<15)); //û�зǷ�ɲ��    
		//	alarm_state =alarm_state&(~(((u32)0x01)<<31)); //û�зǷ�����
			if(OBD_Com_Flag.brake)
				OBD_Com_Flag.Have_alarm = 1; //���о����־��1
			OBD_Com_Flag.brake =0;
    }
 /*   if(OBD_Com_Flag.OBD_ReadBodyState_Flag)
		{
      
			OBD_Com_Flag.Have_alarm = 0; //Ϊ�˱���ÿ�ζ�״̬��ʱ��һֱ������λ�����±���Ƶ��
    }*/
  }
/*
	if(OBD_Com_Flag.OBD_ReadBodyState_Flag) //�����������ȡ��־���ⲽֻ��Ϊ���Է���һOBD_Com_Flag.Have_alarm ����λ
	{
    //OBD_Com_Flag.OBD_ReadBodyState_Flag = 0; //�����ｫ������ȡ��־����
		OBD_Com_Flag.Have_alarm = 0; //Ϊ�˱���ÿ�ζ�״̬��ʱ��һֱ������λ�����±���Ƶ��
  } */
/*	else  //û��������ȡ��־��˵����OBDģ�������ϱ��ģ���ʱ��Ϊ�г���״̬�����ı�
	{
    //OBD_Com_Flag.OBD_BodyState_Turn_Flag = 1;  //����״̬�����ı䣬���ı��־λ��1 ����λ���ϴ���λ����Ϣ��ᱻ��ӦΪ����
		OBD_Com_Flag.OBD_ReadBodyState_Flag = 0;
  }	*/

}
	

//��ȡ���,������̱�Ķ���
//����55 AA 00 03 43 8A D0
//����55 AA 00 07 C3 8A 00 08 05 E3 44 
//    0  1  2  3  4  5  6  7  8  9  10
void mileage(void)
{
  u8 i =0;
	
	if(OBD_Com_Flag.ACC_state) //���ACC�ǿ����Ż��������Ķ���
	{	
		//Usart2_SendData("Read mileage...\r\n",17);
		for(i=0;i<5;i++) //�����ȡʧ�ܵĻ������5��
		{
			if(OBD_Send_Commnd((u8 *)OBD_mileage_arry,OBD_mileage_len,3))//��ȡ�����Ϣ
			{
				if(OBD_Com_Flag.OBD_Error)
				{
					OBD_Error_Judge();      //���ô����жϺ���
					if(OBD_Com_Flag.OBD_error_type==1 ||OBD_Com_Flag.OBD_error_type==2)
					{
						OBD_Com_Flag.OBD_Mileage_Support_FLag = 1;
						memcpy(OBD_mileage_buffer_arry,0x00,4);//�����̶�ȡ����
					}					
					OBD_Com_Flag.OBD_Error = 0; //���յ�7FӦ��������㡣
					break;
				}
				if((usart1_Rx_backup[4]==0xC3)&&(usart1_Rx_backup[5]==0x8A))//�յ������Ϣ
				{						
					//mileage_km=0 ;//stm32���ô��ģʽ������λ���ڵ͵�ַ�У�RAM��ַ�����ʱ���Ǵӵ͵���һ�η���ģ������õ͵�ַ
					memcpy(OBD_mileage_buffer_arry,&usart1_Rx_backup[6],4);//�������е�������ݣ�ת�Ƶ���̽��ջ�����������
								 //��ģ��ȷ�ϣ�OBD_mileage_buffer_arry[0]��ŵ���������λ��OBD_mileage_buffer_arry[3]��ŵ���������λ8λ
					//mileage_km = mileage_km|usart1_Rx_backup[9];
					//mileage_km = mileage_km|(usart1_Rx_backup[8]<<8);
					//mileage_km = mileage_km|(usart1_Rx_backup[7]<<16);
					//mileage_km = mileage_km|(usart1_Rx_backup[6]<<24);
					OBD_Com_Flag.OBD_Mileage_Receivded_FLag =1;//��̶�ȡ��ɱ�־��1
					Usart2_SendData("success\r\n",9);					
					break;
				}
			}		
			else
			{
				delay_Nx100ms(3);//�ȴ�300ms
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

//����55 AA 00 03 43 8B D1
//����55 AA 00 07 C3 8B 00 00 00 87 DC 
//    0  1  2  3  4  5  6  7  8  9  10
void Read_oil_mass(void)
{
  u8 i =0;
  Usart2_SendData("Read oil mass...\r\n",18);
	if(OBD_Com_Flag.ACC_state)  //ACCΪ����״̬��ȥ��ȡ�ͱ��ʣ������
	{	
		for(i=0;i<5;i++)
		{
			if(OBD_Send_Commnd((u8 *)Read_oil_mass_arry,Read_oil_mass_len,3))//��ȡ˯����Ϣ
			{
				if(OBD_Com_Flag.OBD_Error)
				{
					OBD_Error_Judge();      //���ô����жϺ���
					if(OBD_Com_Flag.OBD_error_type==1 ||OBD_Com_Flag.OBD_error_type==2)
					{
						OBD_Com_Flag.OBD_oil_massSupport_FLag = 1; //��֧��ʣ��������ȡ����־��1
						memset(OBD_oil_mass_buffer_arry,0x00,2);//���ʣ��������ȡ����
					}					
					OBD_Com_Flag.OBD_Error = 0;
					break;
				}
				if((usart1_Rx_backup[4]==0xC3)&&(usart1_Rx_backup[5]==0x8B))//�յ�ʣ����������
				{
					
					memcpy(OBD_oil_mass_buffer_arry,&usart1_Rx_backup[8],2); //����ȡ����ʣ������ֵ��ֱ��ת�Ƶ�ʣ������������������
						//��ģ��ȷ�ϣ�OBD_oil_mass_buffer_arry[0]��ŵ���������λ��OBD_oil_mass_buffer_arry[3]��ŵ���������λ8λ
					//oil_mass=0 ;//stm32���ô��ģʽ������λ���ڵ͵�ַ�У�RAM��ַ�����ʱ���Ǵӵ͵���һ�η���ģ������õ͵�ַ
					//oil_mass = oil_mass|usart1_Rx_backup[9];
					//oil_mass = oil_mass|(usart1_Rx_backup[8]<<8);
					OBD_Com_Flag.OBD_oil_massReceivded_FLag = 1;//ʣ��������ȡ��ɱ�־����ʱ���Է���ʣ����������
					Usart2_SendData("success\r\n",9); 
					break;
				}
			}
			else
			{
				delay_Nx100ms(10);//�ȴ�1S 
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

//����55 AA 00 03 43 88 CE
//����55 AA 00 07 C3 88 00 00 00 0C 5E 
//    0  1  2  3  4  5  6  7  8  9  10
u8 Read_oil_LperH(void) //��ȡL/H˲ʱ�ͺģ�����Ǹ���һ��֮�����ۼӺ����������ÿСʱ�ͺ�
{
  u8 i =0;
	if(OBD_Com_Flag.ACC_state) //ACC����״̬�²�ȥ���ͺģ���Ȼû������
	{
    //Usart2_SendData("Read oil LperH...\r\n",19);
		for(i=0;i<5;i++)
		{
			if(OBD_Send_Commnd((u8 *)Read_oil_LperH_arry,Read_oil_LperH_len,3))//��ȡ˯����Ϣ
			{
				if(OBD_Com_Flag.OBD_Error)
				{
					OBD_Error_Judge();           //���ô����жϺ���
					OBD_Com_Flag.OBD_Error = 0;  //
					return 0xFF;
					//break;
				}
				if((usart1_Rx_backup[4]==0xC3)&&(usart1_Rx_backup[5]==0x88))//�յ��ر�˯��ָ��ķ���
				{ 
					oil_LperH=0 ;//stm32���ô��ģʽ������λ���ڵ͵�ַ�У�RAM��ַ�����ʱ���Ǵӵ͵���һ�η���ģ������õ͵�ַ
					oil_LperH = oil_LperH|usart1_Rx_backup[9]; //L/H,������ƽ��ֵ,���ֵ�� 0xDF
					//oil_LperH = oil_LperH|(usart1_Rx_backup[8]<<8);					
					//Usart2_SendData("success\r\n",9);
					return oil_LperH;
					//break;
				}
			}
			else
			{
				delay_Nx100ms(10);//�ȴ�1S 
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

void oil_average_LperH(void)  //ƽ���ͺļ������
{
  static u8 oil_average_LperH_arry[50] ={0}; //50�����Ĵ�����飬
  static u8 temp_oil_LperH_F =0; //������������
  u8 temp_LperH=0;  //��ʱ��Ų���ֵ���ж���ȷ��ŵ�����������
  u8 i =0;
  u16 temp_oil_LperH_average =0;

  if(oil_sampling_time == 300) //3���һ�Σ��ж�3sʱ�䵽��û
	{
		oil_sampling_time =0; //������ʱ������
    temp_LperH = Read_oil_LperH(); //��ȡ����ֵ
		if((temp_LperH ==0xFF)||(temp_LperH ==0xFE))
		{
			//�ͺĶ�ȡ����ȷ
		}
		else
			oil_average_LperH_arry[temp_oil_LperH_F++]=temp_LperH; //�Ѳ����õ���ֵ�ŵ�������������
		
		if(temp_oil_LperH_F > oil_LperH_F)  //���һ���������ڣ�һ����������,ʹ�ô��ںţ�
		{			
			oil_LperH_average = 0;
			
			for(i=0;i<oil_LperH_F;i++)  //i�Ǵ�0��ʼ�ģ����Ե�oil_LperH_F-1������
			   temp_oil_LperH_average = temp_oil_LperH_average+oil_average_LperH_arry[i]; //���
			
			oil_LperH_average = temp_oil_LperH_average/oil_LperH_F; //��ƽ��
			Flag_all.oil_LperH_average_Flag = 1;//�������
			//oil_LperH_average = temp_oil_LperH_average/temp_oil_LperH_F; //��ƽ��
			temp_oil_LperH_F =0;
		}
  }
}

//��ȡ����״̬��Ϣ
//ɲ�����������ϱ���55 AA 00 0A C4 00 00 00 00 00 08 00 00 D6
//��ȡ״̬�·�ָ�55 AA 00 02 44 46
void OBD_Read_Body_state(void)
{
	u8 i =0;
//��ȡ����״̬��Ϣ
//55 AA 00 02 44 46
//0  1  2  3  4  5	
	//Usart2_SendData("Read Body State...\r\n",20);
	for(i=0;i<3;i++)
	{
		if(0x02==OBD_Send_Commnd((u8 *)Read_Body_state_arry,Read_Body_state_len,3))//��ȡ˯����Ϣ
		  return ;
		else
		{
			delay_Nx100ms(5);//�ȴ�1S 
			if(i == 2)
			{
				//Usart2_SendData("OBD Error...\r\n",14);
				break;
				//return 0;
			}					
		}
	}
}


//��OBDII PID����������
//������
//p_PID:PID��ŵ�ַ
//len_PID:��ȡPID����
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
			OBDComm_arry[37] =0x00; //��У��λ���㣬�����������
			for(i=0;i<35;i++) //����У��λ
			  OBDComm_arry[37]+=OBDComm_arry[2+i];
			//if(4==OBD_Send_Commnd((u8 *)OBDComm_arry,32,5))
			OBD_Send_Commnd((u8 *)OBDComm_arry,32+6,10); //��PID
			p_PID+=32;
    }
		else
		{			
      memcpy(OBDComm_arry,temp_arry,0x05);
			OBDComm_arry[3]=len_PID+2;
			memcpy(OBDComm_arry+5,p_PID,len_PID);
			OBDComm_arry[len_PID+5] =0x00; //��У��λ���ڵ�λ�����㣬��������ļ���
			for(i=0;i<len_PID+3;i++)
			  OBDComm_arry[len_PID+5]+=OBDComm_arry[2+i];
			OBD_Send_Commnd((u8 *)OBDComm_arry,len_PID+6,10); //��PID
			len_PID = 0;
			OBD_Com_Flag.OBD_PID_complete =1;//PID�������������
    }
  }  
}





