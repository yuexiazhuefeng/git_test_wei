#include "Test.h"

u8 test_OBD_PID_T = 0;


const unsigned char ascTable[17] = {"0123456789ABCDEF"};
void OBD_PID_Test(void)
{
  u16 i = 0;
	u8 temp_arry[3] = {0};
	if(test_OBD_PID_T>=10)
	{	
		test_OBD_PID_T=0;
		memset(OBD_PID_Buffer,0x00,OBD_BUF_LEN);
		RING_FLUSH(&ring_obd_buffer);
		Read_PID(PID_user_defined,PID_user_defined_number);
		if(OBD_Com_Flag.OBD_PID_complete)
		{
      OBD_Com_Flag.OBD_PID_complete = 0;
			IWDG_Feed();
			for(i=0;i<ring_obd_buffer.count;i++)
			{
				temp_arry[0] = ascTable[(OBD_PID_Buffer[i]>>4)&0x0f];
				temp_arry[1] = ascTable[(OBD_PID_Buffer[i])&0x0f];
				temp_arry[2] = 0x20;
        Usart2_SendData(temp_arry,3);
				IWDG_Feed();
				//Usart3_SendData(,2);
      }
			
    }
	}
}

#if 0
 u8 test_arry_write[20] = {0};
 u8 test_arry_read[20] = {0};
 u8 SPI_TEST = 0;
#if 0
 void test_SPI(void)
{
    u8 i = 0;
	  for(i=0;i<20;i++)
	 {
       test_arry_write[i] = i;
   }
	// SPI_Flash_Write(test_arry_write,0x000000,20);
	// SPI_Flash_Read(test_arry_read,0x000000,20);
	 SPI_Flash_SafeWrite(test_arry_write,0x000000,20);
	 SPI_Flash_SafeRead(test_arry_read,0x000000,20);

#if 0	 
	 for(i=0;i<20;i++)
	 {
      if(test_arry_write[i] != test_arry_read[i])
			{
         SPI_TEST = 0xaa;
				break;
      }
			else
			{
         SPI_TEST = 0x55;
      }
   }
#endif	 
}
#endif

/*****/
void init_arry(void)
{
  u8 i =0;
	for(i=0;i<10;i++)
	{
     test_arry_write[i]= 0x30+i;
  }
	for(i=0;i<10;i++)
	{
     test_arry_read[i]= 0x39-i;
  }
}
void test_show_led(void)
{
  
	if(readwriteerror == 0x88)
	{
     if((0 < timer2_100ms )&&( timer2_100ms < 2))
			{
         
				//Usart1_SendData(test_arry_write, 1);
				//Usart1_SendData("jYR",1);
				//Usart2_SendData("usart2\r\n",8);
				//Usart3_SendData("usart3\r\n",8);
				PCout(6) =1;
				PCout(8) =0;
      }
			if((timer2_100ms>99)&&(timer2_100ms <102))
			{
        Usart1_SendData(test_arry_read, 20);
				//GPIO_SetBits(GPIOA,GPIO_Pin_11);
				PCout(6) =0;
				PCout(8) =0;
      }	
 }
	 if(readwriteerror == 0xaa)
	{
     if((0 < timer2_100ms )&&( timer2_100ms < 2))
			{
         
				PCout(6) =0;
				PCout(8) =1;
      }
			if((timer2_100ms>99)&&(timer2_100ms <102))
			{
        
				//GPIO_SetBits(GPIOA,GPIO_Pin_11);
				PCout(6) =0;
				PCout(8) =0;
      }	
  }
	else if(readwriteerror == 0x00)
	{
     PCout(6) =1;
		 PCout(8) =1;
  }
		
}
const u8 AA1[15]={"Send GPS Data\r\n"};
const u8 AA2[15]={"Send OBD Data\r\n"};
void TESET_GSM_Send(void)
{
  u8 arry[10];
	u8 i=0;
	if(timer2_100ms>999)
	{
		memset(arry,0x38,10);
		GSM_Send_Data(arry,10);
    timer2_100ms = 0;
  }
//    memset(arry,0x38,10);
//		GSM_Send_Data(arry,10,(u8 *)AA1,15);
//		delay_Nx100ms(100);
//		memset(arry,0x33,10);		
//		GSM_Send_Data(arry,10,(u8 *)AA2,15);
//		delay_Nx100ms(100);
//		GSM_Recivdata_deal();
  
}
void TEST_Terminal_authentication(void)  //²âÊÔÖÕ¶Ë×¢²á
{
	if(Flag_all.Terminal_auth_Flag != TRUE)
	{
    if(timer2_100ms>=500)
		{
			timer2_100ms =0 ;	
			Terminal_authentication();
		}
  }
	else if(Flag_all.Terminal_JianQuan_Flag != TRUE)
	{
    if(timer2_100ms>=500)
		{
			timer2_100ms =0 ;	
			Terminal_JianQuan();
		}
  
  }
	
}
	
#endif

/*****/

