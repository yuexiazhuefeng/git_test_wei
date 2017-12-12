#include "usart.h"

//const u8 GPS_Head[6]={'$','G','P','R','M','C'};
u8 usart1_Rx_buffer[166] ={0}; //OBDÊý¾Ý£¬×î¶àÊÇ¶ÁPIDÊý¾ÝÁ÷£¬Ã¿°ü×î¶à¶Á32¸ö£¬³¤¶ÈÎ»32*5+6=166£¬Ô¶Ð¡ÓÚ255
u8 usart1_Rx_length = 0;
u8 OBD_Package_len = 0;
u8 usart1_Rx_backup[50] ={0}; //Õâ¸öÊý×éÓÃÀ´´æ·Å´®¿Ú3»º´æÖÐµÄÊý¾Ý£¬ÒÔ±¸ºóÃæ´¦ÀíÓÃ¡£
u8 USART1_Rx_timer = 0;//¶¨Ê±30ms£¬ÓÃÀ´Çå¿Õ´®¿Ú1½ÓÊÕ»º´æ£¬·ÀÖ¹¸ÉÈÅ£¬ºÍÒ»Ð©²»±ØÒªµÄ·µ»ØÊý¾Ý

u8 usart2_Rx_buffer[90] ={0};//GPSÊý¾Ý×î¶àÖ»ÓÐ90¸ö
u8 usart2_Rx_length = 0;
u8 USART2_Rx_timer = 0;//¶¨Ê±30ms£¬ÓÃÀ´Çå¿Õ´®¿Ú2½ÓÊÕ»º´æ£¬·ÀÖ¹¸ÉÈÅ£¬ºÍÒ»Ð©²»±ØÒªµÄ·µ»ØÊý¾

u8 usart3_Rx_buffer[1100] ={0}; //GSMÊý¾ÝÔÝ¶¨Ö»ÓÐ140¸ö£¬
u16 usart3_Rx_length = 0;
u8 usart3_Rx_backup[1100] ={0}; //Õâ¸öÊý×éÓÃÀ´´æ·Å´®¿Ú3»º´æÖÐµÄÊý¾Ý£¬ÒÔ±¸ºóÃæ´¦ÀíÓÃ¡£
u8 usart3_Rx_SM[150] = {0};  //¶ÌÐÅ»º´æ
u8 usart3_Rx_SM_len = 0;     //½ÓÊÕ¶ÌÐÅ³¤¶È
u16 usart3_Rx_backup_length = 0;
u8 USART3_Rx_timer = 0;//¶¨Ê±30ms£¬ÓÃÀ´Çå¿Õ´®¿Ú3½ÓÊÕ»º´æ£¬·ÀÖ¹¸ÉÈÅ£¬ºÍÒ»Ð©²»±ØÒªµÄ·µ»ØÊý¾Ý
u8 u8_0x7e[1]={0x7e};
/*****´®¿Ú³õÊ¼»¯²¿·Ö***********************/
//³õÊ¼»¯IO ´®¿Ú1 
//bound:²¨ÌØÂÊ
void uart1_init(u32 bound)
{
    //GPIO¶Ë¿ÚÉèÖÃ
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//Ê¹ÄÜUSART1£¬GPIOAÊ±ÖÓ
 	USART_DeInit(USART1);  //¸´Î»´®¿Ú1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//¸´ÓÃÍÆÍìÊä³ö
    GPIO_Init(GPIOA, &GPIO_InitStructure); //³õÊ¼»¯PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//¸¡¿ÕÊäÈë; ²»ÓÃÉÏÀ­ÊäÈëµÄÔ­ÒòÊÇÅÂ¶ÔÍâÓÐµçÁ÷³öÀ´
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //³õÊ¼»¯PA10

   //Usart1 NVIC ÅäÖÃ

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷
  
   //USART ³õÊ¼»¯ÉèÖÃ

	USART_InitStructure.USART_BaudRate = bound;//Ò»°ãÉèÖÃÎª9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½

    USART_Init(USART1, &USART_InitStructure); //³õÊ¼»¯´®¿Ú
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï
    USART_Cmd(USART1, ENABLE);                    //Ê¹ÄÜ´®¿Ú 

}

//³õÊ¼»¯IO ´®¿Ú2
//bound:²¨ÌØÂÊ4800£¬GPS
void uart2_init(u32 bound)
{
    //GPIO¶Ë¿ÚÉèÖÃ
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//Ê¹ÄÜUSART1£¬GPIOAÊ±ÖÓ
	
 	USART_DeInit(USART2);  //¸´Î»´®¿Ú1
	 //USART1_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//¸´ÓÃÍÆÍìÊä³ö
    GPIO_Init(GPIOA, &GPIO_InitStructure); //³õÊ¼»¯PA3
   
    //USART1_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//¸¡¿ÕÊäÈë
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //³õÊ¼»¯PA3

   //Usart2 NVIC ÅäÖÃ

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷
  
   //USART2 ³õÊ¼»¯ÉèÖÃ

	USART_InitStructure.USART_BaudRate = bound;  // ÓëGPSÍ¨ÐÅ£¬4800
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½

    USART_Init(USART2, &USART_InitStructure); //³õÊ¼»¯´®¿Ú
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï
    USART_Cmd(USART2, ENABLE);                    //Ê¹ÄÜ´®¿Ú 

}

//³õÊ¼»¯IO ´®¿Ú3 
//bound:²¨ÌØÂÊ
void uart3_init(u32 bound)
{
    //GPIO¶Ë¿ÚÉèÖÃ
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//Ê¹ÄÜUSART1£¬GPIOAÊ±ÖÓ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//Ê¹ÄÜUSART1£¬GPIOAÊ±ÖÓ
 	USART_DeInit(USART3);  //¸´Î»´®¿Ú3
	 //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//¸´ÓÃÍÆÍìÊä³ö
    GPIO_Init(GPIOB, &GPIO_InitStructure); //³õÊ¼»¯PB10
   
    //USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//¸¡¿ÕÊäÈë
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //³õÊ¼»¯PA10

   //Usart3 NVIC ÅäÖÃ

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷
  
   //USART3 ³õÊ¼»¯ÉèÖÃ

	USART_InitStructure.USART_BaudRate = bound;//Ò»°ãÉèÖÃÎª9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½

    USART_Init(USART3, &USART_InitStructure); //³õÊ¼»¯´®¿Ú
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï
    USART_Cmd(USART3, ENABLE);                    //Ê¹ÄÜ´®¿Ú 

}

/*****´®¿Ú³õÊ¼»¯²¿·Ö ½áÊø***********************/

/*****´®¿Ú½ÓÊÕ²¿·Ö ***********************/
u8 test_usart1_number = 0;
void USART1_IRQHandler(void)                	//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
{
	u8 Res;	   
	u8 i ;
	u8 checksum; //°üÐ£Ñé£¬ÓÃÀ´ºÍÊý¾Ý°üÖÐµÄÐ£ÑéºÍ×ö±È½Ï

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //½ÓÊÕÖÐ¶Ï(½ÓÊÕµ½µÄÊý¾Ý±ØÐëÊÇ0x0d 0x0a½áÎ²)
	{		
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
    if(USART1_Rx_timer >=2 )
		{
      memset(usart1_Rx_buffer,0x00,usart1_Rx_length);
			usart1_Rx_length = 0 ;
    }
		USART1_Rx_timer = 0;
		usart1_Rx_buffer[usart1_Rx_length++] = Res;
		if(!OBD_Com_Flag.OBD_Head) //»ñÈ¡OBDÊý¾ÝÍ· 0x55 0xAA
		{
			if(usart1_Rx_length>=2)
			{
        if((usart1_Rx_buffer[0] == 0x55)&&(usart1_Rx_buffer[1] == 0xAA))
				{
          OBD_Com_Flag.OBD_Head = 1;
					return ;
        }
				else
				{
          memset(usart1_Rx_buffer,0x00,usart1_Rx_length);
			    usart1_Rx_length = 0 ;
					return ;
        }
      }
    }
		else
		{
      if(usart1_Rx_length == 4)
			{
         OBD_Package_len = (usart1_Rx_buffer[2]<<8)|(usart1_Rx_buffer[3]);
      }
			if(usart1_Rx_length == (OBD_Package_len+4))
			{				
				checksum =0;
//55 AA 00 06 B3 20 13 08 30 24 
//0  1  2  3  4  5  6  7  8  9				
				for(i=0;i<(usart1_Rx_length-3);i++)  //¼ÆËã°üÐ£ÑéµÄÖµ£¬ÓÃÀ´¸ö°üÀïÃæµÄÐ£ÑéÂë×ö¶Ô±È
				  checksum = checksum+usart1_Rx_buffer[2+i];
				if(checksum == usart1_Rx_buffer[usart1_Rx_length-1])
				{
					if(usart1_Rx_buffer[4]==0xC2)  //PIDÊý¾ÝÁ÷
					{
            for(i=0;i<OBD_Package_len-2;i++) //°Ñ×îºóµÄ¼ìÑéÎ»Ò²È¥µô
						{
              if(ring_obd_buffer.count > ring_obd_buffer.size)
								ring_obd_buffer.count= 0;
							RING_PUTC(&ring_obd_buffer, usart1_Rx_buffer[5+i])
            }
						OBD_Com_Flag.OBD_Received_PID = 1;
						return ;
          }
					else //·ÇPIDÊý¾ÝÁ÷µÄ´¦Àí·½·¨
					{
            memset(usart1_Rx_backup,0x00,50);
						memcpy(usart1_Rx_backup,usart1_Rx_buffer,usart1_Rx_length);//Ð£ÑéºÍÑéÖ¤Íê³Éºó£¬°ÑÊÕµ½µÄOBDÊý¾Ý±£´æµ½±¸·ÝÊý×éÀïÃæ 
						memset(usart1_Rx_buffer,0x00,usart1_Rx_length);
						usart1_Rx_length = 0;
          }          
					if(usart1_Rx_backup[4]==0xC4)  //ÊÕµ½³µÉí×´Ì¬ÐÅÏ¢
					{//³µÉí×´Ì¬ÐÅÏ¢55 AA 00 0A C4 00 01 00 00 00 00 00 00 CF
            OBD_Com_Flag.OBD_Received_Body = 1;
						Body_state_deal();
          }
						 
					else if(usart1_Rx_backup[4]==0x7F) //ÊÕµ½Ó¦´ð´íÎó
						 OBD_Com_Flag.OBD_Error =1;
					else
					 OBD_Com_Flag.OBD_Received_Complete =1;  //½ÓÊÕÍê³É
        }
				else
				{
          memset(usart1_Rx_buffer,0x00,usart1_Rx_length);
			    usart1_Rx_length = 0 ;
					return ;
        }					
      }
    }		
  } 

	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)  //·¢ËÍÖÐ¶Ï£»ÅÐ¶ÏµÄÊÇTXE¶ø²»ÊÇTC
	{
		if (sendlengthU1 >= 1)
		{
			//while((USART1->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
      USART1->DR = (u8) (*(psendU1++));
			sendlengthU1--;
    }
		else
		{
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE); //¹Ø±Õ·¢ËÍÖÐ¶Ï
			return ;
    }
		
	}	
} 


void USART2_IRQHandler(void)   //´®¿Ú2ÖÐ¶Ï·þÎñ³ÌÐò GPSÊý¾Ý¿Ú   ³£ÓÃ´òÓ¡¿Ú
{
	u8 Res;
  static u8 gps_AV =0;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //½ÓÊÕÖÐ¶Ï(½ÓÊÕµ½µÄÊý¾Ý±ØÐëÊÇ0x0d 0x0a½áÎ²)
	{
		//IWDG_Feed();
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
		usart2_Rx_buffer[usart2_Rx_length++] = Res;
		if((!Flag_all.GPSHead)&&(!Flag_all.GPSreceiv_complete)) //ÅÐ¶Ï$GPRMCÍ·
		{
       if(usart2_Rx_buffer[0] =='$')
		   {
          if(usart2_Rx_length >= 6)
					{
             if((usart2_Rx_buffer[0] =='$')&&(usart2_Rx_buffer[1] =='G')&&(usart2_Rx_buffer[2] =='P')&&
							 (usart2_Rx_buffer[3] =='R')&&(usart2_Rx_buffer[4] =='M')&&(usart2_Rx_buffer[5] =='C'))
						 {
                Flag_all.GPSHead = 1;
             }
             else
						 {
                memset(usart2_Rx_buffer,0x00,usart2_Rx_length);
							  usart2_Rx_length = 0 ;
							  Flag_all.GPSHead = 0;
							 return ;
             }
          }
       }
			 else
			 {
          memset(usart2_Rx_buffer,0x00,usart2_Rx_length);
					usart2_Rx_length = 0 ;
					Flag_all.GPSHead = 0; 
				  Flag_all.GPSreceiv_complete = 0;
				 return ;
       }       			 
    }
		else
		{
       
				if (('$' == usart2_Rx_buffer[0x00]) && (0x0A == usart2_Rx_buffer[usart2_Rx_length - 1]))	//ÕæÈ·½ÓÊÕÍêGPSÊý¾Ý
				{					
					if(usart2_Rx_buffer[18]=='A') //½ÓÊÕµÄÊý¾ÝÓÐÐ§
					{
            Flag_all.GPSreceiv_complete = 1; //½ÓÊÕGPSÊý¾ÝÍê³É±êÖ¾£¬±êÖ¾Òªµ÷ÓÃchange_gpsdata()
						gps_AV = 0; 
						GPS_LED_ON;//½ÓÊÕµ½ÓÐÐ§µÄGPSÊý¾Ý£¬¿ÉÒÔÈÃµÆÁÁ						
          }
					else
					{
             memset(usart2_Rx_buffer,0x00,usart2_Rx_length);
					   usart2_Rx_length = 0 ;
					   Flag_all.GPSHead = 0; 
				     Flag_all.GPSreceiv_complete = 0;	
             if(++gps_AV > 4) 						
						 {
                gps_AV =0 ;
							 GPS_LED_OFF;  //½ÓÊÕµ½µÄGPSÊý¾Ý£¬ÎÞÐ§
							 //PCout(6)=0;
             }
						  return ;
          }
					if(Flag_all.GPSreceiv_complete)
					{
              if(change_gpsdata())
							{
                 Flag_all.GPSdeal_complete = 1;//GPSÊý¾Ý´¦ÀíÍê³É
								 memset(usart2_Rx_buffer,0x00,usart2_Rx_length);
					       usart2_Rx_length = 0 ;
					       Flag_all.GPSHead = 0; 
				         Flag_all.GPSreceiv_complete = 0;
								 memset(GPS_DATA,0x00,20);  //ÓÃÀ´²âÊÔÓÃµÄ£¬ÕýÊ½³ÌÐòÒª×¢ÊÍµô
              }
							else
							{
                 memset(usart2_Rx_buffer,0x00,usart2_Rx_length);
					       usart2_Rx_length = 0 ;
					       Flag_all.GPSHead = 0; 
				         Flag_all.GPSreceiv_complete = 0;
              }
          }
				}
    }
  } 
  if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)  //·¢ËÍÖÐ¶Ï£»ÅÐ¶ÏµÄÊÇTXE¶ø²»ÊÇTC
	{
		
		if (sendlengthU2 >= 1)
		{
			//while((USART1->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
      USART2->DR = (u8) (*(psendU2++));
			sendlengthU2--;
    }
		else
		{
      USART_ITConfig(USART2, USART_IT_TXE, DISABLE); //¹Ø±Õ·¢ËÍÖÐ¶Ï
			return ;
    }
		
	}		
}

//Æ½Ì¨·¢ËÍµÄÊý¾Ý³¬¹ý1024¸ö×Ö½Ú¾Í»á½øÐÐ·Ö°ü£¬¼ÓÉÏGSMµÄ°ü×°×Ö½Ú£¬½«½ÓÊÕ»º´æÉèÎª1100¸ö×Ö½Ú
//¼ÓÉÏGSMµÄ°ü×°×Ö½Ú£¬½«
void USART3_IRQHandler(void) //´®¿Ú3ÖÐ¶Ï·þÎñ³ÌÐò  GSMºÍ²ÎÊýÉèÖÃ½ÓÊÕ
{
	u8 Res;	  
	u8 i = 0;
  u8* pStart;	
  u8* pEnd;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //½ÓÊÕÖÐ¶Ï(½ÓÊÕµ½µÄÊý¾Ý±ØÐëÊÇ0x0d 0x0a½áÎ²)
	{
		Res =USART_ReceiveData(USART3);//(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
		if(USART3_Rx_timer >=10)
		{
      USART3_Rx_timer = 0;
			memset(usart3_Rx_buffer,0x00,140);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
			usart3_Rx_length = 0;
    }
		USART3_Rx_timer = 0;
		usart3_Rx_buffer[usart3_Rx_length++]=Res;    
/****ÉèÖÃ²ÎÊý*   */ 
		if(!Flag_all.debugmodel)
		{
       if(usart3_Rx_buffer[0]=='%')
			 {
         if(usart3_Rx_length >= 20)					 
				 {
					 if(usart3_Rx_buffer ==(u8 *)strstr((const char*)(usart3_Rx_buffer), "%%%%%%%%%%%%%%%%%%%%"))
       //    if((usart3_Rx_buffer[1]=='%')&&(usart3_Rx_buffer[2]=='%')&&(usart3_Rx_buffer[3]=='%')\
				//		 &&(usart3_Rx_buffer[4]=='%')&&(usart3_Rx_buffer[5]=='%')&&(usart3_Rx_buffer[6]=='%'))
					 {
             Flag_all.debughead = 1;
						 memset(usart3_Rx_buffer,0x00,140);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
						 usart3_Rx_length = 0;
           }
         }
       }
    }
		else
		{
       Flag_all.PC_Command =1 ;
			 ReTimeout = 0;		
    }	
/*******/	
/*******¼ì²éOK*******/
//\r \n O K \r \n
//0  1  2 3 4  5
		
    if(((!Flag_all.OK)&&(Res == 'K')&&(usart3_Rx_length >2))||(Flag_all.OK_O))
    {
	    if((usart3_Rx_length>2)&&(usart3_Rx_buffer[usart3_Rx_length-2]=='O'))
			{	
				Flag_all.OK_O = 1;
				
			 // memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
			  //usart3_Rx_length = 0;
			}
			if((Flag_all.OK_O)&&(usart3_Rx_buffer[usart3_Rx_length-2]=='\r')&&(usart3_Rx_buffer[usart3_Rx_length-1]=='\n'))
			{
        Flag_all.OK_O = 0;
				Flag_all.OK = 1;
				memset(usart3_Rx_backup,0x00,140);
				memcpy(usart3_Rx_backup,usart3_Rx_buffer,usart3_Rx_length);
				usart3_Rx_backup_length = usart3_Rx_length;
				memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
			  usart3_Rx_length = 0;
      }			
	  }
		if(Flag_all.COPS)
			//((usart3_Rx_length>7)&&(usart3_Rx_buffer[usart3_Rx_length-2]=='\r')&&(usart3_Rx_buffer[usart3_Rx_length-1]=='\n'))
		{
      if((usart3_Rx_length>7)&&(usart3_Rx_buffer[usart3_Rx_length-2]=='\r')&&(usart3_Rx_buffer[usart3_Rx_length-1]=='\n'))
			{
        if(NULL!=strstr((const char*)(usart3_Rx_buffer), "+COPS:"))
				{  
          //Flag_all.COPS = 0;
					return;
        }
				else
				{  
          memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
			    usart3_Rx_length = 0;
        }
				
      }		
    }
		
		
//		if((Flag_all.OK))
			
/*********¼ì²éOK***********************/
/*********¼ì²éSISW***********************/	
//\r \n ^  S  I  S  W  :     0  ,  1  0  ,  0 \r \n
//0D 0A 5E 53 49 53 57 3A 20 30 2C 31 30 2C 30 0D 0A	
		
	  if((!Flag_all.SISW)&&(usart3_Rx_buffer[6]=='W')&&(usart3_Rx_buffer[usart3_Rx_length-2]=='\r')&&(usart3_Rx_buffer[usart3_Rx_length-1]=='\n'))
    {
	    if((usart3_Rx_buffer[2]=='^')&&(usart3_Rx_buffer[3]=='S')&&(usart3_Rx_buffer[4]=='I')&&(usart3_Rx_buffer[5]=='S'))
			{	
				if(usart3_Rx_length <=14)
				{
           memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
			     usart3_Rx_length = 0;
        }
				else
				{
          Flag_all.SISW = 1;
				  memset(usart3_Rx_backup,0x00,140);
				  memcpy(usart3_Rx_backup,usart3_Rx_buffer,usart3_Rx_length);
			    memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
			    usart3_Rx_length = 0;
        }			
			}		
	  }
/*********¼ì²éSISW***********************/		
		
/*********¼ì²éSISR***Start********************/			
//\r \n ^  S  I  S  R  :     0  ,     9  ,     "  1  2  3  4  5  6  7  8  9"    \r \n
//0D 0A 5E 53 49 53 52 3A 20 30 2C 20 39 2C 20 22 31 32 33 34 35 36 37 38 39 22 0D 0A
 		
		
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27		
/*
\r \n ^  S  I  S  R  :     0   ,    1  1  8  ,     "  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  G  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z  a  b  c  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  G  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z  a  b  c  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z  " \r  \n
0D 0A 5E 53 49 53 52 3A 20 30 2C 20 31 31 38 2C 20 22 31 32 33 34 35 36 37 38 39 41 42 43 44 45 46 47 49 4A 4B 4C 4D 4E 4F 50 51 52 53 54 55 56 57 58 59 5A 61 62 63 65 66 67 68 69 6A 6B 6C 6D 6E 6F 70 71 72 73 74 75 76 77 78 79 7A 31 32 33 34 35 36 37 38 39 41 42 43 44 45 46 47 49 4A 4B 4C 4D 4E 4F 50 51 52 53 54 55 56 57 58 59 5A 61 62 63 65 66 67 68 69 6A 6B 6C 6D 6E 6F 70 71 72 73 74 75 76 77 78 79 7A 22 0D 0A 
0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129130131132134135136137138139     		
*/		
		if((!Flag_all.SISR)&&(usart3_Rx_buffer[6]=='R')&&(usart3_Rx_buffer[usart3_Rx_length-2]=='\r')&&(usart3_Rx_buffer[usart3_Rx_length-1]=='\n'))
    {
	    if((usart3_Rx_buffer[2]=='^')&&(usart3_Rx_buffer[3]=='S')&&(usart3_Rx_buffer[4]=='I')&&(usart3_Rx_buffer[5]=='S')&&(usart3_Rx_buffer[7]==':'))
			{
				i =0;
				GSM_Reciev_len = 0;
			#if 0 //MG323
				if(NULL!=strstr((const char*)(usart3_Rx_buffer), "\r\n^SISR: 0,0,0\r\n"))
				{                                      //GSM»º´æÃ»ÓÐÊý¾Ý
          Flag_all.haveGSMdata =SISR_7E_EMPTY; //»º´æÎª¿Õ
					return;
        }
				if(NULL!=strstr((const char*)(usart3_Rx_buffer), "\r\n^SISR: 0, 1\r\n"))
				{                                        //GSM»º´æÖÐÓÐÊý¾Ý£¬Çó¶ÁÈ¡¡£ Ö÷¶¯ÉÏ±¨µÄ¡£
          memset(usart3_Rx_buffer,0x00,140);  //Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
			    usart3_Rx_length = 0;	 
					Flag_all.haveGSMdata = SISR_1 ;    //»º´æÓÐÊý¾Ý£¬´ý¶ÁÈ¡
					return;
        }
			#endif //MG323
				if(NULL!=strstr((const char*)(usart3_Rx_buffer), "\r\n^SISR: 0,0\r\n"))
				{                                      //GSM»º´æÃ»ÓÐÊý¾Ý
          Flag_all.haveGSMdata =SISR_7E_EMPTY; //»º´æÎª¿Õ
					return;
        }
				if(NULL!=strstr((const char*)(usart3_Rx_buffer), "\r\n^SISR: 0,1\r\n"))
				{                                        //GSM»º´æÖÐÓÐÊý¾Ý£¬Çó¶ÁÈ¡¡£ Ö÷¶¯ÉÏ±¨µÄ¡£
          memset(usart3_Rx_buffer,0x00,140);  //Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
			    usart3_Rx_length = 0;	 
					Flag_all.haveGSMdata = SISR_1 ;    //»º´æÓÐÊý¾Ý£¬´ý¶ÁÈ¡
					return;
        }
					
			//	else 
				//	Flag_all.haveGSMdata =0;
				//i =0;
//MG323				while(usart3_Rx_buffer[11+i]!=',')
				while((usart3_Rx_buffer[11+i]!='\r')&&(usart3_Rx_buffer[11+i+1]!='\n'))
				{                                      //ÌáÈ¡½ÓÊÕµ½µÄÊý¾Ý³¤¶ÈÐÅÏ¢×î³¤3Î»					
          if(i==0)
					{
						if((usart3_Rx_buffer[11+i]>=0x30)&&(usart3_Rx_buffer[11+i] <= 0x39))
						{
							GSM_Reciev_len = (usart3_Rx_buffer[11]-0x30);
            }
          }	
					else
					{
            if((usart3_Rx_buffer[11+i]>=0x30)&&(usart3_Rx_buffer[11+i] <= 0x39))
						   GSM_Reciev_len=GSM_Reciev_len*10+(usart3_Rx_buffer[11+i]-0x30);
          }						
					i++;
        }
				if((GSM_Reciev_len+18) >= usart3_Rx_length)  //16//Ã»½ÓÊÕÍêÊý¾Ý£¬ÕâÀïÊ¹ÓÃ½ÓÊÕ³¤¶ÈÀ´ÅÐ¶ÏÊÇ·ñ½ÓÍêÕû£¬·ÀÖ¹²¿±êÊý¾ÝÄÚ²¿ÓÐ0A0D£¬
				{ 
           return;
        } 
				
				pStart = (u8 *)memchr((const char*)usart3_Rx_buffer,0x7E,usart3_Rx_length); //ÕÒµ½0x7e
				pEnd =(u8 *)memchr((const char *)(pStart+1),0x7e,GSM_Reciev_len+1);
			//	pEnd = (u8 *)strstr((const char*)(pStart+1), (const char *)u8_0x7e);
				if(GSM_Reciev_len!=((pEnd - pStart)+1))
				{
          memset(usart3_Rx_buffer,0x00,140);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
			    usart3_Rx_length = 0;	
          return ;
        }
		  	
				if(Flag_all.haveGSMdata != SISR_7E_OK)
				{
          memset(GSM_Reciev,0x00,140);
				  memcpy(GSM_Reciev,(pStart),GSM_Reciev_len);
			    memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
			    usart3_Rx_length = 0;
					//Flag_all.SISR = 1; // ÓÐ7EÊý¾ÝÐè´¦Àí
				  Flag_all.haveGSMdata =SISR_7E_OK;  // ÓÐ7EÊý¾ÝÐè´¦Àí					
        }
				else //ÓÐÊý¾ÝÕýÔÚ´¦Àí£¬Ö±½Ó¶ªÆúÏÖÔÚ½ÓÊÜµ½µÄ£¬ÒÔºó»á¿¼ÂÇÏûÏ¢¶ÓÁÐµÄ·½Ê½À´×ö£¬¿ÉÒÔÈÃÏûÏ¢ÅÅ¶Ó¡£
				{
          memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
			    usart3_Rx_length = 0;
        }
			}
	  }
/*********¼ì²éSISR***End********************/		
/********À´µç****************************************************************/		
//0D 0A 52 49 4E 47 0D 0A 0D 0A 2B 43 4C 49 50 3A 20 22 31 38 33 35 36 39 35 37 38 32 38 22 2C 31 36 31 2C 22 22 2C 2C 2C 30 0D 0A 
//\r \n R  I  N  G  \r \n \r \n +  C  L  I  P  :     "  1  8  3  5  6  9  5  7  8  2  8  "  ,  1  6  1  ,  "  "  ,  ,  ,  0	 \r \n
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42
//      +  C  R  I  N  G  :     V  O  I  C  E  \r \n \r \n +  C  L  I  P  :  "18356957828",161,"",,,0		
		if(((!Flag_all.RING)&&(Res=='G')&&(usart3_Rx_length >5))||((Flag_all.RING_S==1)&&(usart3_Rx_length >19)))
		{
      if(!Flag_all.RING_S)
			{
        if(NULL!=strstr((const char*)(usart3_Rx_buffer), "RING"))
			  //if((usart3_Rx_buffer[usart3_Rx_length-4]=='R')&&(usart3_Rx_buffer[usart3_Rx_length-3]=='I')&&(usart3_Rx_buffer[usart3_Rx_length-2]=='N'))
			  {
					Flag_all.RING_S = 1;
			  }
      }			
			else if((usart3_Rx_buffer[usart3_Rx_length-2]=='\r')&&(usart3_Rx_buffer[usart3_Rx_length-1]=='\n'))//||(Flag_all.RING_S))
			{				
				//if(Flag_all.RING_S)
				//{//Õâ¸öÊ±ºò´®¿ÚµÄÊý¾ÝÒÑ¾­½ÓÊÕÍê³ÉÁË£¬
					if(NULL!=strstr((const char*)(usart3_Rx_buffer), "+CLIP:"))				
					{
						GSM_Ring_Number_len = 0;
						pEnd = (u8 *)strstr((const char*)(usart3_Rx_buffer+8), "+CLIP");
						pStart = (u8 *)strstr((const char*)pEnd, "\"");
						pEnd = (u8 *)strstr((const char*)(pStart+1), "\"");
						GSM_Ring_Number_len = pEnd-pStart-1;
						if((GSM_Ring_Number_len<5)||(GSM_Ring_Number_len >15))//ÅÐ¶ÏÀ´µçÏÔÊ¾µÄµç»°ºÅÂë³¤¶ÈÊÇ·ñ¹ý¶Ì»ò¹ý³¤
						{
							memset(usart3_Rx_buffer,0x00,140);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
							usart3_Rx_length = 0;	
							return ;					
						}
						Flag_all.RING_S = 0;
						Flag_all.RING = 1;
						memset(GSM_Ring_Number,0x00,15);
						memcpy(GSM_Ring_Number,(pStart+1),GSM_Ring_Number_len);
						memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
						usart3_Rx_length = 0;
				  }
				  else
				  {
					  memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
					  usart3_Rx_length = 0;
						Flag_all.RING_S = 0;
				  }
				//}
				 if(usart3_Rx_length >50)//else
				{
					 memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
					 usart3_Rx_length = 0;
					 Flag_all.RING_S = 0;
				}
			}
    }
/********À´µç****************************************************************/		
/**Í¨»°½áÊø**********************/
//0D 0A 5E 43 45 4E 44 3A 20 31 2C 30 2C 34 38 0D 0A 0D 0A 4F 4B 0D 0A 
//\r \n ^  C  E  N  D  :     1  ,  0  ,  4  8	 \r \n \r \n O  K  \r \n
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22		
		if(Flag_all.jieting)
		{
      if((usart3_Rx_length>6)&&(usart3_Rx_buffer[usart3_Rx_length-2]=='\r')&&(usart3_Rx_buffer[usart3_Rx_length-1]=='\n'))
			{
        if(NULL!=strstr((const char*)(usart3_Rx_buffer), "^CEND:"))
				//if((usart3_Rx_buffer[2]=='^')&&(usart3_Rx_buffer[3]=='C')&&(usart3_Rx_buffer[4]=='E')&&(usart3_Rx_buffer[5]=='N')&&(usart3_Rx_buffer[6]=='D'))
				{
          Flag_all.jieting =0;
					Flag_all.guaji = 1;
					memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
					usart3_Rx_length = 0;
        }
      }
    }
		if(Flag_all.CALL_Flag)//ÓÐ²¦ºÅ¶¯×÷£¬ÐèÒª¼ì²â²¦ºÅÏà¹ØµÄÊý¾Ý
		{
			//^ORIG: 1,0
      if((usart3_Rx_length>8)&&(usart3_Rx_buffer[usart3_Rx_length-2]=='\r')&&(usart3_Rx_buffer[usart3_Rx_length-1]=='\n'))
			{
        if(NULL!=strstr((const char*)(usart3_Rx_buffer), "^ORIG:"))
				{
          Flag_all.ORIG_Flag =1; //
        }
				//^CONF: 1
				if(NULL!=strstr((const char*)(usart3_Rx_buffer), "^CONF:"))
				{
          Flag_all.CONF_Flag = 1;
        }
				//^CONN: 1,0 
				if(NULL!=strstr((const char*)(usart3_Rx_buffer), "^CONN:"))
				{
          Flag_all.CONN_Flag = 1;
        }
				//^CEND: 1,6, 67, 16 
				if(NULL!=strstr((const char*)(usart3_Rx_buffer), "^CEND:"))
				{
          Flag_all.CEND_Flag = 1;
					pStart =(u8 *) strstr((const char*)(usart3_Rx_buffer), "^CEND:");
					pStart =(u8 *) strstr((const char*)(pStart), ",");
					i =0 ;
					while((*(pStart+1+i)) !=',')
					{
            if(i==0)
						{
							if((*(pStart+1+i)>=0x30)&&(*(pStart+1+i) <= 0x39))
							{
								CEND_time = *(pStart+1+i)-0x30;
							}
						}	
						else
						{
							if((*(pStart+1+i)>=0x30)&&(*(pStart+1+i) <= 0x39))
								 CEND_time=CEND_time*10+(*(pStart+1+i)-0x30);
						}						
						i++;
          }
        }
				//NO CARRIER
				if(NULL!=strstr((const char*)(usart3_Rx_buffer), "NO CARRIER")) //Í¨»°½áÊøºó£¬²»ÂÛÊÇ·ñ½ÓÍ¨£¬¶¼»áÉÏ±¨NO CARRIER»òNO ANSWER
				{                                                                //NO ANSWER:²¦ºÅ³¬Ê±£¬»¹Ã»ÓÐ½ÓÍ¨
           Flag_all.NO_CARRIER = 1;

        } 
         //NO ANSWER				
        if(NULL!=strstr((const char*)(usart3_Rx_buffer), "NO CARRIER")) //Í¨»°½áÊøºó£¬²»ÂÛÊÇ·ñ½ÓÍ¨£¬¶¼»áÉÏ±¨NO CARRIER»òNO ANSWER
				{                                                                //NO ANSWER:²¦ºÅ³¬Ê±£¬»¹Ã»ÓÐ½ÓÍ¨
           Flag_all.NO_ANSWER = 1;

        }		
      }
    }
		if(Flag_all.GM301_CPMS) //¶ÌÐÅµÄ´¦Àí³ÌÐò
		{
      if((usart3_Rx_length>41)&&(usart3_Rx_buffer[usart3_Rx_length-2]=='\r')&&(usart3_Rx_buffer[usart3_Rx_length-1]=='\n'))
			{
				if((NULL!=strstr((const char*)(usart3_Rx_buffer), "+CPMS:")))
				{
					Flag_all.GM301_CPMS = 0;
         // Flag_all.SM_FLAG = HAVE_SM;
					memcpy(usart3_Rx_SM,usart3_Rx_buffer,usart3_Rx_length);
					usart3_Rx_SM_len = usart3_Rx_length;
					//pStart = strstr((const char*)(usart3_Rx_buffer), "NO CARRIER");
					//pStart = (u8 *)memchr((const char*)usart3_Rx_buffer,',',usart3_Rx_length);
					//pEnd = (u8 *)memchr((const char*)(pStart+1),',',usart3_Rx_length);
        }
			}

    }
		//+CMGR: "REC READ","106553500",,"14/11/06,15:26:27"
		//+CMGR: "REC UNREAD","18356957828",,"14/11/08,14:54:58"
    //SOBDPID*BC*00*00*00*00*00*00*00*00*00*00*00*00#
		//OK
		if(Flag_all.GM301_CMGR) //¶ÌÐÅµÄ´¦Àí³ÌÐòGM301_CMGR
		{
      if((usart3_Rx_length>6)&&(usart3_Rx_buffer[usart3_Rx_length-2]=='\r')&&(usart3_Rx_buffer[usart3_Rx_length-1]=='\n'))
			{				
				if(Flag_all.GM301_CMGR_UNread)
				{  //S    O    B     D    P    I   D    A    *    1    #    \r   \n  Ç°ÃæÃ»ÓÐ»Ø³µ»»ÐÐ·û
					 //0x53 0x4F 0x42 0x44 0x50 0x49 0x44 0x41 0x2A 0x31 0x23 0x0D 0x0A
					Flag_all.GM301_CMGR = 0;
        //  pStart = (u8 *)strstr((const char*)(usart3_Rx_buffer), "\r\n");
				//	pEnd = (u8 *)strstr((const char*)(pStart+1), "\r\n");
				//	if(pEnd > pStart)   
				//		usart3_Rx_SM_len = pEnd-pStart-2;
				////	else 
				//		return ;
					//if(usart3_Rx_SM_len==usart3_Rx_length-2) 
          usart3_Rx_SM_len=usart3_Rx_length-2	;				
					memcpy(usart3_Rx_SM,usart3_Rx_buffer,usart3_Rx_length-2);
				//	else 
						return ;

        }
				if((NULL!=strstr((const char*)(usart3_Rx_buffer), "+CMGR:")))
				{
          if((NULL!=strstr((const char*)(usart3_Rx_buffer), "\"REC UNREAD\"")))  //ÅÐ¶Ï¶ÌÐÅÎ´¶Á
					{
						Flag_all.GM301_CMGR_UNread = 1; //¶ÌÐÅÎ´¶Á
            memset(usart3_Rx_buffer,0x00,usart3_Rx_length);	
            usart3_Rx_length = 0;			
            return ;						
          }		
          else if((NULL!=strstr((const char*)(usart3_Rx_buffer), "\"REC READ\""))) //ÅÐ¶Ï¶ÌÐÅÒÑ¶Á
          {            
						Flag_all.GM301_CMGR_UNread = 0; //¶ÌÐÅÒÑ¶Á
						Flag_all.GM301_CMGR = 0;
            memset(usart3_Rx_buffer,0x00,usart3_Rx_length);	
						usart3_Rx_length = 0;
						return ;
          }
					//pStart = strstr((const char*)(usart3_Rx_buffer), "NO CARRIER");
					//pStart = (u8 *)memchr((const char*)usart3_Rx_buffer,',',usart3_Rx_length);
					//pEnd = (u8 *)memchr((const char*)(pStart+1),',',usart3_Rx_length);
        }
				
			}
    }
		
//		else if((usart3_Rx_length>8)&&(usart3_Rx_buffer[usart3_Rx_length-2]=='\r')&&(usart3_Rx_buffer[usart3_Rx_length-1]=='\n'))
//		{
///      memset(usart3_Rx_buffer,0x00,140);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
//			usart3_Rx_length = 0;
//    }
/***Í¨»°½áÊø****************/
		if(usart3_Rx_length >140) //½ÓÊÕµ½Êý¾Ý³¬¹ý140£¬Êý×é¾ÍÒç³öÁË£¬
		{
       memset(usart3_Rx_buffer,0x00,140);//Çå¿Õ´®¿Ú3µÄ½ÓÊÕÊý×Ö
			 usart3_Rx_length = 0;
			 return ;
    }		
  } 
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)  //·¢ËÍÖÐ¶Ï£»ÅÐ¶ÏµÄÊÇTXE¶ø²»ÊÇTC
	{		
		if (sendlengthU3 >= 1)
		{
			//while((USART1->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
      USART3->DR = (u8) (*(psendU3++));
			sendlengthU3--;
    }
		else
		{
      USART_ITConfig(USART3, USART_IT_TXE, DISABLE); //¹Ø±Õ·¢ËÍÖÐ¶Ï
			return ;
    }		
	}	
} 
/*****´®¿Ú½ÓÊÕ²¿·Ö ½áÊø***********************/

//·¢ËÍ³ÌÐò
//ÊäÈë£º
//Êä³ö£º
volatile u8 sendlengthU1 = 0;
u8 * psendU1;
void Usart1_SendData(u8 * psend, u8 length)
{
	sendlengthU1 = length;
	psendU1 = psend;
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //¿ªÆô·¢ËÍÖÐ¶Ï
	  if(sendlengthU1 >= 1)
		{
			sendlengthU1--; //Õâ²½ÒªÔÚ¿ªÆô·¢ËÍÖÐ¶ÏÖ®Ç°Íê³É£¬·ñÔòÖÐ¶ÏÒ»¿ªÆôÕâ²½Ö´ÐÐÂß¼­¾Í´íÁË
			USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //¿ªÆô·¢ËÍÖÐ¶Ï
      while((USART1->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
      USART1->DR = (u8) (*(psendU1++));	
    }
		else
		{
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE); //¿ªÆô·¢ËÍÖÐ¶Ï
			return ;
    }
}

volatile u8 sendlengthU2 = 0;
u8 * psendU2;
void Usart2_SendData(u8 * psend, u8 length)
{
	sendlengthU2 = length;
	psendU2 = psend;
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //¿ªÆô·¢ËÍÖÐ¶Ï
	  if(sendlengthU2 >= 1)
		{
			sendlengthU2--; //Õâ²½ÒªÔÚ¿ªÆô·¢ËÍÖÐ¶ÏÖ®Ç°Íê³É£¬·ñÔòÖÐ¶ÏÒ»¿ªÆôÕâ²½Ö´ÐÐÂß¼­¾Í´íÁË
			USART_ITConfig(USART2, USART_IT_TXE, ENABLE); //¿ªÆô·¢ËÍÖÐ¶Ï
      while((USART2->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
      USART2->DR = (u8) (*(psendU2++));	
    }
		else
		{
      USART_ITConfig(USART2, USART_IT_TXE, DISABLE); //¿ªÆô·¢ËÍÖÐ¶Ï
			return ;
    }
}

volatile u8 sendlengthU3 = 0;
u8 * psendU3;
void Usart3_SendData(u8 * psend, u8 length)
{
	sendlengthU3 = length;
	psendU3 = psend;
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //¿ªÆô·¢ËÍÖÐ¶Ï
	  if(sendlengthU3 >= 1)
		{
			sendlengthU3--; //Õâ²½ÒªÔÚ¿ªÆô·¢ËÍÖÐ¶ÏÖ®Ç°Íê³É£¬·ñÔòÖÐ¶ÏÒ»¿ªÆôÕâ²½Ö´ÐÐÂß¼­¾Í´íÁË
			USART_ITConfig(USART3, USART_IT_TXE, ENABLE); //¿ªÆô·¢ËÍÖÐ¶Ï
      while((USART3->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
      USART3->DR = (u8) (*(psendU3++));	
    }
		else
		{
      USART_ITConfig(USART3, USART_IT_TXE, DISABLE); //¿ªÆô·¢ËÍÖÐ¶Ï
			return ;
    }
}

/****GPSÊý¾Ý´íÎóºóÇå¿Õ±êÖ¾**************************/
void GPS_Flag_Clear (void)
{
  Flag_all.GPSreceiv_eorr = 1 ;
	Flag_all.GPSreceiv_complete = 0;
	Flag_all.GPSdeal_complete = 0;//GPSÊý¾Ý´¦ÀíÍê³É
	memset(usart2_Rx_buffer,0x00,usart2_Rx_length);
	usart2_Rx_length = 0 ;
	Flag_all.GPSHead = 0; 
	Flag_all.GPSreceiv_complete = 0;
	memset(GPS_DATA,0x00,20);  //ÓÃÀ´²âÊÔÓÃµÄ£¬ÕýÊ½³ÌÐòÒª×¢ÊÍµô
}
/******************************/
/**********************************
GPS_DATA[20]
Î³¶È       ¾­¶È        ¸ß³Ì      ËÙ¶È      ·½Ïò       UTCÊ±¼ä
4×Ö½Ú      4×Ö½Ú      2×Ö½Ú      2×Ö½Ú     2×Ö½Ú      6×Ö½Ú
0--3       4--7       8--9        10--11   12--13    14--19

**********************************/
u8 GPS_DATA[20] ={0}; //ÓÃÀ´´æ·ÅÕýÔÚ½âÎöµÄÊý¾Ý
u8 GPS_DATA_BACKUP[20] ={0};//ÓÃÀ´±¸·Ý½âÎöºÃµÄÊý¾Ý 
u8 change_gpsdata(void)
{
  volatile u8 ucTen, ucOne;
	 u8* pTempBuf;
	 u8* pCommard;
	volatile u8 GPS_COMMARD = 0;
	volatile u8 temparry[40] = {0}; //ÁÙÊ±´æ·ÅgpsÊý¾Ý
	volatile u32 data_change = 0;
  u32 data_change1 = 0;
  u8 i = 0 ,j=0;
	if(!Flag_all.GPSreceiv_complete)
		return 0;
	
	if(Flag_all.GPSreceiv_complete)
	{
    // usart2_Rx_buffer[usart2_Rx_length++]
		pTempBuf = usart2_Rx_buffer;
		ucOne = 0x00;
		do			//´Ëdo...while½á¹¹ÖÐÕÒ³ö$GPRMCÖÐµÄUTCÊ±¼äºÍÄêÔÂÈÕÊ±¼ä
	  {	
		  pCommard = (u8*)strchr((const char*)pTempBuf, ',');
	    ucTen = pCommard - pTempBuf;
	    if (0x01 == ucOne) //È¡GPSµÄUTCÊ±¼ä£¨Ê±·ÖÃë£© HHMMSS.SSS
	    {
	    	if ((ucTen >= 0x03) && (ucTen <= 0x0A))
	    	{
	    		if (ucTen >= 0x06)
	    		{
	    			i =0;
						j=0;
					  memset((u8 *)temparry,0x00,40);  //
            memcpy((u8 *)temparry,pTempBuf,0x06);
						i=((temparry[0]&0x0f)*10+(temparry[1]&0x0f)+8)%24;
						j=i/10;  //×ª³ÉBCDÂë£¬µÃµ½Ê®Î»
						i=i-j*10;//×ª³ÉBCDÂë£¬µÃµ½¸öÎ»
						GPS_DATA[UTC_GPS+3]=((j<<4)|i);
						GPS_DATA[UTC_GPS+4]=((temparry[2]&0x0f)<<4)|(temparry[3]&0x0f);
						GPS_DATA[UTC_GPS+5]=((temparry[4]&0x0f)<<4)|(temparry[5]&0x0f);
						data_change1 = 0;
						//memcpy(GPS_DATA+UTC_GPS, pTempBuf, 0x06);		//È¡GPSµÄUTCÊ±¼ä£¨Ê±·ÖÃë£©
	    		}
	    		else
	    		{
	    			GPS_Flag_Clear();
						//memcpy(GPS_DATA+UTC_GPS, pTempBuf, ucTen);
						return 0;
	    		}	
	    	}
	    	else
	    	{
	    		GPS_Flag_Clear();
					return 0;//Ê±¼ä½ÓÊÕ´íÎó
	    	}
	    }
	    else if (0x02 == ucOne) //»ñÈ¡GPSÊ±¼äÊÇ·ñÓÐÐ§µÄ±êÖ¾£¬£¨A--ÓÐÐ§£¬V--ÎÞÐ§£©
	    {
            GPS_COMMARD = *pTempBuf;		//»ñÈ¡GPSÊ±¼äÊÇ·ñÓÐÐ§µÄ±êÖ¾£¬£¨A--ÓÐÐ§£¬V--ÎÞÐ§£©
	          if(GPS_COMMARD =='V')
						{
               GPS_Flag_Clear();					
							 GPIO_ResetBits(GPIOC,GPIO_Pin_6);  //GPSµÆÃð
					     return 0;//Ê±¼ä½ÓÊÜ´íÎó	
            }
						else if(GPS_COMMARD =='A')
						{
               GPIO_SetBits(GPIOC,GPIO_Pin_6);  //GPSµÆÁÁ
            }
      }
			else if(0x03==ucOne)//ÌáÈ¡Î³¶ÈÐÅÏ¢
			{
        if(ucTen ==9) //Î³¶ÈÓÐ9¸ö×Ö·û
				{//ddmm.mmmm
					data_change = 0;
					 memset((u8 *)temparry,0x00,40);  //
           memcpy((u8 *)temparry,pTempBuf,ucTen);
					 //GPS_DATA[latitude_GPS]=(((temparry[0]&0x0f)<<4)|(temparry[1]&0x0f)); //DDmm.mmmm	
					data_change=((u32)(((temparry[0]&0x0f)*10)+(temparry[1]&0x0f))); //DDmm.mmmm
					data_change =(((temparry[2]&0x0f)*1000000+(temparry[3]&0x0f)*100000+(temparry[5]&0x0f)*10000+(temparry[6]&0x0f)*1000+(temparry[7]&0x0f)*100+(temparry[8]&0x0f)*10)/6)+data_change*1000000;
					 GPS_DATA[latitude_GPS]=data_change >> 24; //DDmm.mmmm
					GPS_DATA[latitude_GPS+1]=(data_change >> 16)&0x000000ff;//ddMM.mmmm   
					 GPS_DATA[latitude_GPS+2]=(data_change >> 8)&0x000000ff;//ddmm.MMmm  Ìø¹ýÐ¡Êýµã
					 GPS_DATA[latitude_GPS+3]=data_change &0x000000ff;//ddmm.mmMM
					//data_change =((516889*100)/60);
					//data_change1 =shiliutoshi(data_change);
					//shiliutoshi(data_change,GPS_DATA+latitude_GPS+1);
					
        }					
				else
				{
           GPS_Flag_Clear();
					return 0;//Î³¶ÈÌáÈ¡´íÎó
        }
						
      }
			else if(0x05==ucOne)//ÌáÈ¡¾­¶ÈÐÅÏ¢
			{
        if(ucTen ==10) //Î³¶ÈÓÐ9¸ö×Ö·û
				{//dddmm.mmmm
					data_change = 0;  //
          memset((u8 *)temparry,0x00,40);
					memcpy((u8 *)temparry,pTempBuf,ucTen);
					 //GPS_DATA[latitude_GPS]=(((temparry[0]&0x0f)<<4)|(temparry[1]&0x0f)); //DDmm.mmmm
          //data_change	=((u32)(((temparry[0]&0x0f)*100)+(temparry[1]&0x0f)*10+(temparry[2]&0x0f)));	
					data_change	=((temparry[0]&0x0f)*100)+((temparry[1]&0x0f)*10)+(temparry[2]&0x0f);	 //×¢ÒâÀ¨ºÅµÄÔËÓÃ					
					data_change =(((temparry[3]&0x0f)*1000000+(temparry[4]&0x0f)*100000+(temparry[6]&0x0f)*10000+(temparry[7]&0x0f)*1000+(temparry[8]&0x0f)*100+(temparry[9]&0x0f)*10)/6)+data_change*1000000;
					GPS_DATA[longitude_GPS]=data_change >> 24; //DDmm.mmmm
					GPS_DATA[longitude_GPS+1]=(data_change >> 16)&0x000000ff;//ddMM.mmmm   
					 GPS_DATA[longitude_GPS+2]=(data_change >> 8)&0x000000ff;//ddmm.MMmm  Ìø¹ýÐ¡Êýµã
					 GPS_DATA[longitude_GPS+3]=data_change &0x000000ff;//ddmm.mmMM
					//data_change =((516889*100)/60);
					//data_change1 =shiliutoshi(data_change);
					//shiliutoshi(data_change,GPS_DATA+latitude_GPS+1);
        }					
				else
				{
           GPS_Flag_Clear();
					return 0;//Ê±¼ä½ÓÊÕ´íÎó
        }						
      }
			else if(0x07==ucOne)//ÌáÈ¡ËÙ¶ÈÐÅÏ¢£¬20140618°ì¹«ÊÒÎÞ·¨µÃµ½ËÙ¶ÈÖµ£¬´ý²âÊÔ
			{
        if(ucTen > 0) //ÓÐËÙ¶ÈÖµ£¬ ËÙ¶ÈÖµÓÃ¶àÉÙ¸ö×Ö½Ú±íÊ¾²»È·¶¨¡£
				{//dddmm.mmmm
					i = 0;
					data_change = 0;  //
          memset((u8 *)temparry,0x00,40);
					memcpy((u8 *)temparry,pTempBuf,ucTen);
					while(i < ucTen )                  //????
          {
            if(isdigit(temparry[i]))
            {
              data_change *= 10;
              data_change += temparry[i]&0x0f;
            }
            i++;
          }
					data_change = (data_change*1852)/10000;					
					GPS_DATA[velocity_GPS]=(data_change >> 8)&0x000000ff; //DDmm.mmmm					
					GPS_DATA[velocity_GPS+1]=data_change &0x000000ff;//ddmm.mmMM
					//data_change1 = 0;
        }					
				else
				{
           GPS_Flag_Clear();
					return 0;//Ê±¼ä½ÓÊÕ´íÎó
        }					
      }
			else if(0x08==ucOne)//ÌáÈ¡ËÙ¶È·½Ïò
			{
        if((ucTen > 3)&&(ucTen <7)) //ÓÐËÙ¶ÈÖµ£¬ ËÙ¶ÈÖµÓÃ¶àÉÙ¸ö×Ö½Ú±íÊ¾²»È·¶¨¡£
				{//dddmm.mmmm
					i = 0;
					data_change = 0;  //
          memset((u8 *)temparry,0x00,40);
					memcpy((u8 *)temparry,pTempBuf,ucTen);
					while(temparry[i] !='.' )                  //????
          {
            if(isdigit(temparry[i]))
            {
              data_change *= 10;
              data_change += temparry[i]&0x0f;
            }
            i++;
          }					
					GPS_DATA[direction_GPS]=(data_change >> 8)&0x000000ff; //DDmm.mmmm					
					GPS_DATA[direction_GPS+1]=data_change &0x000000ff;//ddmm.mmMM
					//data_change1 = 0;
        }					
				else
				{
           GPS_Flag_Clear();
					return 0;//Ê±¼ä½ÓÊÕ´íÎó
        }				
      } 
			
	    else if (0x09 == ucOne)		//»ñÈ¡GPSÄêÔÂÈÕ
	    {
	    	if ((ucTen >= 0x03) && (ucTen <= 0x06))
	    	{
	    		memset((u8 *)temparry,0x00,40);
					memcpy((u8 *)temparry,pTempBuf,ucTen);
					GPS_DATA[UTC_GPS]=((temparry[4]&0x0f)<<4)|(temparry[5]&0x0f);
					GPS_DATA[UTC_GPS+1]=((temparry[2]&0x0f)<<4)|(temparry[3]&0x0f);
					GPS_DATA[UTC_GPS+2]=((temparry[0]&0x0f)<<4)|(temparry[1]&0x0f);
					data_change1 = 0;
					//memcpy(GPS_YMD, pTempBuf, ucTen);
	    	}
	    	else
	    	{
           GPS_Flag_Clear();
					return 0;//Ê±¼ä½ÓÊÕ´íÎó
        }		
	    	break;
	    }
		  pTempBuf = pCommard + 1;
		  ucOne++;
	  }while(1);
	  for(ucOne=0;ucOne<20;ucOne++) //ÔÚwhileºóÃæ£¬ËµÃ÷ÕýÈ·´¦ÀíÍê³ÉGPSÊý¾Ý¡£
    {
	    GPS_DATA_BACKUP[ucOne]=	GPS_DATA[ucOne]; //½«´¦ÀíºÃµÄGPSÊý¾Ý·Åµ½±¸·ÝÊý×éÀïÃæÈ¥
    }
	  return 1;	
  }
	return 0;
}

u32 shiliutoshi(u32 ret,u8 *p)
{
   
 unsigned char ret6=0,ret5=0,ret4=0,ret3=0,ret2=0,ret1=0;//

    ret6=ret/100000;                   //eg:ret=0xD2529=861481,Ö±½ÓÓÃÊ®½øÖÆÔËËãµÃµ½¸ßÎ»8£¬ÔÙµÃµ½6Ò»´ÎÀàÍÆ
    //ret=ret<<4;        	
	  ret5=ret/10000-ret6*10;                   //
    //ret=ret<<4;        	
	  ret4=ret/1000-(ret/10000)*10;                   //
    //ret=ret<<4;                       //
    ret3=ret/100-(ret/1000)*10;                  //
    //ret=ret<<4;
    ret2=ret/10-(ret/100)*10;                 //
    //ret=ret<<4;
    ret1=ret-(ret/10)*10;                //
    //ret=ret1+ret2*16+ret3*16*16+ret4*16*16*16+ret5*16*16*16*16 ;//+ret6*16*16*16*16*16;//
    *p = (ret6<<4)|ret5;
	  *(p+1) = (ret4<<4)|ret3;
	  *(p+2) = (ret2<<4)|ret1;
        return(ret);
}


void sirf4Config(void)
{
    wait(100);
  	Usart2_SendData("$PSRF103,00,00,00,01*24\r\n",25);wait(100);
  	Usart2_SendData("$PSRF103,01,00,00,01*25\r\n",25);wait(100);
  	Usart2_SendData("$PSRF103,02,00,00,01*26\r\n",25);wait(100);
  	Usart2_SendData("$PSRF103,03,00,00,01*27\r\n",25);wait(100);
  	Usart2_SendData("$PSRF103,05,00,00,01*21\r\n",25);wait(100);
  	Usart2_SendData("$PSRF103,06,00,00,01*22\r\n",25);wait(100);  
 		
}

