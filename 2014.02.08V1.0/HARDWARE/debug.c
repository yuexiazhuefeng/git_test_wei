/******************************************************************************************
*  File Name                             :prot.h
*  Copyright                             :2014.2 wei
*  Module Name                           :Init port
*
*  CPU                                   :STM32F103RC
*  RTOS                                  :NULL
*
*  Create Date                           :2014/7/1
*  Author                                :wei
*  Abstract Dscription                   :
*
*
*----------------------Revision History-----------------------------
*  No   Version   Date        Revised By          Item           Description
*
*
*******************************************************************************/
#include "debug.h"
u8 Version_arry[10]={"2014-08-28"};

u8 Mid[6]= {0};
u8 ReTimeout = 0;
u16 Debug_Timeout =0;//5min=30000��ʱ��������ģʽά��5���ӣ�5����֮���Զ��˳�



//�޸�IP�������Ż��û�������ע��

void Debug(void)
{
  Debug_waitcounter = 1002;//402��ʱ10S
	while(Debug_waitcounter >2)  //10s
  {
    IWDG_Feed();
    if(Flag_all.debughead)
    {
      Flag_all.debugmodel = 1;
        
      break;
    }
		
  }
	if(Flag_all.debugmodel)    //debug model
  {
    Usart2_SendData((u8 *)"in debug model!please press command\r\n",37);
		Debug_Timeout =0;
    while(Debug_Timeout<30000)//��������ģʽ����ʱ5min,����5����֮����Զ�����
    {
		  IWDG_Feed();
	    if((Flag_all.PC_Command) && (ReTimeout == 7)) //timeout control
	    {
	       D_data_deal();
	       Flag_all.PC_Command = 0;
	   	}
	   	  
	   }
//		SystemReset(); //5min��ʱ�Ż����е��˴�������һ�������λ
  } 
}

void D_data_deal(void)
{
  unsigned char i,j = 0;
  unsigned char len_tem;
  unsigned char dou_temp[5] = {0};
  unsigned char bytenumbertemp[2] = {0};
  unsigned char bytetemp= 0;
  unsigned char temp_BenJiHao[12]={0}; //�����յ��ı�����ת��12���ֽڴ�������Ȼ����ת��BCD[6]
/*IP****START***************************************/
  if(usart3_Rx_buffer ==(u8 *)strstr((const char*)(usart3_Rx_buffer), "FHTMAINIP"))//IP,���úͲ�ѯ
	{
    if(usart3_Rx_buffer[9]=='*') //IP����
		{
			bytetemp = 15;//IP:220.178.000.056���Ϸָ��õ�'.'������15���ֽڣ����ȹ̶���ÿ��ǰ�治��Ĳ�0
			//if((bytetemp > 0) && (bytetemp <= 9))
			//	bytenumbertemp[0] = (bytetemp + 0x30);
		//	else bytenumbertemp[0] = (bytetemp + 0x37);
			bytenumbertemp[0] = bytetemp ;
				SPI_Flash_SafeWrite(bytenumbertemp,IPaddress_addr,1);
				SPI_Flash_SafeWrite(usart3_Rx_buffer+10,IPaddress_addr+1,bytetemp);
			  Usart2_SendData(usart3_Rx_buffer,usart3_Rx_length);
				Usart2_SendData("OK\r\n",5);
			
			 SPI_Flash_SafeRead(bytenumbertemp,Terminal_auth_Flag_addr,0x01);
			 if(0x55 == bytenumbertemp[0])//֮ǰע��ɹ�,����Ҫ����洢��SPI-FLASH�е�ע��ɹ���־λ
			 {
				 bytenumbertemp[0]=0xFF;
				 SPI_Flash_SafeWrite(bytenumbertemp,Terminal_auth_Flag_addr,0x01);
			 }
			
    }
		else if(usart3_Rx_buffer[9]=='?') //��ѯIP
		{
      SPI_Flash_SafeRead(bytenumbertemp,IPaddress_addr,1);
			//if((bytenumbertemp[0] > '0') && (bytenumbertemp[0] <= '9'))
			//bytetemp = (bytenumbertemp[0] - 0x30);
			//else if((bytenumbertemp[0] >= 'A') && (bytenumbertemp[0] <= 'F'))
			 // bytetemp = (bytenumbertemp[0] - 0x37);
			bytetemp = bytenumbertemp[0];
			if(bytetemp >0x0F)//else 
			{
				Usart2_SendData("FHTMAINIP ERROR\r\n",sizeof("FHTMAINIP ERROR\r\n"));
			  memset(usart3_Rx_buffer,0x00,1100);
				usart3_Rx_length =0 ;
				return;
			}
			usart3_Rx_buffer[9]='*';
		  SPI_Flash_SafeRead(usart3_Rx_buffer+10,IPaddress_addr+1,bytetemp);
		  Usart2_SendData(usart3_Rx_buffer,bytetemp+10);
		  Usart2_SendData(" OK\r\n",5);   
    }
		else Usart2_SendData("COMMAND ERROR!",sizeof("COMMAND ERROR!"));//���յ��������		
  }
/*IP****END***************************************/	
	
/******PORT  start********************/	
	if(usart3_Rx_buffer ==(u8 *)strstr((const char*)(usart3_Rx_buffer), "FHTUDPPORT"))//�˿�PORT,���úͲ�ѯ,����ֻ��UDP
	{    
		if(usart3_Rx_buffer[10]=='*') //�˿�����
		{
			bytetemp = (usart3_Rx_length - 11);
			if((bytetemp == 0) || (bytetemp >= 15))
			{
				memset((u8 *)usart3_Rx_buffer,0x00,1100);
				usart3_Rx_length =0 ;
				Usart2_SendData("COMMAND ERROR!",sizeof("COMMAND ERROR!"));//���յ��������		
				return;
			}
			//bytenumbertemp[0] = (((bytetemp & 0xf0) >> 4)+0x30);  //����hex��ASCIIת�ķ�ʽֻ���ڱ���С��0x0F��ʱ����á�
			//bytenumbertemp[1] = ((bytetemp & 0x0f) + 0x30); 
			bytenumbertemp[0] = bytetemp;
			SPI_Flash_SafeWrite(bytenumbertemp,PortNumber_addr,1); //д���ݵĳ���
			SPI_Flash_SafeWrite(usart3_Rx_buffer+11,PortNumber_addr+2,bytetemp);
			Usart2_SendData(usart3_Rx_buffer,usart3_Rx_length);
			Usart2_SendData(" OK\r\n",5);		
    }
		else if(usart3_Rx_buffer[10]=='?') //�˿ڲ�ѯ
		{
			 SPI_Flash_SafeRead(bytenumbertemp,PortNumber_addr,1);	 
			 //bytenumbertemp[0] = (bytenumbertemp[0] - 0x30);
			// bytenumbertemp[1] = (bytenumbertemp[1] - 0x30);
			 bytetemp = bytenumbertemp[0]; //(bytenumbertemp[0] * 10 + bytenumbertemp[1]);
			 if((bytetemp == 0) || (bytetemp >= 15))
			 {
					memset(usart3_Rx_buffer,0x00,1100);
					usart3_Rx_length =0 ;
					Usart2_SendData("FHTUDPPORT ERROR\r\n",sizeof("FHTUDPPORT ERROR\r\n"));
					return; 
			 }
			 usart3_Rx_buffer[10]='*';
			 SPI_Flash_SafeRead(usart3_Rx_buffer+11,PortNumber_addr+2,bytetemp);
			 Usart2_SendData(usart3_Rx_buffer,bytetemp+11);
			 Usart2_SendData(" OK\r\n",5); 
		 }
		else Usart2_SendData("COMMAND ERROR!",sizeof("COMMAND ERROR!"));//���յ��������		
  }
/******PORT  end********************/			
/******APN  START********************/		
	if(usart3_Rx_buffer ==(u8 *)strstr((const char*)(usart3_Rx_buffer), "FHTSETMAINAPN"))//�˿�PORT,���úͲ�ѯ,����ֻ��UDP
	{    
		if(usart3_Rx_buffer[13]=='*') //�˿�APN����
		{
			bytetemp = (usart3_Rx_length - 14);
			if((bytetemp == 0) || (bytetemp >= 15))
			{
				memset(usart3_Rx_buffer,0x00,1100);
				usart3_Rx_length =0 ;
				Usart2_SendData("COMMAND ERROR!",sizeof("COMMAND ERROR!"));//���յ��������		
				return;
			}
			bytenumbertemp[0] = (((bytetemp & 0xf0) >> 4)+0x30);  //����hex��ASCIIת�ķ�ʽֻ���ڱ���С��0x0F��ʱ����á�
			bytenumbertemp[1] = ((bytetemp & 0x0f) + 0x30); 
			SPI_Flash_SafeWrite(bytenumbertemp,apn_addr,2);
			SPI_Flash_SafeWrite(usart3_Rx_buffer+14,apn_addr+2,bytetemp);							
			Usart2_SendData(usart3_Rx_buffer,usart3_Rx_length);
			Usart2_SendData(" OK\r\n",5);		
    }
		else if(usart3_Rx_buffer[13]=='?') //��ѯAPN
		{
			 SPI_Flash_SafeRead(bytenumbertemp,apn_addr,2);	 
			 bytenumbertemp[0] = (bytenumbertemp[0] - 0x30);
			 bytenumbertemp[1] = (bytenumbertemp[1] - 0x30);
			 bytetemp = (bytenumbertemp[0] * 10 + bytenumbertemp[1]);
			 if((bytetemp == 0) || (bytetemp >= 15))
			 {
					memset(usart3_Rx_buffer,0x00,1100);
					usart3_Rx_length =0 ;
					Usart2_SendData("FHTSETMAINAPN ERROR\r\n",sizeof("FHTSETMAINAPN ERROR\r\n"));
					return; 
			 }
			 usart3_Rx_buffer[13]='*';
			 SPI_Flash_SafeRead(usart3_Rx_buffer+14,apn_addr+2,bytetemp);
			// SPI_Flash_SafeRead(usart3_Rx_buffer+4,apn_addr+2,bytetemp);
			 Usart2_SendData(usart3_Rx_buffer,bytetemp+15);
			 Usart2_SendData(" OK\r\n",5); 
		 }
		else Usart2_SendData("COMMAND ERROR!",sizeof("COMMAND ERROR!"));//���յ��������		
  }	
/******APN  END********************/	
/******benjihao start********************/
	if(usart3_Rx_buffer ==(u8 *)strstr((const char*)(usart3_Rx_buffer), "FHTSETMID"))//������,���úͲ�ѯ
	{
    if(usart3_Rx_buffer[9]=='*') //����������
		{
			bytetemp = usart3_Rx_length - 10;//�ն��ֻ��ţ�BCD[6].
			if(bytetemp<12)//����12���ֽڣ���ǰ�油�����
			{
        for(i=0;i<(12-bytetemp);i++)
				  temp_BenJiHao[i]=0x00;
				memcpy(temp_BenJiHao+i,usart3_Rx_buffer+10,bytetemp);				
      }
			bytetemp =6;//��������̶�Ϊ12���������
			if((bytetemp > 0) && (bytetemp <= 9))
				bytenumbertemp[0] = (bytetemp + 0x30);
			else bytenumbertemp[0] = (bytetemp + 0x37);
				SPI_Flash_SafeWrite(bytenumbertemp,BenJiHao_addr,1);
			for(i=0,j=0;i<(bytetemp*2);i++,j++)
			{
        //usart3_Rx_buffer[10+j]= (usart3_Rx_buffer[10+(i++)]>0x39||usart3_Rx_buffer[10+(i--)]>0x39)?(0x55):((usart3_Rx_buffer[10+(i++)]<<4)|(usart3_Rx_buffer[10+i]&0x0f));
				temp_BenJiHao[j]= (temp_BenJiHao[(i++)]>0x39||temp_BenJiHao[(i--)]>0x39)?(0x55):((temp_BenJiHao[(i++)]<<4)|(temp_BenJiHao[i]&0x0f));
				if(temp_BenJiHao[(j-1)]==0x55)
				{
          Usart2_SendData("FHTSETMID ERROR\r\n",sizeof("FHTSETMID ERROR\r\n"));
			    memset(usart3_Rx_buffer,0x00,1100);
				  usart3_Rx_length =0 ;
				  return;
        }
      }				
				SPI_Flash_SafeWrite(temp_BenJiHao,BenJiHao_addr+1,bytetemp);
			  Usart2_SendData(usart3_Rx_buffer,usart3_Rx_length);
				Usart2_SendData(" OK\r\n",5);	
			
			SPI_Flash_SafeRead(bytenumbertemp,Terminal_auth_Flag_addr,0x01);
			 if(0x55 == bytenumbertemp[0])//֮ǰע��ɹ�,����Ҫ����洢��SPI-FLASH�е�ע��ɹ���־λ
			 {
				 bytenumbertemp[0]=0xFF;
				 SPI_Flash_SafeWrite(bytenumbertemp,Terminal_auth_Flag_addr,0x01);
			 }
    }
		else if(usart3_Rx_buffer[9]=='?') //��ѯ������
		{
      SPI_Flash_SafeRead(bytenumbertemp,BenJiHao_addr,1);
			if((bytenumbertemp[0] > '0') && (bytenumbertemp[0] <= '9'))
			bytetemp = (bytenumbertemp[0] - 0x30);
			else if((bytenumbertemp[0] >= 'A') && (bytenumbertemp[0] <= 'F'))
			  bytetemp = (bytenumbertemp[0] - 0x37);
			else 
			{
				Usart2_SendData("FHTSETMID ERROR\r\n",sizeof("FHTSETMID ERROR\r\n"));
			  memset(usart3_Rx_buffer,0x00,1100);
				usart3_Rx_length =0 ;
				return;
			}
			usart3_Rx_buffer[9]='*';
			SPI_Flash_SafeRead(usart3_Rx_buffer+23,BenJiHao_addr+1,bytetemp);
			for(i=0,j=0;i<bytetemp;i++,j++)
			{
        	
				usart3_Rx_buffer[10+(j++)]=(usart3_Rx_buffer[23+i]>>4)+0x30;
				usart3_Rx_buffer[10+j]=(usart3_Rx_buffer[23+i]&0x0f)+0x30;
				if((usart3_Rx_buffer[10+j]>0x39)||(usart3_Rx_buffer[10+(j-1)]>0x39))
				{
					Usart2_SendData("FHTSETMID ERROR\r\n",sizeof("FHTSETMID ERROR\r\n"));
					memset(usart3_Rx_buffer,0x00,1100);
					usart3_Rx_length =0 ;
					return;
			  } 
      }
		  
		  Usart2_SendData(usart3_Rx_buffer,(bytetemp*2)+10);
		  Usart2_SendData(" OK\r\n",5);   
    }
		else Usart2_SendData("COMMAND ERROR!",sizeof("COMMAND ERROR!"));//���յ��������		
  }
/******benjihao end********************/
/******RESET**start******************/	
	//FHTRESETHOST*
	if(usart3_Rx_buffer ==(u8 *)strstr((const char*)(usart3_Rx_buffer), "FHTRESETHOST*"))//��λ����
	{
    Usart2_SendData("FHTRESET OK\r\n",sizeof("FHTRESET OK\r\n"));
		//Usart2_SendData("\r\n",2);
		SystemReset()  ;//����һ�������λ

  }
	
/******RESET***end*****************/	
/******Check Version**start******************/		
	if(usart3_Rx_buffer ==(u8 *)strstr((const char*)(usart3_Rx_buffer), "FHTZDZT?"))//��λ����
	{
    Usart2_SendData(Version_arry,Version_Len);
		//Usart2_SendData("\r\n",2);
		//SystemReset()  ;//����һ�������λ

  }
	
	
/******Check Version***end*****************/		
	
/*	
  if(usart3_Rx_buffer[0] == '%')
  {
		if(usart3_Rx_buffer[3] == '?')  //check command
		{
			 if((usart3_Rx_buffer[1] == 'S') && (usart3_Rx_buffer[2] == 'R'))
			 {
				 //ReadEprom(IPaddress_addr,bytenumbertemp,1);
				 SPI_Flash_SafeRead(bytenumbertemp,IPaddress_addr,1);
				 if((bytenumbertemp[0] > '0') && (bytenumbertemp[0] <= '9'))
					bytetemp = (bytenumbertemp[0] - 0x30);
				 else if((bytenumbertemp[0] >= 'A') && (bytenumbertemp[0] <= 'F'))
					bytetemp = (bytenumbertemp[0] - 0x37);
				 else 
				 {
						 Usart2_SendData("EEpropm error,reset SR\r\n",24);
						 memset(usart3_Rx_buffer,0x00,140);
						 usart3_Rx_length =0 ;
						 return;
				 }
				 SPI_Flash_SafeRead(usart3_Rx_buffer+4,IPaddress_addr+1,bytetemp);
				 Usart2_SendData(usart3_Rx_buffer,bytetemp+4);
				 Usart2_SendData("\r\n",2);   
			 }
			 else if((usart3_Rx_buffer[1] == 'P') && (usart3_Rx_buffer[2] == 'T'))
			 {
				 SPI_Flash_SafeRead(bytenumbertemp,PortNumber_addr,2);	 
				 bytenumbertemp[0] = (bytenumbertemp[0] - 0x30);
				 bytenumbertemp[1] = (bytenumbertemp[1] - 0x30);
				 bytetemp = (bytenumbertemp[0] * 10 + bytenumbertemp[1]);
				 if((bytetemp == 0) || (bytetemp >= 15))
				 {
						memset(usart3_Rx_buffer,0x00,140);
						usart3_Rx_length =0 ;
						Usart2_SendData("EEpropm error,reset PT\r\n",24);
						return; 
				 }
				 SPI_Flash_SafeRead(usart3_Rx_buffer+4,PortNumber_addr+2,bytetemp);
				 Usart2_SendData(usart3_Rx_buffer,bytetemp+4);
				 Usart2_SendData("\r\n",2); 
			 }
			 else if((usart3_Rx_buffer[1] == 'A') && (usart3_Rx_buffer[2] == 'P'))
			 {					 
				 SPI_Flash_SafeRead(bytenumbertemp,apn_addr,2);	
				 bytenumbertemp[0] = (bytenumbertemp[0] - 0x30);
				 bytenumbertemp[1] = (bytenumbertemp[1] - 0x30);
				 bytetemp = (bytenumbertemp[0] * 10 + bytenumbertemp[1]);
				 if((bytetemp == 0) || (bytetemp >= 15))
				 {
						memset(usart3_Rx_buffer,0x00,140);
						usart3_Rx_length =0 ;
						Usart2_SendData("EEpropm error,reset AP\r\n",24);
						return; 
				 }
				 SPI_Flash_SafeRead(usart3_Rx_buffer+4,apn_addr+2,bytetemp);
				 Usart2_SendData(usart3_Rx_buffer,bytetemp+4);
				 Usart2_SendData("\r\n",2); 
			 }
			 else if((usart3_Rx_buffer[1] == 'U') && (usart3_Rx_buffer[2] == 'U'))
			 {
				 SPI_Flash_SafeRead(bytenumbertemp,user_addr,2);
				 bytenumbertemp[0] = (bytenumbertemp[0] - 0x30);
				 bytenumbertemp[1] = (bytenumbertemp[1] - 0x30);
				 bytetemp = (bytenumbertemp[0] * 10 + bytenumbertemp[1]);
				 if((bytetemp == 0) || (bytetemp >= 15))
				 {
						memset(usart3_Rx_buffer,0x00,140);
						usart3_Rx_length =0 ;
						Usart2_SendData("EEpropm error,reset UU\r\n",24);
						return; 
				 }
				 SPI_Flash_SafeRead(usart3_Rx_buffer+4,user_addr+2,bytetemp);
				 Usart2_SendData(usart3_Rx_buffer,bytetemp+4);
				 Usart2_SendData("\r\n",2); 
			 }
			 else if((usart3_Rx_buffer[1] == 'P') && (usart3_Rx_buffer[2] == 'P'))
			 {
				 SPI_Flash_SafeRead(bytenumbertemp,password_addr,2);
				 bytenumbertemp[0] = (bytenumbertemp[0] - 0x30);
				 bytenumbertemp[1] = (bytenumbertemp[1] - 0x30);
				 bytetemp = (bytenumbertemp[0] * 10 + bytenumbertemp[1]);
				 if((bytetemp == 0) || (bytetemp >= 15))
				 {
						memset(usart3_Rx_buffer,0x00,140);
						usart3_Rx_length =0 ;
						Usart2_SendData("EEpropm error,reset PP\r\n",24);
						return; 
				 }
				 SPI_Flash_SafeRead(usart3_Rx_buffer+4,password_addr+2,bytetemp);
				 Usart2_SendData(usart3_Rx_buffer,bytetemp+4);
				 Usart2_SendData("\r\n",2); 
			 }
			 else Usart2_SendData("receive command error!\r\n",24);		 
			}
		  else if((usart3_Rx_buffer[1] == 'R') && (usart3_Rx_buffer[2] == 'S') && (usart3_Rx_buffer[3] == 'T'))
			{
				Usart2_SendData("Reset MCU OK\r\n",14);
				Usart2_SendData("\r\n",2);
			 SystemReset()  ;//����һ�������λ

			}
      else if((usart3_Rx_buffer[1] != '%')||(usart3_Rx_buffer[2] != '%')||(usart3_Rx_buffer[3] != '%'))  //set command 
      {  
         bytetemp = (usart3_Rx_length - 3);
				 if((usart3_Rx_buffer[1] == 'S') && (usart3_Rx_buffer[2] == 'R'))
				 {
					 if((bytetemp == 0) || (bytetemp >= 16))
					 {
							memset(usart3_Rx_buffer,0x00,140);
							usart3_Rx_length =0 ;
							Usart2_SendData("not valid!!!\r\n",14);
							return;
					 }
					 if((bytetemp > 0) && (bytetemp <= 9))
						bytenumbertemp[0] = (bytetemp + 0x30);
					 else bytenumbertemp[0] = (bytetemp + 0x37);
						 SPI_Flash_SafeWrite(bytenumbertemp,IPaddress_addr,1);
						 SPI_Flash_SafeWrite(usart3_Rx_buffer+3,IPaddress_addr+1,bytetemp);
						 Usart2_SendData("OK\r\n",4);
				 }
				 else
				 {//���ԣ��������ü�������	 % P H N , 1 , 1 1 , 1  8  3  5  6  9  5  7  8  2  7
					 //                        0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
					 //                        % P H N , 2 , 8 , 6 7  1  0  8  0  6  3
						if((usart3_Rx_buffer[1] == 'P') && (usart3_Rx_buffer[2] == 'H') && (usart3_Rx_buffer[3] == 'N'))
						{
							if(usart3_Rx_buffer[5] == '1')
							{
								for(i=0; i<2;i++)
								{
                  if(usart3_Rx_buffer[8+i]==',')
										break ;
                }
								if(i==0)
								{
                  dou_temp[2] =usart3_Rx_buffer[7]-0x30;
									dou_temp[1] =usart3_Rx_buffer[7];
									dou_temp[0]=0x30;
									SPI_Flash_SafeWrite(dou_temp,PhoneNumber1_addr,2);
									SPI_Flash_SafeWrite(usart3_Rx_buffer+9,PhoneNumber1_addr+2,dou_temp[2] );
									Usart2_SendData("OK\r\n",4);
									i=0;
                }								  
								else if(i==1)
								{
                  dou_temp[2] =(usart3_Rx_buffer[7]-0x30)*10+(usart3_Rx_buffer[8]-0x30);
									dou_temp[1] =usart3_Rx_buffer[8];
									dou_temp[0]=usart3_Rx_buffer[7];
									SPI_Flash_SafeWrite(dou_temp,PhoneNumber1_addr,2);
									SPI_Flash_SafeWrite(usart3_Rx_buffer+10,PhoneNumber1_addr+2,dou_temp[2] );
									Usart2_SendData("OK\r\n",4);
									i=0;
                }
							}
							if(usart3_Rx_buffer[5] == '2')
							{
								for(i=0; i<2;i++)
								{
                  if(usart3_Rx_buffer[8+i]==',')
										break ;
                }
								if(i==0)
								{
                  dou_temp[2] =usart3_Rx_buffer[7]-0x30;
									dou_temp[1] =usart3_Rx_buffer[7];
									dou_temp[0]=0x30;
									SPI_Flash_SafeWrite(dou_temp,PhoneNumber2_addr,2);
									SPI_Flash_SafeWrite(usart3_Rx_buffer+9,PhoneNumber2_addr+2,dou_temp[2] );
									Usart2_SendData("OK\r\n",4);
									i=0;
                }								  
								else if(i==1)
								{
                  dou_temp[2] =(usart3_Rx_buffer[7]-0x30)*10+(usart3_Rx_buffer[8]-0x30);
									dou_temp[1] =usart3_Rx_buffer[8];
									dou_temp[0]=usart3_Rx_buffer[7];
									SPI_Flash_SafeWrite(dou_temp,PhoneNumber2_addr,2);
									SPI_Flash_SafeWrite(usart3_Rx_buffer+10,PhoneNumber2_addr+2,dou_temp[2] );
									Usart2_SendData("OK\r\n",4);
									i=0;
                }	
							}

						}			
					 if((bytetemp == 0) || (bytetemp >= 15))
						{
							memset(usart3_Rx_buffer,0x00,140);
							usart3_Rx_length =0 ;
							Usart2_SendData("not valid!!!\r\n",14);
							return;
						}
						bytenumbertemp[0] = (((bytetemp & 0xf0) >> 4)+0x30);  //����hex��ASCIIת�ķ�ʽֻ���ڱ���С��0x0F��ʱ����á�
						bytenumbertemp[1] = ((bytetemp & 0x0f) + 0x30); 
					  if((usart3_Rx_buffer[1] == 'P') && (usart3_Rx_buffer[2] == 'T'))
					  {
							 SPI_Flash_SafeWrite(bytenumbertemp,PortNumber_addr,2); //д���ݵĳ���
							 SPI_Flash_SafeWrite(usart3_Rx_buffer+3,PortNumber_addr+2,bytetemp);
							 Usart2_SendData("OK\r\n",4);
					  }
					  else if((usart3_Rx_buffer[1] == 'A') && (usart3_Rx_buffer[2] == 'P'))
					  {
							 SPI_Flash_SafeWrite(bytenumbertemp,apn_addr,2);
							 SPI_Flash_SafeWrite(usart3_Rx_buffer+3,apn_addr+2,bytetemp);
							 Usart2_SendData("OK\r\n",4);
					  }
						else if((usart3_Rx_buffer[1] == 'U') && (usart3_Rx_buffer[2] == 'U'))
						{
							 SPI_Flash_SafeWrite(bytenumbertemp,user_addr,2);
							 SPI_Flash_SafeWrite(usart3_Rx_buffer+3,user_addr+2,bytetemp);
							 Usart2_SendData("OK\r\n",4);
						}							 
				    else if((usart3_Rx_buffer[1] == 'P') && (usart3_Rx_buffer[2] == 'P'))
						{ 
								SPI_Flash_SafeWrite(bytenumbertemp,password_addr,2);
								SPI_Flash_SafeWrite(usart3_Rx_buffer+3,password_addr+2,bytetemp);
								Usart2_SendData("OK\r\n",4);
						}

						else Usart2_SendData("receive command error!\r\n",24);
			   }
       }
			 else Usart2_SendData("COMMAND ERROR!",sizeof("COMMAND ERROR!"));
    }

  else if(usart3_Rx_buffer[0] == '*')
    {
       if((usart3_Rx_buffer[1] == 'I') && (usart3_Rx_buffer[2] == 'D') && (usart3_Rx_buffer[3] == '#'))
        {
          SPI_Flash_SafeWrite(Mid,MID_addr,0x6);
          for(i = 0; i < 6; i++)
          {
             if(Mid[i] == 0xff)
             {
                Mid[i] = 0x30;
                SPI_Flash_SafeWrite("000000",MID_addr,0x06);
								Usart2_SendData("the ID is : ",12);
								Usart2_SendData("000000",6);
								Usart2_SendData("\r\n",2); 
								memset(usart3_Rx_buffer,0x00,140);
						    usart3_Rx_length =0 ;
								return;              
             }
          }
          Usart2_SendData("the ID is : ",12);
          Usart2_SendData(Mid,6);
          Usart2_SendData("\r\n",2);
        }
       else
        { 
         if((usart3_Rx_buffer[7] != ',') || (usart3_Rx_buffer[11] != ','))
         {
						memset(usart3_Rx_buffer,0x00,140);
						usart3_Rx_length =0 ;
						Usart2_SendData("not valid!!!\r\n",14);
						return;    
         }
         if((usart3_Rx_buffer[8] != '9') || (usart3_Rx_buffer[9] != '9') ||
            (usart3_Rx_buffer[10] != '9'))
         {
						memset(usart3_Rx_buffer,0x00,140);
						usart3_Rx_length =0 ;
						Usart2_SendData("not valid!!!\r\n",14);
						return;          
         }
         if((usart3_Rx_buffer[12] == '2') && (usart3_Rx_buffer[13] == 'D') && (usart3_Rx_buffer[21] == '#'))
         {
            if(usart3_Rx_buffer[14] != ',')
            {
							memset(usart3_Rx_buffer,0x00,140);
						  usart3_Rx_length =0 ;
							Usart2_SendData("not valid!!!\r\n",14);
							return;          
            }
            SPI_Flash_SafeWrite(Mid,MID_addr,0x6);
//            if(Usart0Rxbuffer[15] == '0')
//            {
//				    ResetGPRSTemp();
//				    UART6_SendData("unvalid new ID!!!\r\n",19);
//                  return;          
              
//            }
            if((Mid[0] == 0xff) && (Mid[1] == 0xff) && (Mid[2] == 0xff)
               && (Mid[3] == 0xff) && (Mid[4] == 0xff) && (Mid[5] == 0xff))
             {
               SPI_Flash_SafeWrite("000000",MID_addr,0x6);
               Mid[0] = 0x30;
               Mid[1] = 0x30;
               Mid[2] = 0x30;
               Mid[3] = 0x30;
               Mid[4] = 0x30;
               Mid[5] = 0x30;
             }
            for(i = 0;i < 6; i++)
            {
               if(usart3_Rx_buffer[i+15] <= 47)
               {
									memset(usart3_Rx_buffer,0x00,140);
						      usart3_Rx_length =0 ;
									Usart2_SendData("unvalid new ID!!!\r\n",19);
									return;          
			         } 
               else if((usart3_Rx_buffer[i+15] >= 58) && (usart3_Rx_buffer[i+15] <= 64))
							 {
									memset(usart3_Rx_buffer,0x00,140);
						      usart3_Rx_length =0 ;
									Usart2_SendData("unvalid new ID!!!\r\n",19);
									return;         
							 }
               else if(usart3_Rx_buffer[i+15] >= 91)
							 {
									memset(usart3_Rx_buffer,0x00,140);
						      usart3_Rx_length =0 ;
									Usart2_SendData("unvalid new ID!!!\r\n",19);
									return;         
							 }
             }
            for(i = 0;i < 6; i++)
             {
               if(usart3_Rx_buffer[i+1] != Mid[i])
               {
									memset(usart3_Rx_buffer,0x00,140);
						      usart3_Rx_length =0 ;
									Usart2_SendData("unvalid old ID!!!\r\n",19);
									return;          
               }
             }
            SPI_Flash_SafeWrite(usart3_Rx_buffer+15,MID_addr,6);
            Usart2_SendData("update ID success\r\n",19);
          }
#if 0				
         else if((usart3_Rx_buffer[12] == '0') && (usart3_Rx_buffer[13] == '3') && (usart3_Rx_buffer[14] == '#'))
          {
            usart3_Rx_buffer[14] = ',';
            i = 0;
						while(rx1_bak[i] != '#')
	   	      {
							i++;
							if(i >= 65)
							 {
								 UART6_SendData("GPS data is error,check GPS!\r\n",30);
								 ResetGPRSTemp();
								 return;
							 }
					  }
            UART6_SendData(Usart0Rxbuffer,15);
            UART6_SendData("00,",3);
            UART6_SendData(rx1_bak,i+1);
            UART6_SendData("\r\n",2);
          }
#endif						
         else Usart2_SendData("receive command error!\r\n",24);
        }
			
    }

  else Usart2_SendData("receive command error!\r\n",24);
  //ResetGPRSTemp();
	memset(usart3_Rx_buffer,0x00,140);
	usart3_Rx_length =0 ;	
	*/
}


