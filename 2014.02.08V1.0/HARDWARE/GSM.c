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
const u8 IOMODE[15]={"AT^IOMODE=1,1\r\n"};  //1,�Խ������ݽ���ת�������Զ���hex���Ʒ�������ʱ; 1����ʹ�ý��ջ���
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
u8 PID_user_defined[103]={0x00};  //PID�û��Զ���
u8 PID_user_defined_number =0 ;//PID�û��Զ�����Ŀ��
u8 PID_default_arry[9] ={0x05,0x0c,0x0d,0x0f,0x1f,0x33,0x46,0x5a,0x5c};

u8 	GSM_command[140] ={0};//GSM����������
u8 	GSM_Reciev[1100] ={0};//GSM���յ����Է�����������
u16  GSM_Reciev_len = 0;//GSM���յ����Է����������ݵĳ���
u8  GSM_Ring_Number[16]={0};//������ʾ����
u8  GSM_Ring_Number_len =0; //����������ʾ�ĺ��볤��
u32 Body_state =0; //����״̬λ���ĸ��ֽ�
u32 alarm_state =0; //������־���ĸ��ֽ�
//u32 mileage_km=0; //��̱���������ֳ��Ϳ��ܲ�֧��
//u16 oil_mass=0; //ʣ���������������ʣ���������������ֽڱ�ʾ������0x8000Ϊ%��ʾ��С��0x8000ΪL��ʾ
u8 oil_LperH_average=0; //L/H,������ƽ��ֵ,���ֵ�� 0xDF.
u8 oil_LperH=0; //L/H,��ǰ����ֵ,���ֵ�� 0xDF
u16 LocationUptime =0; //�ϴ�λ����Ϣ��ʱ����
u16 ACCOFF_LocationUptime = 0;//ACC �ص�״̬��λ����Ϣ�ϱ�ʱ����
u32 upheartime =0; //�����ϴ����
u16 upheart_timeout =0;//����Ӧ��ʱ
u8 oil_LperH_F=0; //�ϴ������ÿСʱ�ͺĵĲ�������

u16 upheartime_count=0; //���������ʱ��
u16 upheart_timeout_count =0;  //����Ӧ��ʱ��ʱ����Ĭ����Ϊ5min

u16 LocationUptime_count =0; //λ����Ϣ�ϴ���ʱ����
 
u16 JinJI_timeout =0; //��������ʱ����
u16 JinJI_count =0; //�������� ��ʱ��
u16 JinJi_LianXucount = 0; //�������½�������ʱ,�������

u8 ms100; //100ms��ʱ�������幷�����ŵ�while���档
void delay_Nx100ms(u8 n) //100ms��ʱ�������幷�����ŵ�while���档
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

u8 GSM_startup(void) //GSM��������,����ATָ����֤�����Ƿ�ɹ�  �ɹ�:����1;   ʧ��:����0;
{
  Usart2_SendData("Power on...\r\n",13);
	power_on;
	delay_Nx100ms(5);//GSM�ϵ����ʱ0.5s
	TERM_ON = 1;//
  delay_Nx100ms(15);//GSM�ϵ��TERM_ON������1.5s,������
	TERM_ON = 0;  
	delay_Nx100ms(30); //GSM��������ʱ3s����ģ���ʼ��ʱ��
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
u8 GSM_shutdown(void) //GSM�ػ�����,�ɹ�:����1;   ʧ��:����0;
{
  Usart2_SendData("Power off...\r\n",14);
	TERM_ON = 1;	
  delay_Nx100ms(15);//GSM��TERM_ON������1.5s,�ػ�
	TERM_ON = 0;  
	delay_Nx100ms(2);
	if(GSM_Send_ATCommnd((u8 *)AT,4,2)) //�ж�ATָ����û�з���OK
	{
		Usart2_SendData("Power off Failed...\r\n",21);		
		return 0;
	}
	else
	{
    Usart2_SendData("Power off success...\r\n",22);
		power_off;//�ػ��ϵ�
		delay_Nx100ms(75);//�ϵ����ʱ7.5S MG323�µ����ʱ��Ϊ6S
		return 1;	
  }	
}

u8 GSM_Send_ATCommnd( u8 *p,u8 length,u8 delay_count) //ָ��ɹ�����1; ���ɹ�����0
{ //GSM ATָ����Ӻ��������Ͳ��ɹ��Ļ�������λ���
	//*p:����������ĵ�ַ; length:����ָ��ĳ���; delay_count:����ָ����ж�OK�ĵȴ�ʱ��,��λ100ms
  u8 *pAT_commnd = p;
	u8 send_length = length;
	u8 delay = delay_count;
	u8 i = 0;	
	Flag_all.OK = 0;
	for(i=0;i<5;i++)
	{
     if(Flag_all.OK)  //��־λ�жϣ��Ժ�����ټ�һ���γɱ�Ŷ�Ӧ��ͬ��־λ��
		 {
       Flag_all.OK = 0;				
			 return 1;
     }
		 if(Flag_all.SISW) //������GPRS���淢������
		 {
       Flag_all.SISW = 0;	
       //if((usart3_Rx_backup[11]==GSM_command[10])&&(usart3_Rx_backup[12]==GSM_command[11])&&(usart3_Rx_backup[13]==GSM_command[12]))			 
			  return SISW_OK;
     }
/*		 
		 if(Flag_all.SISR) //�յ�GPRSͨ�������ݣ�ͨ���Ƿ������·��Ĳ���Э�����Ϣ����
		 {
       Flag_all.SISR = 0;	
       //if((usart3_Rx_backup[11]==GSM_command[10])&&(usart3_Rx_backup[12]==GSM_command[11])&&(usart3_Rx_backup[13]==GSM_command[12]))			 
			  return SISR_7E_OK;
     }*/
		 memset(usart3_Rx_backup,0x00,1100);//����ATָ��֮ǰ���ȰѴ���3�Ľ��ջ��汸����ա�
		 memset(usart3_Rx_buffer,0x00,1100);//����ATָ��֮ǰ���ȰѴ���3�Ľ��ջ�����ա�
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
			delay_Nx100ms(70);//�ϵ���ӳ�7S��MG323�ϵ����ʱ����Ҫ6S��      		
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
	
	if(!GSM_Send_ATCommnd((u8 *)CLIP,CLIP_Len,3))//AT+CLIP =1 ����������ʾ
	{
    if(!GSM_Send_ATCommnd((u8 *)CLIP,CLIP_Len,3))
			return 0;
  }
	if(!GSM_Send_ATCommnd((u8 *)CMIC,CMIC_Len,3))//AT+CMIC =10 ����MIC����
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
	//delay_Nx100ms(150);//��ʱ15s�ȴ�GSMģ��ע������
//0D 0A 2B 43 52 45 47 3A 20 30 2C 31 0D 0A 0D 0A 4F 4B 0D 0A	
//\r \n +  C  R  E  G  :     0  ,  1	\r \n \r \n O  K  \r \n
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19	
//	if(GSM_Send_ATCommnd((u8 *)CREG,CREG_Len,2))//AT+CREG? ���GSM�����Ƿ�ע��ɹ�
//	{
	Usart2_SendData("Checking GSM registration status...\r\n",38);
	for(i=0;i<10;i++)
	{
		if(GSM_Send_ATCommnd((u8 *)CREG,CREG_Len,2))//AT+CGREG? ���GPRS�����Ƿ�ע��ɹ�
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
					delay_Nx100ms(10);//�ȴ�1S 
					if(i == 99)
					{
						Usart2_SendData("GSM registered Faild, Restart GSM...\r\n",40);
						return 0;
					}					
				}
				else if(usart3_Rx_backup[11] == '2')
				{
					//Usart2_SendData("not registered,searching the operator...\r\n",42);
					delay_Nx100ms(10);//�ȴ�1S 
					if(i == 99)
					{
						Usart2_SendData("GSM registered Faild, Restart GSM...\r\n",40);
						return 0;
					}					
				}
				else if(usart3_Rx_backup[11] == '3')
				{
					//Usart2_SendData("registration is refused!!!\r\n",28);
					delay_Nx100ms(10);//�ȴ�1S 
					if(i == 99)
					{
						Usart2_SendData("GSM registered Faild, Restart GSM...\r\n",40);
						return 0;
					}					
				}
				else if(usart3_Rx_backup[11] == '4')
				{
					//Usart2_SendData("unknown state!!!\r\n",18);
					delay_Nx100ms(10);//�ȴ�1S 
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
			delay_Nx100ms(10);//�ȴ�1S 
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
//	if(GSM_Send_ATCommnd((u8 *)CGREG,CGREG_Len,2))//AT+CGREG? ���GPRS�����Ƿ�ע��ɹ�
//	{
/******AT+CGREG?****************************************/	
	Usart2_SendData("Checking GPRS registration status...\r\n",39);
	for(i=0;i<15;i++)
	{
		if(GSM_Send_ATCommnd((u8 *)CGREG,CGREG_Len,2))//AT+CGREG? ���GPRS�����Ƿ�ע��ɹ�
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
					delay_Nx100ms(10);//�ȴ�1S 
					if(i == 14)
					{
						Usart2_SendData("GPRS registered Faild, Restart GSM...\r\n",40);
						return 0;
					}		
				}
				else if(usart3_Rx_backup[12] == '2')
				{
					//Usart2_SendData("GPRS not registered,searching the operator...\r\n",47);
					delay_Nx100ms(10);//�ȴ�1S 
					if(i == 14)
					{
						//Usart2_SendData("GPRS registered Faild, Restart GSM...\r\n",40);
						return 0;
					}		
				}
				else if(usart3_Rx_backup[12] == '3')
				{
					//Usart2_SendData("GPRS registration is refused!!!\r\n",33);
					delay_Nx100ms(10);//�ȴ�1S 
					if(i == 14)
					{
						Usart2_SendData("GPRS registered Faild, Restart GSM...\r\n",40);
						return 0;
					}		
				}
				else if(usart3_Rx_backup[12] == '4')
				{
					//Usart2_SendData("GPRS unknown state!!!\r\n",23);
					delay_Nx100ms(10);//�ȴ�1S 
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
			delay_Nx100ms(10);//�ȴ�1S 
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

u8 sm_used=0; //GSM���յ��Ķ�����Ŀ
u8 sm_total =0;//gsm�洢���ɽ��յ��ܶ�����Ŀ
u8 MG301_SM_Init(void)
{
	u8 i = 0;
	
	Usart2_SendData((u8 *)"Init short message...\r\n",sizeof("Init short message...\r\n"));
//	for(i= 0;i<100;i++)
	//{
	if(!GSM_Send_ATCommnd((u8 *)GM301_CMGF,GM301_CMGF_Len,3))//GM301_CMGF ���ö���ϢģʽΪTEXT
		if(!GSM_Send_ATCommnd((u8 *)GM301_CMGF,GM301_CMGF_Len,3))//AT+COPS? ���������Ӫ��
			return 0;	
	//if(GSM_Send_ATCommnd((u8 *)GM301_CPMS,GM301_CPMS_Len,3))//GM301_CMGF ���ö���ϢģʽΪTEXT
	//  sm_used = GM301_CPMS_deal(); //��ȡ���Ż���״̬�������յ��Ķ�����Ŀ
	  SM_Deal(); //���Ŵ���
	//	Read_SM(14);  //
  //}
#if 0 
  if(!GSM_Send_ATCommnd((u8 *)CMGD,CMGD_Len,3))//AT+CMGD=1,4 ɾ����ѡ�����ȫ������
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
//����˵����
//p:
u8 GM301_CPMS_deal(void)  //AT+CPMS?  ��ѯ���Ŵ洢����״̬���Ƿ��ж���
{
  u8 *pStart;
	u8 *pEnd;
	u8 char_len = 0;	
	u8 i = 0;
	u8 temp_sm_used = 0;
	Flag_all.GM301_CPMS = 1;
	IWDG_Feed();
	if(GSM_Send_ATCommnd((u8 *)GM301_CPMS,GM301_CPMS_Len,3))//GM301_CMGF ���ö���ϢģʽΪTEXT
	{
    pStart = (u8 *)memchr((const char*)usart3_Rx_SM,',',usart3_Rx_SM_len);
		pEnd   = (u8 *)memchr((const char*)(pStart+1),',',usart3_Rx_SM_len);
		char_len = pEnd-pStart-1;
		temp_sm_used = 0;
		for(i=0;i<char_len;i++) //�����������
		{
      temp_sm_used = temp_sm_used*10+((*(pStart+1+i))-0x30);
    }
  }
	memset(usart3_Rx_SM,0x00,usart3_Rx_SM_len);
	usart3_Rx_SM_len =0;
	return temp_sm_used; //���ض�������
}

//���Ŷ�ȡ������AT+CMGR=sm_num
//char *p,��ȡ���ŵĵ�ַ��ֱ�����ַ�����ʾ��ʡȥת��
//������sm_num:���Ŵ洢λ���ַ�����
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
		if(GSM_Send_ATCommnd((u8 *)GSM_command,sizeof("AT+CMGR=")+2,5))//GM301_CMGF ���ö���ϢģʽΪTEXT
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
		if(GSM_Send_ATCommnd((u8 *)GSM_command,sizeof("AT+CMGR=")+1+2,5))//GM301_CMGF ���ö���ϢģʽΪTEXT
		{
			return ;
		}
  }
}
//���Ŵ����ܺ���������������������ȡ��ÿ�����ŵĴ�������ȫ��ɾ��
void SM_Deal()
{
	u8 i = 0;
  sm_used = GM301_CPMS_deal(); //��ȡ�ж�����������Ҫ����
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

//�������ݴ���������������ĳ���������ݵĺ�����Ʃ��: "SOBDPIDA*0#"
void SM_Data_Deal(void)
{
	u8 i=0;
	u8 j=0;
	u8 *p;
	u8 temp_pid[26] = {0x00};
  //OBD PID����������
  if(NULL==strstr((const char*)(usart3_Rx_SM), "#")) return ; //��ֹû�н�β��'#'

	if(NULL!=strstr((const char*)(usart3_Rx_SM), "SOBDPID*")) //�Զ�������
	{
   //SOBDPID*C0*C0*00*00*00*00*00*00*00*00*00*00*00#
    p=strstr((const char*)(usart3_Rx_SM), "SOBDPID*");
		for(i=0;p[8+i]!='#';i++)  //ȥ��*
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
					PID_user_defined[PID_user_defined_number++]=POD_OBD_PID_Tabl[i][j];  //PID�û��Զ���
      }
    }
		SPI_Flash_SafeWrite((u8 *)(&PID_user_defined_number),PID_addr,0x01);
		SPI_Flash_SafeWrite(PID_user_defined,PID_addr+1,PID_user_defined_number);
    memset(usart3_Rx_SM,0x00,usart3_Rx_SM_len);
	  usart3_Rx_SM_len =0;
		//SPI_Flash_SafeRead((u8 *)(&(PID_user_defined_number)),PID_addr,0x01);
		//SPI_Flash_SafeRead(PID_user_defined,PID_addr+1,PID_user_defined_number);
  }
	if(NULL!=strstr((const char*)(usart3_Rx_SM), "SOBDPIDA*0#")) //ȫ�أ�Ĭ��
	{
	//	SOBDPIDA*0#
    memset(PID_user_defined,0x00,103);
	  PID_user_defined_number =0;
		SPI_Flash_SafeWrite((u8 *)(&PID_user_defined_number),PID_addr,0x01);
		SPI_Flash_SafeWrite(PID_user_defined,PID_addr+1,103); //��PID�洢λ��ȫ������
		memset(usart3_Rx_SM,0x00,usart3_Rx_SM_len);
	  usart3_Rx_SM_len =0;
  }
	if(NULL!=strstr((const char*)(usart3_Rx_SM), "SOBDPIDA*1#")) //ȫ��
	{
		//SOBDPIDA*1#   
		PID_user_defined_number = 103;
    memcpy(PID_user_defined,POD_OBD_PID_Tabl,103);		
		SPI_Flash_SafeWrite((u8 *)(&PID_user_defined_number),PID_addr,0x01);
		SPI_Flash_SafeWrite(PID_user_defined,PID_addr+1,PID_user_defined_number);
    memset(usart3_Rx_SM,0x00,usart3_Rx_SM_len);
	  usart3_Rx_SM_len =0;
  }
	if(NULL!=strstr((const char*)(usart3_Rx_SM), "SOBDPIDA*2#")) //�Ƽ�����������
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

//����ɾ��������ɾ��������ȫ������
void Delete_AllSM(void)
{
  if(!GSM_Send_ATCommnd((u8 *)CMGD,CMGD_Len,10))//AT+CMGD=1,4 ɾ����ѡ�����ȫ������,���űȽ϶�ʱ��Ҫ����ʱ��ϳ���������1s�ĵȴ�ʱ��
	{
    if(!GSM_Send_ATCommnd((u8 *)CMGD,CMGD_Len,10))
			return ;
  }
}

void GSM_Module_Start(void)
{
	u8 i = 0;
/*****�ϵ�********************************/		
	while(1)
	{
    while(1)  //GSMģ�鿪������
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
      GPIO_SetBits(GPIOC,GPIO_Pin_8);  //GSM����
			break;
    }
			
		else
		{
      Usart2_SendData("GSM Init Faild, Restart1...\r\n",29);
			delay_Nx100ms(20);
			if((GSM_INIT())&&(MG301_SM_Init())&&(GPRS_Connect()))
			{
        GPIO_SetBits(GPIOC,GPIO_Pin_8);  //GSM����
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
		if(GSM_Send_ATCommnd((u8 *)COPS,COPS_Len,2))//AT+COPS? ���������Ӫ��
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
				delay_Nx100ms(10);//�ȴ�1S 
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
      delay_Nx100ms(10);//�ȴ�1S 
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
    if(GSM_Send_ATCommnd((u8 *)CSQ,CSQ_Len,2))//AT+CSQ? ����ź�ǿ��
	  { 
      Usart2_SendData("The Signal is:",14);
			Usart2_SendData(usart3_Rx_backup+8,5);
			Usart2_SendData("\r\n",2);
      break;			
    }
		else
		{
      delay_Nx100ms(10);//�ȴ�1S 
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
		if(GSM_Send_ATCommnd((u8 *)IOMODE,IOMODE_Len,2))//AT+CSQ? ����ź�ǿ��
	  { 
      Usart2_SendData("OK\r\n",4);		
      break;			
    }
		else
		{
      delay_Nx100ms(10);//�ȴ�1S 
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
    if(GSM_Send_ATCommnd((u8 *)SICS_con,SICS_con_Len,2))//AT^SICS? ���������Ӫ��
	  { 
      Usart2_SendData("conType : GPRS0\r\n",18);			
      break;			
    }
		else
		{
      delay_Nx100ms(10);//�ȴ�1S 
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
      delay_Nx100ms(10);//�ȴ�1S 
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
		if(GSM_Send_ATCommnd((u8 *)SISS_conId,SISS_conId_Len,2))//AT^SISS=0,conId,0���������Ӫ��
	  { 
      Usart2_SendData("Setting 0 conId success\r\n",25);			
      break;			
    }
		else
		{
      delay_Nx100ms(10);//�ȴ�1S 
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
		if(GSM_Send_ATCommnd((u8 *)SISS_srv,SISS_srv_Len,2))//AT^SISS=0,conId,0���������Ӫ��
	  { 
      Usart2_SendData("Set srvType is socket success\r\n",31);			
      break;			
    }
		else
		{
      delay_Nx100ms(10);//�ȴ�1S 
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
		if(GSM_Send_ATCommnd((u8 *)SISS_srv,SISS_srv_Len,2))//AT^SISS=0,conId,0���������Ӫ��
	  { 
      Usart2_SendData("Set srvType is socket success\r\n",31);			
      break;			
    }
		else
		{
      delay_Nx100ms(10);//�ȴ�1S 
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
      delay_Nx100ms(10);//�ȴ�1S 
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
			Usart2_SendData(GSM_command+29,bytetemp);//��IP������
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
			Usart2_SendData(GSM_command+30+bytetemp_i,bytetemp);//��port������
		  Usart2_SendData("\r\n",2);
    }
		if(GSM_Send_ATCommnd((u8 *)GSM_command,bytetemp_i+bytetemp+32,2))//AT+COPS? ���������Ӫ��
	  { 
      Usart2_SendData("Set server IP and port success...\r\n",35); //APN
      break;			
    }
		else
		{
      delay_Nx100ms(10);//�ȴ�1S 
			if(i >= 19)
			{
				Usart2_SendData("APN Erorr, Reset GSM...\r\n",25);
				return 0;
			}
    }
  }
//////////����·
	Usart2_SendData("Open the 0 srvProfileld...\r\n",28); //APN
	for(i= 0;i<6;i++)
	{   
		if(GSM_Send_ATCommnd((u8 *)SISO,SISO_Len,30))//AT^SISS=0,conId,0���������Ӫ��
	  { 
      Usart2_SendData("Open the 0 srvProfileld success\r\n",33);
			Usart2_SendData("GSM molel power on DONE\r\n",25);
      break;			
    }
		else
		{
      delay_Nx100ms(10);//�ȴ�1S 
			if(i >= 5)
			{
				Usart2_SendData("Open the 0 srvProfileld erorr, Reset GSM...\r\n",45);
				
				return 0;
			}
    }		
  }	
 return 1;
}

//u8 GSM_Send_Data(u8 *p,u8 sendlen,u8 *pid,u8 idlen)//*p:Ҫ���͵������������ݣ� *pid:��ӡ������2�����ݣ����涼�Ƕ�Ӧ�����ݳ���
u8 GSM_Send_Data(u8 *p,u8 sendlen)//*p:Ҫ���͵�������������
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
		if(GSM_Send_ATCommnd((u8 *)Sendp,sendlen,3))//��������
		  return 1;		
  }
  return 0;	
}

u8 temp_GSM_Reciev[1024] ={0}; //��ʱ���ת��������
u16 temp_len =0;  //ת�������ݳ���
u8 GSM_Recivdata_deal(void)
{//ֱ�Ӵ���GSM_Reciev[]�����е����ݣ��������е������ǽ�����ȡ��õ��Ĵ�ָ�����ݡ�
     
	//zhuanyi	
	
  
  u8 check_number =0; //������յ�����Ϣ��֤��
  u16 temp_mesg_ID =0; //��ȡ��ϢID
  u16 temp_mese_Len =0; //��Ϣ��ĳ��ȣ�����Ϣ����������ȡ��
  u16 temp_respmesg_ID =0;
  u8 temp_liushui_arry[2]={0};//��ʱ���ƽ̨��������Ϣ��ˮ��
  u16 response_liushui = 0u;
  u8 i=0;
  u8 call_back =0;

  temp_len = Bubiao_Reci_7E_Analysis(GSM_Reciev,temp_GSM_Reciev,GSM_Reciev_len);//���÷�ת�庯��������ת������ݵĳ���  
  memset(GSM_Reciev,0x00,GSM_Reciev_len); //���洦����ת��󣬼������GSM_Reciev[]����
  GSM_Reciev_len = 0;
  Flag_all.haveGSMdata = SISR_7E_UN; //����յ�7E���ݱ�־��GSM_Reciev[]������Կ�ʼ��һ�ֽ���

  if(temp_len > 1024)  return 0;//��ֹҪ����Ĳ�����������Χ��ֻ��Ϊ����ǿ����Ľ�׳��
	
  check_number = add_check_number(temp_GSM_Reciev,temp_len);		//����У����
  if(check_number ==temp_GSM_Reciev[temp_len-2]) //�Ƚ�У���룬���У���������ȷ�����
	{
//7E 81 00 00 09 01 83 56 95 78 28 0D 2A 00 02 00 42 42 49 54 51 4F BF 7E
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 
    temp_mesg_ID = (temp_GSM_Reciev[1]<<8)|temp_GSM_Reciev[2]; //��ȡID��IDλ��temp_GSM_Reciev�ĵ�1�͵�2��Ԫ��
		//temp_mese_Len =(temp_GSM_Reciev[3]&0x03)?((temp_GSM_Reciev[3]&0x03)*256+temp_GSM_Reciev[4]):temp_GSM_Reciev[4];  //��ȡ��Ϣ��ĳ��ȣ�����Ϣ�������С�
		temp_mese_Len =(temp_GSM_Reciev[3]&0x03)*256+temp_GSM_Reciev[4];  //��ȡ��Ϣ��ĳ��ȣ�����Ϣ�������С�
		temp_liushui_arry[0] = temp_GSM_Reciev[11];
		temp_liushui_arry[1] = temp_GSM_Reciev[12]; //��ȡƽ̨��������Ϣ��ˮ��
		if(temp_GSM_Reciev[3]&(0x01<<5)) //�ж���Ϣ��û�зְ�
			Flag_all.Subpackage = SUB;
		else
			Flag_all.Subpackage = UNSUB;
		for(i=0;i<6;i++)//�Ƚϱ�����
		{
			if(ben_ji_hao[i]!= temp_GSM_Reciev[5+i])
				return 0 ; //�����Ų��ԣ�ֱ�ӷ��ء�
    }
/**�����Ƕ���Ϣͷ����ȡ����������Ϣͷ����Ϣ�峤�ȣ���Ϣ�Ƿ�ְ����������Ƿ���ȷ,************/
		if(Flag_all.Subpackage == UNSUB) //�����Ϣû�зְ�������˷�֧
		{
      if(temp_mesg_ID == 0x8100)//�ն�ע��Ӧ��
			{
				//��ȡ��Ȩ��
				JianQuanMa_Len = temp_mese_Len-3;//�����Ȩ��ĳ���
				memcpy(JianQuanMa,(temp_GSM_Reciev+16),JianQuanMa_Len);
				return (temp_GSM_Reciev[15]+5);   //Ϊ�˺�TRUE=1,FALSE=0��ͻ�������������5������������л��ټ�ȥ����л�ԭ������ע������0���ɹ���1�������ѱ�ע�᣻2�����ݿ����޸ó�����
                                      //3���ն��ѱ�ע�᣻4�����ݿ����޸��ն�				
      }
			if(temp_mesg_ID == 0x8001) //ƽ̨ͨ��Ӧ��
			{
        //PPP��:[7E 80 01 00 05 01 32 05 14 48 12 10 3D 00 02 01 02 00 D0 7E ]-11:47:51
				//       0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19
				temp_respmesg_ID = (temp_GSM_Reciev[15]<<8)|temp_GSM_Reciev[16]; //��ȡӦ��ID��IDλ��temp_GSM_Reciev�ĵ�15�͵�16���ֽ�
				response_liushui = (temp_GSM_Reciev[13]<<8)|temp_GSM_Reciev[14]; //��ȡ8001ͨ��Ӧ���Ӧ����ˮ��
				if(temp_respmesg_ID==0x0102)//���յ��ն˼�Ȩ��ƽ̨ͨ��Ӧ��
				{
          if(temp_GSM_Reciev[17]==0x00)//��Ȩ�ɹ� ƽ̨ͨ��Ӧ�𷵻ؽ����0,�ɹ�/ʧ�ܣ� 1:ʧ�ܣ� 2,��Ϣ���� 3:��֧�֣� 4:��������ȷ��
					{
            Flag_all.Terminal_JianQuan_Flag = TRUE;
						return TRUE;
          }
					else//�ն˼�Ȩʧ��
					{
            Flag_all.Terminal_JianQuan_Flag = FALSE;
						return FALSE;
          }
        }
				/*******��Ȩ END*************************/
					/*******����Ӧ�� START*************************/
				if(temp_respmesg_ID==0x0002)//���յ�������ƽ̨ͨ��Ӧ��
				{
          if(temp_GSM_Reciev[17]==0x00)//ƽ̨ͨ��Ӧ�𷵻ؽ����0,�ɹ�/ʧ�ܣ� 1:ʧ�ܣ� 2,��Ϣ���� 3:��֧�֣� 4:��������ȷ��
					{
            upheart_timeout_count = 0;  //������ʱ��ʱ�����㡣
						Flag_all.SendHeart_Flag = 0;  //�����ϴ���־���㣬��ʾ�յ�Ӧ��
						return TRUE;
          }
					else//�ն˼�Ȩʧ��
					{
            Flag_all.Terminal_JianQuan_Flag = FALSE;
						return FALSE;
          }
        }
			/*******����Ӧ�� END*************************/
			/********λ����ϢӦ�� START**********************/
				if(temp_respmesg_ID==0x0200)//���յ�λ����Ϣ��ƽ̨ͨ��Ӧ��
				{
          if(temp_GSM_Reciev[17]==0x00)//λ����Ϣ�ϴ��ɹ� ƽ̨ͨ��Ӧ�𷵻ؽ����0,�ɹ�/ʧ�ܣ� 1:ʧ�ܣ� 2,��Ϣ���� 3:��֧�֣� 4:��������ȷ��
					{
            if(Flag_all.JinJi_Flag)  //���,�н���������־���ж��ǲ��ǽ���������Ӧ��
						{
              if(response_liushui == JinJi_liushui)//ͨ���Ա���ˮ�ţ��ж��ǲ��ǽ���������Ϣ��Ӧ��
							{
                Flag_all.JinJi_Flag = 0; //������������־���㣬
								alarm_state =alarm_state&(~((u32)0x01)); //�����������ı���λ����
								JinJi_Num =0;//�����������������㣬�´α���ʱ�ִ�0��ʼ��ʱ
								return TRUE;
              }
            }
						if(Flag_all.Shake_Flag)
						{
              if(response_liushui == Shake_liushui)//ͨ���Ա���ˮ�ţ��ж��ǲ��ǽ���������Ϣ��Ӧ��
							{
                Flag_all.Shake_Flag = 0; //������������־���㣬
								alarm_state =alarm_state&(~(((u32)0x01)<<16)); //�����������ı���λ����
								Shake_sendNum =0;//�𶯱����������㣬�´α���ʱ�ִ�0��ʼ��ʱ
								return TRUE;
              }
            }
						//Flag_all.SendHeart_Flag =1;  //�����ϴ���־���㣬��ʾ�յ�Ӧ��
						return TRUE;
          }
					else//�ն˼�Ȩʧ��
					{
            Flag_all.Terminal_JianQuan_Flag = FALSE;
						return FALSE;
          }
        }
		  /********λ����ϢӦ�� END*************************/
				
      }
//����λ����Ϣ:7E 82 01 00 00 01 78 10 00 00 00 00 02 E8 7E		
      if(temp_mesg_ID == 0x8201)	//����λ����Ϣ
      {  
        response_upload(temp_liushui_arry); //����λ����Ϣ��ѯӦ����
      }	
//�궨����7E 0F 0A 00 03 01 78 10 00 00 00 00 02 55 04 00 E8 7E	
//        0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
      if(temp_mesg_ID == 0x0F0A)	//�궨����
      {  
        if(1==Mark_Car(temp_GSM_Reciev[14],temp_GSM_Reciev[15])) //���ó��ͱ궨������
				{
          memcpy(TongYongResp.pliu_shui,&temp_GSM_Reciev[11],2) ;//��ȡ��ˮ��
					memcpy(TongYongResp.pmessage_ID,&temp_GSM_Reciev[1],2) ;//��ȡ��ˮ��
					TongYongResp.type = 0x00; //��ӽ��
          SPI_Flash_SafeWrite(temp_GSM_Reciev+14,CAR_TYPE_ADDR,0x02);	
        }
				else
				{
          memcpy(TongYongResp.pliu_shui,&temp_GSM_Reciev[11],2) ;//��ȡ��ˮ��
					memcpy(TongYongResp.pmessage_ID,&temp_GSM_Reciev[1],2) ;//��ȡ��ˮ��
					TongYongResp.type = 0x01; //��ӽ��
        }
      }
			
//���յ������ն˲�������0x8103	
//PPP��:[7E 81 03 00 3C 01 78 20 00 00 09 00 1A 07 00 00 00 57 04 00 00 00 00 00 00 00 58 04 00 00 00 00 00 00 00 59 04 00 00 00 00 00 00 00 5A 04 00 00 00 00 00 00 00 5B 02 00 00 00 00 00 5C 02 00 00 00 00 00 80 04 00 00 00 00 7C 7E ]-15:06:14
//       0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17			
			if(temp_mesg_ID == 0x8103) //�����ն˲�������ϢID��: 0x8103
			{ //ʹ��ͨ��Ӧ��
        Parameter_Set(temp_GSM_Reciev+13, 0);
				
				memcpy(TongYongResp.pliu_shui,&temp_GSM_Reciev[11],2) ;//��ȡ��ˮ��
				memcpy(TongYongResp.pmessage_ID,&temp_GSM_Reciev[1],2) ;//��ȡ��ˮ��
				TongYongResp.type = 0x00; //��ӽ��
				Terminal_TongYong_response(TongYongResp) ; //ʹ��ͨ��Ӧ�𣬽���Ӧ��
				
				SystemReset();
      }
 //������ƽ̨�·����룬�ն�������
 //PPP?:[7E 84 00 00 0C 01 78 10 00 00 00 00 07 01 31 38 33 35 36 39 35 37 38 32 38 D7 7E ]-15:10:21
 //      0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17	
			if(temp_mesg_ID == 0x8400)
			{
				if(temp_GSM_Reciev[13] ==0x01) //��������    0:��ͨͨ���� 1:����
				{ 
          Flag_all.SendHeart_Flag = 0; //��ʼ����绰֮ǰ�����������ϴ���־���㣬���Ƿ�ֹ����������ʱ��û���ļ��յ�����Ӧ���˳�ͨ��ʱ������ɲ���Ҫ�ĸ�λ					
          for(i= 0;i<6;i++) //�����ѭ��������в��ŵĶ���
					{ 
            call_back = GSM_CALL(temp_GSM_Reciev+14, temp_mese_Len-1);   //���ò���绰����						
						if(call_back == CEND_NORMAL)  //���н�ͨ�������Ҷϣ�
						{ 
							Flag_all.Check_Net_Flag = 1;  //�����źţ���������ȥ����ն˸�������������״̬
							Check_Net();
							break;			
						}
						else
						{
							delay_Nx100ms(10);//�ȴ�1S 
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
			SPI_Flash_SafeRead(Phone_number,PhoneNumber1_addr,2);//��ȡ�洢����1�ĳ���
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
						Usart3_SendData("ATA\r\0",5);//���ͽ�������
						Flag_all.jieting = 1;
						Flag_all.guaji = 0;	
						return;
          }
        }
      }					
			memset(Phone_number,0x00,16);
			Phone_number_Len = 0;
			SPI_Flash_SafeRead(Phone_number,PhoneNumber2_addr,2);//��ȡ�洢����1�ĳ���
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
						Usart3_SendData("ATA\r\0",5);//���ͽ�������
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
//���ز�����1�����յ�7E���ݣ���Ҫ����
//          0��û�н��յ�7E����
u8 Read_GSM(void) //��ȡGSM���ݺ���������ʱ����2.5s�����յ�GSM���ȡ��־
{
	u8 i =0;
	u8 temp_flag =0;
  for(i= 0;i<2;i++) //��������д���Ļ�����ֻҪ�����飬��Ȼ��ʱ̫�����³����Ч�ʱ�͡�
	{  
		temp_flag = GSM_Read_AT_SISR((u8 *)SISR,SISR_Len,5);//��GSM���ջ��棬�����ؽ��յ������ݱ�־
		if(SISR_7E_OK == temp_flag)//�յ���7E������Ҫ����
	  { 
      //Usart2_SendData("Setting 0 conId success\r\n",25);	
      //GSM_Recivdata_deal();			
      return 1;			
      //break;			
    }
		else if(SISR_7E_EMPTY == temp_flag)  //��������û�����ݣ�����Ҫ����
			return 0;			
		else
		{
      delay_Nx100ms(3);//�ȴ�300ms
			if(i >= 1)
			{
				Usart2_SendData("read error...\r\n",15);
				return 0;
			}
    }		
  }
	return 0;
}

u8 GSM_Read_AT_SISR( u8 *p,u8 length,u8 delay_count) //ָ��ɹ�����1; ���ɹ�����0
{ //GSM ATָ����Ӻ��������Ͳ��ɹ��Ļ�������λ���
	//*p:����������ĵ�ַ; length:����ָ��ĳ���; delay_count:����ָ����ж�OK�ĵȴ�ʱ��,��λ100ms
  u8 *pAT_commnd = p;
	u8 send_length = length;
	u8 delay = delay_count;
	u8 i = 0;	
	Flag_all.OK = 0;
	for(i=0;i<5;i++)
	{
		 if(Flag_all.haveGSMdata) //�յ�GPRSͨ�������ݣ�ͨ���Ƿ������·��Ĳ���Э�����Ϣ����
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
		 
//		 memset(usart3_Rx_backup,0x00,140);//����ATָ��֮ǰ���ȰѴ���3�Ľ��ջ��汸����ա�
		 memset(usart3_Rx_buffer,0x00,1100);//����ATָ��֮ǰ���ȰѴ���3�Ľ��ջ�����ա�
		 usart3_Rx_length =0 ;
		 Flag_all.haveGSMdata = SISR_7E_UN;
		 Usart3_SendData(pAT_commnd,send_length);
		// while(!Flag_all.haveGSMdata)
		 delay_Nx100ms(delay);
  }
  return SISR_7E_UN;
}

//�绰�ز�����
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
u8 CALL_Timer =0 ; //��绰�������õ��Ķ�ʱ��
u16 CEND_time = 0; //��ȡCEND����������ͨ��ʱ�䣬�����0˵��ͨ��û�б���ͨ
u8 GSM_CALL(u8 * p_number, u8 number_len)
{
	u8 call_arry[18]= {0};
  CEND_time = 0;//��ͨ��ʱ�����㣬�ڴ��ڳ����д���
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
  Flag_all.CALL_Flag =1; //�����Ų�����־����1�������ڴ����м����Ҫ������
  Usart3_SendData(call_arry,6+number_len);
  CALL_Timer = 0;
	while(1)//(!Flag_all.CEND_Flag)&&(!Flag_all.NO_CARRIER))
	{
    IWDG_Feed();
		if(CALL_Timer >10) //����10s��û��ORIG��CONF��������˴κ��п����쳣
		{
      if((!Flag_all.ORIG_Flag)&&(!Flag_all.CONF_Flag))
      {
        return CALL_ERROR;
      }			
    }
		if(!Flag_all.ORIG_Flag)
		{
      
    }		
		
		if(!Flag_all.CONF_Flag) //���յ�������
		{  //��ʱ��û�нӵ�����������Ϊ�д������ʱ�䶨��Ϊ10s(���˲²��ֵ���д�ȷ��)
			
      
    }
		
		if(!Flag_all.CONN_Flag)//�绰��û�н�ͨ
		{
      
    }
		
		if(Flag_all.CEND_Flag)
		{
      if(CEND_time >1)  //�绰��ͨ�����������˳�ͨ��״̬��ת��GPRS״̬
			{
				Flag_all.CALL_Flag = 0; //���ж�����־����
        return CEND_NORMAL; //ͨ���������������ú����յ�������ȥ�ж�GPRS����״̬��

      }
			else
			{
        if(!Flag_all.CONF_Flag) //û���յ���������˵�������쳣
				{
          Flag_all.CALL_Flag = 0; //���ж�����־����
					return CEND_UNUSUAL; //ͨ��ʱ��Ϊ0��û�н�ͨ�ͱ��Ҷ��ˣ����ʱ�򣬵��ó�����ܻ����½��в���
        }
        else //�绰�������У�����û�˽���
        {
          Flag_all.CALL_Flag = 0; //���ж�����־����
          return CEND_NORMAL; //ͨ���������������ú����յ�������ȥ�ж�GPRS����״̬��
        }					
      }				
    }
    
    if(Flag_all.NO_CARRIER)
		{

    }	
		
	  if(Flag_all.NO_ANSWER ) //���أ�NO ANSWER ���˽�����������ָ��Ժ�ɸ�����Ҫ���
    {

    }
  }
	IWDG_Feed();
}

u8 Check_Net_timeover = 0; //�����⣬��ʱ��λ
void Check_Net()
{
	static cnt = 0;
  if(Flag_all.SendHeart_Flag)   //&&(!Flag_all.Check_Net_Send))
	{
    if(Check_Net_timeover >10) //10S��ʱ��
		{
      cnt++;
			Check_Net_timeover = 0; //10s��ʱ������
			Flag_all.SendHeart_Flag =1;
			Flag_all.Check_Net_Send = 1;
			upheart_timeout_count = 0; //��������ʱ5min����ʱ�����㣬��Ϊͨ���Ĺ����иü�ʱ�����ڹ���
			Send_Heart(); //��������
    }
  }
	if(!Flag_all.SendHeart_Flag) //������־Ϊ0�������ֿ���
	{
		if(Flag_all.Check_Net_Send)
		{
      Flag_all.Check_Net_Send = 0; //��Check_Net()�����з�����������־
			Flag_all.Check_Net_Flag = 0; //�������״̬��־
    }
	  else
		{
      Flag_all.SendHeart_Flag =1;
			Flag_all.Check_Net_Send = 1;
			Send_Heart(); //��������
    }    
  }
	if(cnt>3)
	{
    Usart2_SendData((u8 *)"Net Bad, Reset MCU...\r\n",sizeof("Net Bad, Reset MCU...\r\n"));
		SystemReset();
  }
}


































