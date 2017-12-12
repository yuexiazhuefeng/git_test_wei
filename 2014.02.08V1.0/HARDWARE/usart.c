#include "usart.h"

//const u8 GPS_Head[6]={'$','G','P','R','M','C'};
u8 usart1_Rx_buffer[166] ={0}; //OBD���ݣ�����Ƕ�PID��������ÿ������32��������λ32*5+6=166��ԶС��255
u8 usart1_Rx_length = 0;
u8 OBD_Package_len = 0;
u8 usart1_Rx_backup[50] ={0}; //�������������Ŵ���3�����е����ݣ��Ա����洦���á�
u8 USART1_Rx_timer = 0;//��ʱ30ms��������մ���1���ջ��棬��ֹ���ţ���һЩ����Ҫ�ķ�������

u8 usart2_Rx_buffer[90] ={0};//GPS�������ֻ��90��
u8 usart2_Rx_length = 0;
u8 USART2_Rx_timer = 0;//��ʱ30ms��������մ���2���ջ��棬��ֹ���ţ���һЩ����Ҫ�ķ������

u8 usart3_Rx_buffer[1100] ={0}; //GSM�����ݶ�ֻ��140����
u16 usart3_Rx_length = 0;
u8 usart3_Rx_backup[1100] ={0}; //�������������Ŵ���3�����е����ݣ��Ա����洦���á�
u8 usart3_Rx_SM[150] = {0};  //���Ż���
u8 usart3_Rx_SM_len = 0;     //���ն��ų���
u16 usart3_Rx_backup_length = 0;
u8 USART3_Rx_timer = 0;//��ʱ30ms��������մ���3���ջ��棬��ֹ���ţ���һЩ����Ҫ�ķ�������
u8 u8_0x7e[1]={0x7e};
/*****���ڳ�ʼ������***********************/
//��ʼ��IO ����1 
//bound:������
void uart1_init(u32 bound)
{
    //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART1);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������; �������������ԭ�����¶����е�������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

   //Usart1 NVIC ����

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}

//��ʼ��IO ����2
//bound:������4800��GPS
void uart2_init(u32 bound)
{
    //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART1��GPIOAʱ��
	
 	USART_DeInit(USART2);  //��λ����1
	 //USART1_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA3
   
    //USART1_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3

   //Usart2 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART2 ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;  // ��GPSͨ�ţ�4800
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

}

//��ʼ��IO ����3 
//bound:������
void uart3_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART1��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART3);  //��λ����3
	 //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PB10
   
    //USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PA10

   //Usart3 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART3 ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

}

/*****���ڳ�ʼ������ ����***********************/

/*****���ڽ��ղ��� ***********************/
u8 test_usart1_number = 0;
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;	   
	u8 i ;
	u8 checksum; //��У�飬���������ݰ��е�У������Ƚ�

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{		
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
    if(USART1_Rx_timer >=2 )
		{
      memset(usart1_Rx_buffer,0x00,usart1_Rx_length);
			usart1_Rx_length = 0 ;
    }
		USART1_Rx_timer = 0;
		usart1_Rx_buffer[usart1_Rx_length++] = Res;
		if(!OBD_Com_Flag.OBD_Head) //��ȡOBD����ͷ 0x55 0xAA
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
				for(i=0;i<(usart1_Rx_length-3);i++)  //�����У���ֵ���������������У�������Ա�
				  checksum = checksum+usart1_Rx_buffer[2+i];
				if(checksum == usart1_Rx_buffer[usart1_Rx_length-1])
				{
					if(usart1_Rx_buffer[4]==0xC2)  //PID������
					{
            for(i=0;i<OBD_Package_len-2;i++) //�����ļ���λҲȥ��
						{
              if(ring_obd_buffer.count > ring_obd_buffer.size)
								ring_obd_buffer.count= 0;
							RING_PUTC(&ring_obd_buffer, usart1_Rx_buffer[5+i])
            }
						OBD_Com_Flag.OBD_Received_PID = 1;
						return ;
          }
					else //��PID�������Ĵ�����
					{
            memset(usart1_Rx_backup,0x00,50);
						memcpy(usart1_Rx_backup,usart1_Rx_buffer,usart1_Rx_length);//У�����֤��ɺ󣬰��յ���OBD���ݱ��浽������������ 
						memset(usart1_Rx_buffer,0x00,usart1_Rx_length);
						usart1_Rx_length = 0;
          }          
					if(usart1_Rx_backup[4]==0xC4)  //�յ�����״̬��Ϣ
					{//����״̬��Ϣ55 AA 00 0A C4 00 01 00 00 00 00 00 00 CF
            OBD_Com_Flag.OBD_Received_Body = 1;
						Body_state_deal();
          }
						 
					else if(usart1_Rx_backup[4]==0x7F) //�յ�Ӧ�����
						 OBD_Com_Flag.OBD_Error =1;
					else
					 OBD_Com_Flag.OBD_Received_Complete =1;  //�������
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

	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)  //�����жϣ��жϵ���TXE������TC
	{
		if (sendlengthU1 >= 1)
		{
			//while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
      USART1->DR = (u8) (*(psendU1++));
			sendlengthU1--;
    }
		else
		{
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE); //�رշ����ж�
			return ;
    }
		
	}	
} 


void USART2_IRQHandler(void)   //����2�жϷ������ GPS���ݿ�   ���ô�ӡ��
{
	u8 Res;
  static u8 gps_AV =0;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		//IWDG_Feed();
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������
		usart2_Rx_buffer[usart2_Rx_length++] = Res;
		if((!Flag_all.GPSHead)&&(!Flag_all.GPSreceiv_complete)) //�ж�$GPRMCͷ
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
       
				if (('$' == usart2_Rx_buffer[0x00]) && (0x0A == usart2_Rx_buffer[usart2_Rx_length - 1]))	//��ȷ������GPS����
				{					
					if(usart2_Rx_buffer[18]=='A') //���յ�������Ч
					{
            Flag_all.GPSreceiv_complete = 1; //����GPS������ɱ�־����־Ҫ����change_gpsdata()
						gps_AV = 0; 
						GPS_LED_ON;//���յ���Ч��GPS���ݣ������õ���						
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
							 GPS_LED_OFF;  //���յ���GPS���ݣ���Ч
							 //PCout(6)=0;
             }
						  return ;
          }
					if(Flag_all.GPSreceiv_complete)
					{
              if(change_gpsdata())
							{
                 Flag_all.GPSdeal_complete = 1;//GPS���ݴ������
								 memset(usart2_Rx_buffer,0x00,usart2_Rx_length);
					       usart2_Rx_length = 0 ;
					       Flag_all.GPSHead = 0; 
				         Flag_all.GPSreceiv_complete = 0;
								 memset(GPS_DATA,0x00,20);  //���������õģ���ʽ����Ҫע�͵�
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
  if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)  //�����жϣ��жϵ���TXE������TC
	{
		
		if (sendlengthU2 >= 1)
		{
			//while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
      USART2->DR = (u8) (*(psendU2++));
			sendlengthU2--;
    }
		else
		{
      USART_ITConfig(USART2, USART_IT_TXE, DISABLE); //�رշ����ж�
			return ;
    }
		
	}		
}

//ƽ̨���͵����ݳ���1024���ֽھͻ���зְ�������GSM�İ�װ�ֽڣ������ջ�����Ϊ1100���ֽ�
//����GSM�İ�װ�ֽڣ���
void USART3_IRQHandler(void) //����3�жϷ������  GSM�Ͳ������ý���
{
	u8 Res;	  
	u8 i = 0;
  u8* pStart;	
  u8* pEnd;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������
		if(USART3_Rx_timer >=10)
		{
      USART3_Rx_timer = 0;
			memset(usart3_Rx_buffer,0x00,140);//��մ���3�Ľ�������
			usart3_Rx_length = 0;
    }
		USART3_Rx_timer = 0;
		usart3_Rx_buffer[usart3_Rx_length++]=Res;    
/****���ò���*   */ 
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
						 memset(usart3_Rx_buffer,0x00,140);//��մ���3�Ľ�������
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
/*******���OK*******/
//\r \n O K \r \n
//0  1  2 3 4  5
		
    if(((!Flag_all.OK)&&(Res == 'K')&&(usart3_Rx_length >2))||(Flag_all.OK_O))
    {
	    if((usart3_Rx_length>2)&&(usart3_Rx_buffer[usart3_Rx_length-2]=='O'))
			{	
				Flag_all.OK_O = 1;
				
			 // memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//��մ���3�Ľ�������
			  //usart3_Rx_length = 0;
			}
			if((Flag_all.OK_O)&&(usart3_Rx_buffer[usart3_Rx_length-2]=='\r')&&(usart3_Rx_buffer[usart3_Rx_length-1]=='\n'))
			{
        Flag_all.OK_O = 0;
				Flag_all.OK = 1;
				memset(usart3_Rx_backup,0x00,140);
				memcpy(usart3_Rx_backup,usart3_Rx_buffer,usart3_Rx_length);
				usart3_Rx_backup_length = usart3_Rx_length;
				memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//��մ���3�Ľ�������
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
          memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//��մ���3�Ľ�������
			    usart3_Rx_length = 0;
        }
				
      }		
    }
		
		
//		if((Flag_all.OK))
			
/*********���OK***********************/
/*********���SISW***********************/	
//\r \n ^  S  I  S  W  :     0  ,  1  0  ,  0 \r \n
//0D 0A 5E 53 49 53 57 3A 20 30 2C 31 30 2C 30 0D 0A	
		
	  if((!Flag_all.SISW)&&(usart3_Rx_buffer[6]=='W')&&(usart3_Rx_buffer[usart3_Rx_length-2]=='\r')&&(usart3_Rx_buffer[usart3_Rx_length-1]=='\n'))
    {
	    if((usart3_Rx_buffer[2]=='^')&&(usart3_Rx_buffer[3]=='S')&&(usart3_Rx_buffer[4]=='I')&&(usart3_Rx_buffer[5]=='S'))
			{	
				if(usart3_Rx_length <=14)
				{
           memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//��մ���3�Ľ�������
			     usart3_Rx_length = 0;
        }
				else
				{
          Flag_all.SISW = 1;
				  memset(usart3_Rx_backup,0x00,140);
				  memcpy(usart3_Rx_backup,usart3_Rx_buffer,usart3_Rx_length);
			    memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//��մ���3�Ľ�������
			    usart3_Rx_length = 0;
        }			
			}		
	  }
/*********���SISW***********************/		
		
/*********���SISR***Start********************/			
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
				{                                      //GSM����û������
          Flag_all.haveGSMdata =SISR_7E_EMPTY; //����Ϊ��
					return;
        }
				if(NULL!=strstr((const char*)(usart3_Rx_buffer), "\r\n^SISR: 0, 1\r\n"))
				{                                        //GSM�����������ݣ����ȡ�� �����ϱ��ġ�
          memset(usart3_Rx_buffer,0x00,140);  //��մ���3�Ľ�������
			    usart3_Rx_length = 0;	 
					Flag_all.haveGSMdata = SISR_1 ;    //���������ݣ�����ȡ
					return;
        }
			#endif //MG323
				if(NULL!=strstr((const char*)(usart3_Rx_buffer), "\r\n^SISR: 0,0\r\n"))
				{                                      //GSM����û������
          Flag_all.haveGSMdata =SISR_7E_EMPTY; //����Ϊ��
					return;
        }
				if(NULL!=strstr((const char*)(usart3_Rx_buffer), "\r\n^SISR: 0,1\r\n"))
				{                                        //GSM�����������ݣ����ȡ�� �����ϱ��ġ�
          memset(usart3_Rx_buffer,0x00,140);  //��մ���3�Ľ�������
			    usart3_Rx_length = 0;	 
					Flag_all.haveGSMdata = SISR_1 ;    //���������ݣ�����ȡ
					return;
        }
					
			//	else 
				//	Flag_all.haveGSMdata =0;
				//i =0;
//MG323				while(usart3_Rx_buffer[11+i]!=',')
				while((usart3_Rx_buffer[11+i]!='\r')&&(usart3_Rx_buffer[11+i+1]!='\n'))
				{                                      //��ȡ���յ������ݳ�����Ϣ�3λ					
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
				if((GSM_Reciev_len+18) >= usart3_Rx_length)  //16//û���������ݣ�����ʹ�ý��ճ������ж��Ƿ����������ֹ���������ڲ���0A0D��
				{ 
           return;
        } 
				
				pStart = (u8 *)memchr((const char*)usart3_Rx_buffer,0x7E,usart3_Rx_length); //�ҵ�0x7e
				pEnd =(u8 *)memchr((const char *)(pStart+1),0x7e,GSM_Reciev_len+1);
			//	pEnd = (u8 *)strstr((const char*)(pStart+1), (const char *)u8_0x7e);
				if(GSM_Reciev_len!=((pEnd - pStart)+1))
				{
          memset(usart3_Rx_buffer,0x00,140);//��մ���3�Ľ�������
			    usart3_Rx_length = 0;	
          return ;
        }
		  	
				if(Flag_all.haveGSMdata != SISR_7E_OK)
				{
          memset(GSM_Reciev,0x00,140);
				  memcpy(GSM_Reciev,(pStart),GSM_Reciev_len);
			    memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//��մ���3�Ľ�������
			    usart3_Rx_length = 0;
					//Flag_all.SISR = 1; // ��7E�����账��
				  Flag_all.haveGSMdata =SISR_7E_OK;  // ��7E�����账��					
        }
				else //���������ڴ���ֱ�Ӷ������ڽ��ܵ��ģ��Ժ�ῼ����Ϣ���еķ�ʽ��������������Ϣ�Ŷӡ�
				{
          memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//��մ���3�Ľ�������
			    usart3_Rx_length = 0;
        }
			}
	  }
/*********���SISR***End********************/		
/********����****************************************************************/		
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
				//{//���ʱ�򴮿ڵ������Ѿ���������ˣ�
					if(NULL!=strstr((const char*)(usart3_Rx_buffer), "+CLIP:"))				
					{
						GSM_Ring_Number_len = 0;
						pEnd = (u8 *)strstr((const char*)(usart3_Rx_buffer+8), "+CLIP");
						pStart = (u8 *)strstr((const char*)pEnd, "\"");
						pEnd = (u8 *)strstr((const char*)(pStart+1), "\"");
						GSM_Ring_Number_len = pEnd-pStart-1;
						if((GSM_Ring_Number_len<5)||(GSM_Ring_Number_len >15))//�ж�������ʾ�ĵ绰���볤���Ƿ���̻����
						{
							memset(usart3_Rx_buffer,0x00,140);//��մ���3�Ľ�������
							usart3_Rx_length = 0;	
							return ;					
						}
						Flag_all.RING_S = 0;
						Flag_all.RING = 1;
						memset(GSM_Ring_Number,0x00,15);
						memcpy(GSM_Ring_Number,(pStart+1),GSM_Ring_Number_len);
						memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//��մ���3�Ľ�������
						usart3_Rx_length = 0;
				  }
				  else
				  {
					  memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//��մ���3�Ľ�������
					  usart3_Rx_length = 0;
						Flag_all.RING_S = 0;
				  }
				//}
				 if(usart3_Rx_length >50)//else
				{
					 memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//��մ���3�Ľ�������
					 usart3_Rx_length = 0;
					 Flag_all.RING_S = 0;
				}
			}
    }
/********����****************************************************************/		
/**ͨ������**********************/
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
					memset(usart3_Rx_buffer,0x00,usart3_Rx_length);//��մ���3�Ľ�������
					usart3_Rx_length = 0;
        }
      }
    }
		if(Flag_all.CALL_Flag)//�в��Ŷ�������Ҫ��Ⲧ����ص�����
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
				if(NULL!=strstr((const char*)(usart3_Rx_buffer), "NO CARRIER")) //ͨ�������󣬲����Ƿ��ͨ�������ϱ�NO CARRIER��NO ANSWER
				{                                                                //NO ANSWER:���ų�ʱ����û�н�ͨ
           Flag_all.NO_CARRIER = 1;

        } 
         //NO ANSWER				
        if(NULL!=strstr((const char*)(usart3_Rx_buffer), "NO CARRIER")) //ͨ�������󣬲����Ƿ��ͨ�������ϱ�NO CARRIER��NO ANSWER
				{                                                                //NO ANSWER:���ų�ʱ����û�н�ͨ
           Flag_all.NO_ANSWER = 1;

        }		
      }
    }
		if(Flag_all.GM301_CPMS) //���ŵĴ������
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
		if(Flag_all.GM301_CMGR) //���ŵĴ������GM301_CMGR
		{
      if((usart3_Rx_length>6)&&(usart3_Rx_buffer[usart3_Rx_length-2]=='\r')&&(usart3_Rx_buffer[usart3_Rx_length-1]=='\n'))
			{				
				if(Flag_all.GM301_CMGR_UNread)
				{  //S    O    B     D    P    I   D    A    *    1    #    \r   \n  ǰ��û�лس����з�
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
          if((NULL!=strstr((const char*)(usart3_Rx_buffer), "\"REC UNREAD\"")))  //�ж϶���δ��
					{
						Flag_all.GM301_CMGR_UNread = 1; //����δ��
            memset(usart3_Rx_buffer,0x00,usart3_Rx_length);	
            usart3_Rx_length = 0;			
            return ;						
          }		
          else if((NULL!=strstr((const char*)(usart3_Rx_buffer), "\"REC READ\""))) //�ж϶����Ѷ�
          {            
						Flag_all.GM301_CMGR_UNread = 0; //�����Ѷ�
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
///      memset(usart3_Rx_buffer,0x00,140);//��մ���3�Ľ�������
//			usart3_Rx_length = 0;
//    }
/***ͨ������****************/
		if(usart3_Rx_length >140) //���յ����ݳ���140�����������ˣ�
		{
       memset(usart3_Rx_buffer,0x00,140);//��մ���3�Ľ�������
			 usart3_Rx_length = 0;
			 return ;
    }		
  } 
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)  //�����жϣ��жϵ���TXE������TC
	{		
		if (sendlengthU3 >= 1)
		{
			//while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
      USART3->DR = (u8) (*(psendU3++));
			sendlengthU3--;
    }
		else
		{
      USART_ITConfig(USART3, USART_IT_TXE, DISABLE); //�رշ����ж�
			return ;
    }		
	}	
} 
/*****���ڽ��ղ��� ����***********************/

//���ͳ���
//���룺
//�����
volatile u8 sendlengthU1 = 0;
u8 * psendU1;
void Usart1_SendData(u8 * psend, u8 length)
{
	sendlengthU1 = length;
	psendU1 = psend;
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //���������ж�
	  if(sendlengthU1 >= 1)
		{
			sendlengthU1--; //�ⲽҪ�ڿ��������ж�֮ǰ��ɣ������ж�һ�����ⲽִ���߼��ʹ���
			USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //���������ж�
      while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
      USART1->DR = (u8) (*(psendU1++));	
    }
		else
		{
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE); //���������ж�
			return ;
    }
}

volatile u8 sendlengthU2 = 0;
u8 * psendU2;
void Usart2_SendData(u8 * psend, u8 length)
{
	sendlengthU2 = length;
	psendU2 = psend;
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //���������ж�
	  if(sendlengthU2 >= 1)
		{
			sendlengthU2--; //�ⲽҪ�ڿ��������ж�֮ǰ��ɣ������ж�һ�����ⲽִ���߼��ʹ���
			USART_ITConfig(USART2, USART_IT_TXE, ENABLE); //���������ж�
      while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
      USART2->DR = (u8) (*(psendU2++));	
    }
		else
		{
      USART_ITConfig(USART2, USART_IT_TXE, DISABLE); //���������ж�
			return ;
    }
}

volatile u8 sendlengthU3 = 0;
u8 * psendU3;
void Usart3_SendData(u8 * psend, u8 length)
{
	sendlengthU3 = length;
	psendU3 = psend;
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //���������ж�
	  if(sendlengthU3 >= 1)
		{
			sendlengthU3--; //�ⲽҪ�ڿ��������ж�֮ǰ��ɣ������ж�һ�����ⲽִ���߼��ʹ���
			USART_ITConfig(USART3, USART_IT_TXE, ENABLE); //���������ж�
      while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
      USART3->DR = (u8) (*(psendU3++));	
    }
		else
		{
      USART_ITConfig(USART3, USART_IT_TXE, DISABLE); //���������ж�
			return ;
    }
}

/****GPS���ݴ������ձ�־**************************/
void GPS_Flag_Clear (void)
{
  Flag_all.GPSreceiv_eorr = 1 ;
	Flag_all.GPSreceiv_complete = 0;
	Flag_all.GPSdeal_complete = 0;//GPS���ݴ������
	memset(usart2_Rx_buffer,0x00,usart2_Rx_length);
	usart2_Rx_length = 0 ;
	Flag_all.GPSHead = 0; 
	Flag_all.GPSreceiv_complete = 0;
	memset(GPS_DATA,0x00,20);  //���������õģ���ʽ����Ҫע�͵�
}
/******************************/
/**********************************
GPS_DATA[20]
γ��       ����        �߳�      �ٶ�      ����       UTCʱ��
4�ֽ�      4�ֽ�      2�ֽ�      2�ֽ�     2�ֽ�      6�ֽ�
0--3       4--7       8--9        10--11   12--13    14--19

**********************************/
u8 GPS_DATA[20] ={0}; //����������ڽ���������
u8 GPS_DATA_BACKUP[20] ={0};//�������ݽ����õ����� 
u8 change_gpsdata(void)
{
  volatile u8 ucTen, ucOne;
	 u8* pTempBuf;
	 u8* pCommard;
	volatile u8 GPS_COMMARD = 0;
	volatile u8 temparry[40] = {0}; //��ʱ���gps����
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
		do			//��do...while�ṹ���ҳ�$GPRMC�е�UTCʱ���������ʱ��
	  {	
		  pCommard = (u8*)strchr((const char*)pTempBuf, ',');
	    ucTen = pCommard - pTempBuf;
	    if (0x01 == ucOne) //ȡGPS��UTCʱ�䣨ʱ���룩 HHMMSS.SSS
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
						j=i/10;  //ת��BCD�룬�õ�ʮλ
						i=i-j*10;//ת��BCD�룬�õ���λ
						GPS_DATA[UTC_GPS+3]=((j<<4)|i);
						GPS_DATA[UTC_GPS+4]=((temparry[2]&0x0f)<<4)|(temparry[3]&0x0f);
						GPS_DATA[UTC_GPS+5]=((temparry[4]&0x0f)<<4)|(temparry[5]&0x0f);
						data_change1 = 0;
						//memcpy(GPS_DATA+UTC_GPS, pTempBuf, 0x06);		//ȡGPS��UTCʱ�䣨ʱ���룩
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
					return 0;//ʱ����մ���
	    	}
	    }
	    else if (0x02 == ucOne) //��ȡGPSʱ���Ƿ���Ч�ı�־����A--��Ч��V--��Ч��
	    {
            GPS_COMMARD = *pTempBuf;		//��ȡGPSʱ���Ƿ���Ч�ı�־����A--��Ч��V--��Ч��
	          if(GPS_COMMARD =='V')
						{
               GPS_Flag_Clear();					
							 GPIO_ResetBits(GPIOC,GPIO_Pin_6);  //GPS����
					     return 0;//ʱ����ܴ���	
            }
						else if(GPS_COMMARD =='A')
						{
               GPIO_SetBits(GPIOC,GPIO_Pin_6);  //GPS����
            }
      }
			else if(0x03==ucOne)//��ȡγ����Ϣ
			{
        if(ucTen ==9) //γ����9���ַ�
				{//ddmm.mmmm
					data_change = 0;
					 memset((u8 *)temparry,0x00,40);  //
           memcpy((u8 *)temparry,pTempBuf,ucTen);
					 //GPS_DATA[latitude_GPS]=(((temparry[0]&0x0f)<<4)|(temparry[1]&0x0f)); //DDmm.mmmm	
					data_change=((u32)(((temparry[0]&0x0f)*10)+(temparry[1]&0x0f))); //DDmm.mmmm
					data_change =(((temparry[2]&0x0f)*1000000+(temparry[3]&0x0f)*100000+(temparry[5]&0x0f)*10000+(temparry[6]&0x0f)*1000+(temparry[7]&0x0f)*100+(temparry[8]&0x0f)*10)/6)+data_change*1000000;
					 GPS_DATA[latitude_GPS]=data_change >> 24; //DDmm.mmmm
					GPS_DATA[latitude_GPS+1]=(data_change >> 16)&0x000000ff;//ddMM.mmmm   
					 GPS_DATA[latitude_GPS+2]=(data_change >> 8)&0x000000ff;//ddmm.MMmm  ����С����
					 GPS_DATA[latitude_GPS+3]=data_change &0x000000ff;//ddmm.mmMM
					//data_change =((516889*100)/60);
					//data_change1 =shiliutoshi(data_change);
					//shiliutoshi(data_change,GPS_DATA+latitude_GPS+1);
					
        }					
				else
				{
           GPS_Flag_Clear();
					return 0;//γ����ȡ����
        }
						
      }
			else if(0x05==ucOne)//��ȡ������Ϣ
			{
        if(ucTen ==10) //γ����9���ַ�
				{//dddmm.mmmm
					data_change = 0;  //
          memset((u8 *)temparry,0x00,40);
					memcpy((u8 *)temparry,pTempBuf,ucTen);
					 //GPS_DATA[latitude_GPS]=(((temparry[0]&0x0f)<<4)|(temparry[1]&0x0f)); //DDmm.mmmm
          //data_change	=((u32)(((temparry[0]&0x0f)*100)+(temparry[1]&0x0f)*10+(temparry[2]&0x0f)));	
					data_change	=((temparry[0]&0x0f)*100)+((temparry[1]&0x0f)*10)+(temparry[2]&0x0f);	 //ע�����ŵ�����					
					data_change =(((temparry[3]&0x0f)*1000000+(temparry[4]&0x0f)*100000+(temparry[6]&0x0f)*10000+(temparry[7]&0x0f)*1000+(temparry[8]&0x0f)*100+(temparry[9]&0x0f)*10)/6)+data_change*1000000;
					GPS_DATA[longitude_GPS]=data_change >> 24; //DDmm.mmmm
					GPS_DATA[longitude_GPS+1]=(data_change >> 16)&0x000000ff;//ddMM.mmmm   
					 GPS_DATA[longitude_GPS+2]=(data_change >> 8)&0x000000ff;//ddmm.MMmm  ����С����
					 GPS_DATA[longitude_GPS+3]=data_change &0x000000ff;//ddmm.mmMM
					//data_change =((516889*100)/60);
					//data_change1 =shiliutoshi(data_change);
					//shiliutoshi(data_change,GPS_DATA+latitude_GPS+1);
        }					
				else
				{
           GPS_Flag_Clear();
					return 0;//ʱ����մ���
        }						
      }
			else if(0x07==ucOne)//��ȡ�ٶ���Ϣ��20140618�칫���޷��õ��ٶ�ֵ��������
			{
        if(ucTen > 0) //���ٶ�ֵ�� �ٶ�ֵ�ö��ٸ��ֽڱ�ʾ��ȷ����
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
					return 0;//ʱ����մ���
        }					
      }
			else if(0x08==ucOne)//��ȡ�ٶȷ���
			{
        if((ucTen > 3)&&(ucTen <7)) //���ٶ�ֵ�� �ٶ�ֵ�ö��ٸ��ֽڱ�ʾ��ȷ����
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
					return 0;//ʱ����մ���
        }				
      } 
			
	    else if (0x09 == ucOne)		//��ȡGPS������
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
					return 0;//ʱ����մ���
        }		
	    	break;
	    }
		  pTempBuf = pCommard + 1;
		  ucOne++;
	  }while(1);
	  for(ucOne=0;ucOne<20;ucOne++) //��while���棬˵����ȷ�������GPS���ݡ�
    {
	    GPS_DATA_BACKUP[ucOne]=	GPS_DATA[ucOne]; //������õ�GPS���ݷŵ�������������ȥ
    }
	  return 1;	
  }
	return 0;
}

u32 shiliutoshi(u32 ret,u8 *p)
{
   
 unsigned char ret6=0,ret5=0,ret4=0,ret3=0,ret2=0,ret1=0;//

    ret6=ret/100000;                   //eg:ret=0xD2529=861481,ֱ����ʮ��������õ���λ8���ٵõ�6һ������
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

