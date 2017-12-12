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
u16 Debug_Timeout =0;//5min=30000定时器，设置模式维持5分钟，5分钟之后自动退出



//修改IP，本机号会让机器重新注册

void Debug(void)
{
  Debug_waitcounter = 1002;//402定时10S
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
    while(Debug_Timeout<30000)//参数设置模式，定时5min,超过5分钟之后会自动重启
    {
		  IWDG_Feed();
	    if((Flag_all.PC_Command) && (ReTimeout == 7)) //timeout control
	    {
	       D_data_deal();
	       Flag_all.PC_Command = 0;
	   	}
	   	  
	   }
//		SystemReset(); //5min超时才会运行到此处，请求一次软件复位
  } 
}

void D_data_deal(void)
{
  unsigned char i,j = 0;
  unsigned char len_tem;
  unsigned char dou_temp[5] = {0};
  unsigned char bytenumbertemp[2] = {0};
  unsigned char bytetemp= 0;
  unsigned char temp_BenJiHao[12]={0}; //将接收到的本机号转成12个字节存放在这里，然后再转成BCD[6]
/*IP****START***************************************/
  if(usart3_Rx_buffer ==(u8 *)strstr((const char*)(usart3_Rx_buffer), "FHTMAINIP"))//IP,设置和查询
	{
    if(usart3_Rx_buffer[9]=='*') //IP设置
		{
			bytetemp = 15;//IP:220.178.000.056加上分割用的'.'正好是15个字节，长度固定，每段前面不足的补0
			//if((bytetemp > 0) && (bytetemp <= 9))
			//	bytenumbertemp[0] = (bytetemp + 0x30);
		//	else bytenumbertemp[0] = (bytetemp + 0x37);
			bytenumbertemp[0] = bytetemp ;
				SPI_Flash_SafeWrite(bytenumbertemp,IPaddress_addr,1);
				SPI_Flash_SafeWrite(usart3_Rx_buffer+10,IPaddress_addr+1,bytetemp);
			  Usart2_SendData(usart3_Rx_buffer,usart3_Rx_length);
				Usart2_SendData("OK\r\n",5);
			
			 SPI_Flash_SafeRead(bytenumbertemp,Terminal_auth_Flag_addr,0x01);
			 if(0x55 == bytenumbertemp[0])//之前注册成功,这里要清除存储在SPI-FLASH中的注册成功标志位
			 {
				 bytenumbertemp[0]=0xFF;
				 SPI_Flash_SafeWrite(bytenumbertemp,Terminal_auth_Flag_addr,0x01);
			 }
			
    }
		else if(usart3_Rx_buffer[9]=='?') //查询IP
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
		else Usart2_SendData("COMMAND ERROR!",sizeof("COMMAND ERROR!"));//接收的命令错误		
  }
/*IP****END***************************************/	
	
/******PORT  start********************/	
	if(usart3_Rx_buffer ==(u8 *)strstr((const char*)(usart3_Rx_buffer), "FHTUDPPORT"))//端口PORT,设置和查询,这里只有UDP
	{    
		if(usart3_Rx_buffer[10]=='*') //端口设置
		{
			bytetemp = (usart3_Rx_length - 11);
			if((bytetemp == 0) || (bytetemp >= 15))
			{
				memset((u8 *)usart3_Rx_buffer,0x00,1100);
				usart3_Rx_length =0 ;
				Usart2_SendData("COMMAND ERROR!",sizeof("COMMAND ERROR!"));//接收的命令错误		
				return;
			}
			//bytenumbertemp[0] = (((bytetemp & 0xf0) >> 4)+0x30);  //这种hex往ASCII转的方式只有在变量小于0x0F的时候管用。
			//bytenumbertemp[1] = ((bytetemp & 0x0f) + 0x30); 
			bytenumbertemp[0] = bytetemp;
			SPI_Flash_SafeWrite(bytenumbertemp,PortNumber_addr,1); //写数据的长度
			SPI_Flash_SafeWrite(usart3_Rx_buffer+11,PortNumber_addr+2,bytetemp);
			Usart2_SendData(usart3_Rx_buffer,usart3_Rx_length);
			Usart2_SendData(" OK\r\n",5);		
    }
		else if(usart3_Rx_buffer[10]=='?') //端口查询
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
		else Usart2_SendData("COMMAND ERROR!",sizeof("COMMAND ERROR!"));//接收的命令错误		
  }
/******PORT  end********************/			
/******APN  START********************/		
	if(usart3_Rx_buffer ==(u8 *)strstr((const char*)(usart3_Rx_buffer), "FHTSETMAINAPN"))//端口PORT,设置和查询,这里只有UDP
	{    
		if(usart3_Rx_buffer[13]=='*') //端口APN设置
		{
			bytetemp = (usart3_Rx_length - 14);
			if((bytetemp == 0) || (bytetemp >= 15))
			{
				memset(usart3_Rx_buffer,0x00,1100);
				usart3_Rx_length =0 ;
				Usart2_SendData("COMMAND ERROR!",sizeof("COMMAND ERROR!"));//接收的命令错误		
				return;
			}
			bytenumbertemp[0] = (((bytetemp & 0xf0) >> 4)+0x30);  //这种hex往ASCII转的方式只有在变量小于0x0F的时候管用。
			bytenumbertemp[1] = ((bytetemp & 0x0f) + 0x30); 
			SPI_Flash_SafeWrite(bytenumbertemp,apn_addr,2);
			SPI_Flash_SafeWrite(usart3_Rx_buffer+14,apn_addr+2,bytetemp);							
			Usart2_SendData(usart3_Rx_buffer,usart3_Rx_length);
			Usart2_SendData(" OK\r\n",5);		
    }
		else if(usart3_Rx_buffer[13]=='?') //查询APN
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
		else Usart2_SendData("COMMAND ERROR!",sizeof("COMMAND ERROR!"));//接收的命令错误		
  }	
/******APN  END********************/	
/******benjihao start********************/
	if(usart3_Rx_buffer ==(u8 *)strstr((const char*)(usart3_Rx_buffer), "FHTSETMID"))//本机号,设置和查询
	{
    if(usart3_Rx_buffer[9]=='*') //本机号设置
		{
			bytetemp = usart3_Rx_length - 10;//终端手机号，BCD[6].
			if(bytetemp<12)//不满12个字节，在前面补零操作
			{
        for(i=0;i<(12-bytetemp);i++)
				  temp_BenJiHao[i]=0x00;
				memcpy(temp_BenJiHao+i,usart3_Rx_buffer+10,bytetemp);				
      }
			bytetemp =6;//这里给他固定为12，方便操作
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
			 if(0x55 == bytenumbertemp[0])//之前注册成功,这里要清除存储在SPI-FLASH中的注册成功标志位
			 {
				 bytenumbertemp[0]=0xFF;
				 SPI_Flash_SafeWrite(bytenumbertemp,Terminal_auth_Flag_addr,0x01);
			 }
    }
		else if(usart3_Rx_buffer[9]=='?') //查询本机号
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
		else Usart2_SendData("COMMAND ERROR!",sizeof("COMMAND ERROR!"));//接收的命令错误		
  }
/******benjihao end********************/
/******RESET**start******************/	
	//FHTRESETHOST*
	if(usart3_Rx_buffer ==(u8 *)strstr((const char*)(usart3_Rx_buffer), "FHTRESETHOST*"))//复位主机
	{
    Usart2_SendData("FHTRESET OK\r\n",sizeof("FHTRESET OK\r\n"));
		//Usart2_SendData("\r\n",2);
		SystemReset()  ;//请求一次软件复位

  }
	
/******RESET***end*****************/	
/******Check Version**start******************/		
	if(usart3_Rx_buffer ==(u8 *)strstr((const char*)(usart3_Rx_buffer), "FHTZDZT?"))//复位主机
	{
    Usart2_SendData(Version_arry,Version_Len);
		//Usart2_SendData("\r\n",2);
		//SystemReset()  ;//请求一次软件复位

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
			 SystemReset()  ;//请求一次软件复位

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
				 {//测试：用来设置监听号码	 % P H N , 1 , 1 1 , 1  8  3  5  6  9  5  7  8  2  7
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
						bytenumbertemp[0] = (((bytetemp & 0xf0) >> 4)+0x30);  //这种hex往ASCII转的方式只有在变量小于0x0F的时候管用。
						bytenumbertemp[1] = ((bytetemp & 0x0f) + 0x30); 
					  if((usart3_Rx_buffer[1] == 'P') && (usart3_Rx_buffer[2] == 'T'))
					  {
							 SPI_Flash_SafeWrite(bytenumbertemp,PortNumber_addr,2); //写数据的长度
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


