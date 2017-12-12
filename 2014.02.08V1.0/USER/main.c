/******************************************************************************************
*  File Name                             :prot.h
*  Copyright                             :2014.2 wei
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
#include "main.h"

GLOBALstruct_Flag  Flag_all;
OBD_Flag  OBD_Com_Flag; 

extern void PORT_Init(void);
//void test_SPI(void);
//void test_show_led(void);

u16 timer2_100ms = 0;      //��ʱ��2  100ms��ʱ��
u16 oil_sampling_time =0;  //L/H,����ʱ�䣬��Ϊ3��
u8  read_GSM_time =0; //��GSM�ļ��ʱ��
u8  Read_oilmass_time =0; //��ȡʣ��������ʱ��
u32 read_reset =0;  //����λԴ
/***
 void Delay(u32 count)
 {
  u32 i=0;
  for(;i<count;i++);

 }
//  ***
  void Delay1(u32 count)
 {
  u32 i=0;
  for(;i<count;i++)
	 {
     Delay(8000); 
   }

 }
 ***/
void wait(u32 count) //count Ϊ1ms
{
  gps_counter = count;
	while(gps_counter !=1)
	{
    IWDG_Feed();
  }	
}

u8 hex2acii_arry[16] ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
u8 reset_source[8] = {0}; //������Ÿ�λԴ
int main(void)
{	
	 
   u8 i = 0;
	/*********************************/ 
	 read_reset =RCC->CSR;
    
	 for(i=0;i<8;i++)
   {
     reset_source[7-i] = hex2acii_arry[(((u8)(read_reset>>(i*4)))&0x0f)];
   }
	 
	 Hardware_Init();
	 memset(&Flag_all,0x00,sizeof(GLOBALstruct_Flag));//�Զ����˱�־���Ľṹ���ʼ��
	 memset(&OBD_Com_Flag,0x00,sizeof(OBD_Flag));//�Զ����˱�־���Ľṹ���ʼ��
	
	 
/**********************************/	 
	 IWDG_Feed();
	 RCC->CSR |=(0x00000001<<24); 
	 //read_reset =RCC->CSR;
	 Usart2_SendData((u8 *)"Reset Source: ",sizeof("Reset Source: "));
	 Usart2_SendData(reset_source,8);
	 Usart2_SendData((u8 *)"\r\n",2);
//	 SystemReset();
//	 while(1);
	 Usart2_SendData((u8 *)"FHT-GBOBD20140616...\r\n",22);
	 Usart2_SendData((u8 *)"press\"%%%%%%%\"FOR DEBUG MODEL whthin 10s......\r\n",48);
 
	 Debug();//�����������ģʽ
	 uart3_init(115200);  //����3��GSMͨ�ŵģ�GSM�õ���9600
	 uart2_init(4800);  //����2��GPSͨ�ŵģ�GPS�õ���4800.
   sirf4Config();//��ʼ��GPSģ�飬
	 
	 Usart3_SendData((u8 *)"Reset Source: ",sizeof("Reset Source: "));
	 Usart3_SendData((reset_source),0x08);
	 Usart3_SendData((u8 *)"\r\n",0x02);
   OBD_init(); //OBD��ʼ������
/******����SPI FLASH****************************/		
	 //test_SPI();
	 //init_arry();
/**********************************/	
/******GSMģ���ʼ��****************************/		 
//  GSM_startup();
//	delay_Nx100ms(50);
//	GSM_shutdown();
//	GSM_INIT();
	GSM_Module_Start();
/**********************************/	
/*********������ʼ��*************************/	

  Parameter_Init(); //������ʼ������Ҫ�Ƕ�ȡ�����SPI FLASH�еĲ���

 // Terminal_JianQuan_authentication(); //�Գ���ע��ͼ�Ȩ
/***************************************************/
	 while(1)
		{      
			IWDG_Feed();
			//delay_Nx100ms(50);
			//read_reset =RCC->CSR;
		
		  // OBD_PID_Test();  //����PID��������ȡ������
//			TESET_GSM_Send();
//			GSM_Ring();
			//alarm();
//			oil_average_LperH();
//			mileage(); //��ȡ��̣����������������
//			Read_oil_mass();
		  Upload_Location();//�ϴ�λ����Ϣ
				IWDG_Feed();
			Upload_Heart(); //�ϴ�����
				IWDG_Feed();			
			alarm();//��龯��
			  IWDG_Feed();		
			BodyChang_UpLoad(); //�����ô�г���״̬�����ı䣬����У������ϴ�λ����Ϣ
			  IWDG_Feed();
			if(Flag_all.haveGSMdata == SISR_7E_OK) //ֻҪ�յ�������Ϣ(��,7E��ͷ��7E��β������)���������ò�����Ϣ������
			{
        GSM_Recivdata_deal(); //���ò�����Ϣ������
      }
			if(Flag_all.Check_Net_Flag)  //�м������״̬��־������ȥ����ն��������������״̬
			{
        //��������״̬������ͨ�������������������         
      }
			if(Flag_all.SM_FLAG == HAVE_SM) //ģ����յ�����
			{
        Flag_all.SM_FLAG = 0;
				SM_Deal();  //���Ŵ�����
      }
			//
			if((OBD_Com_Flag.ACC_state)&&(Read_oilmass_time >=200)) //ACC��������ʱ2S��һ��ʣ������
			{
        Read_oilmass_time =0;//���㶨ʱ��
				Read_oil_mass(); //��ȡʣ����������
      }
			if((read_GSM_time>=250)||(Flag_all.haveGSMdata==SISR_1))  //����3s��һ��GSM������յ�GSM�����ϱ�������Ϣ��־��Ҳ��������ȡ
			{
				Read_GSM();
				//if(Read_Deal_GSM())
				//{
          read_GSM_time=0;
				if(Flag_all.haveGSMdata==SISR_1)
					Flag_all.haveGSMdata=0;
       // }
      }//
			
    }
}
 
void Hardware_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	  Exit_INIT() ; //�ⲿ�жϳ�ʼ��
	  delay_init();
	
	
	 
	  PORT_Init();  //�˿ڳ�ʼ��
	 
	  IWDG_Init(4,1000); // 4,1000->1.6�룻 Tout=((4*2^prer)*rlr)/40 (ms) �������Ź���ʼ��
	  TIM2_Int_Init(999,719);//100Khz�ļ���Ƶ�� ������1000Ϊ10ms����һ���ж�
	 IWDG_Feed();
	  SPI_Flash_Init(); //spi flash w25q64�Ĳ�����ʼ��
	 IWDG_Feed();
	  uart1_init(19200); //����1��OBDͨ�ŵģ�OBD�õ���19200.
	 IWDG_Feed();
	  //uart2_init(9600);  //����2��GPSͨ�ŵģ�GPS�õ���4800.
	 uart2_init(4800);  //����2��GPSͨ�ŵģ�GPS�õ���4800.
	 IWDG_Feed();
	  uart3_init(9600);  //����3��GSM�Ͳ�������ͨ�������������õ���9600��GSM�õ���115200
	 IWDG_Feed();
	 SPI_check();	
}


//������ʼ������
//ע�ᣬ��Ȩ����ȡ
void Parameter_Init(void)
{
  u8 temp[20] ={0};
  u8 bytetemp = 0;
  u16 temp_16 = 0;

/****��ȡPID����������S*******************************************************/
    SPI_Flash_SafeRead((u8 *)(&(PID_user_defined_number)),PID_addr,0x01);
    if((PID_user_defined_number>0)&&(PID_user_defined_number < 104))
		   SPI_Flash_SafeRead(PID_user_defined,PID_addr+1,PID_user_defined_number);
		else 
		{
      PID_user_defined_number = 0;
			memset(PID_user_defined,0x00,103);
    }
/****��ȡPID����������E*******************************************************/
/**********��ȡ������***START*****************/
  memset(temp,0x00,20);
  SPI_Flash_SafeRead(temp,BenJiHao_addr,1); //��ȡ�����ų���
	if((temp[0] > '0') && (temp[0] <= '9'))
		bytetemp = (temp[0] - 0x30);
	else if((temp[0] >= 'A') && (temp[0] <= 'F'))
		bytetemp = (temp[0] - 0x37);
	else 
	{
		Usart2_SendData("BenJiHao ERROR, Reset BenJiHao\r\n",sizeof("BenJiHao ERROR, Reset BenJiHao\r\n"));
		memset((u8 *)ben_ji_hao,0x00,6);//�����Ŵ����ʱ�򣬰ѱ����Ŷ���Ϊ0x00
		return ;
	}
	SPI_Flash_SafeRead((u8 *)ben_ji_hao,BenJiHao_addr+1,bytetemp); //�ѱ����Ŷ��������ŵ�ben_ji_hao������.
	/**********��ȡ������****END*****************/
	
/**********ע��********************�ж��Ƿ�Ҫע��*******/
//SPI_Flash_SafeWrite((u8 *)(&(temp_0x55)),Terminal_auth_Flag_addr,0x01);//�洢ע��ɹ���־
	memset(temp,0x00,20);	
  SPI_Flash_SafeRead(temp,Terminal_auth_Flag_addr,0x01);
  if(0x55 != temp[0])//֮ǰû��ע��ɹ�
	{
    Terminal_authentication();//����ע�����ֱ��ע��ɹ�
		//SystemReset(); //���Ը�λ����
  }
  else
	{
    SPI_Flash_SafeRead((u8 *)(&(JianQuanMa_Len)),JianQuanMa_addr,0x01); //������Ȩ�볤�ȣ�JianQuanMa_Len
		SPI_Flash_SafeRead((u8 *)JianQuanMa,JianQuanMa_addr+1,JianQuanMa_Len);//������Ȩ�룬JianQuanMa[]
		//SPI_Flash_SafeWrite((u8 *)(&(temp_0x55)),Terminal_auth_Flag_addr,0x01);//�洢ע��ɹ���־
		Flag_all.Terminal_auth_Flag = TRUE; //�ն�ע��ɹ��ı�־
  }
/**********�ж��Ƿ�Ҫע��****END*****************/

/**********�ж��Ƿ�Ҫ��Ȩ****START*****************/	
	if(Flag_all.Terminal_JianQuan_Flag != TRUE)
	{
    if(Flag_all.Terminal_auth_Flag == TRUE) //���ע��ɹ��������Ȩ
		{
			Terminal_JianQuan();
		}
		else
		{
			Usart2_SendData("Zhu ce fail reset...\r\n",sizeof("Zhu ce fail reset...\r\n"));
			SystemReset();
		}
  }

/**********�ж��Ƿ�Ҫ��Ȩ****END*****************/
	

/***��ȡλ����Ϣ�ϴ�ʱ��*START******************/  
   memset(temp,0x00,20);		
   SPI_Flash_SafeRead(temp,GPSuploadtime,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;  //
   if((temp_16 > 250)||(temp_16 < 5))//((temp[0]>120)||(temp[0]<20)) //ACC��״̬��λ����Ϣ�ϴ����ȡ5--250s
	 {
     temp[1] =0x3c; //λ����Ϣ��������Ϊ60s 
		 temp[0] = 0;
		 SPI_Flash_SafeWrite(temp,GPSuploadtime,0x02);
		 LocationUptime = 0x3c;  //*100;
		 oil_LperH_F = LocationUptime/3;
   }
	 else
	 {
     LocationUptime =temp_16; //temp[0];  //*100;
		 oil_LperH_F = LocationUptime/3;
	 }
//ACC�ص�״̬�£�λ���ϱ�ʱ����	 
	 memset(temp,0x00,20);		
   SPI_Flash_SafeRead(temp,ACCOFF_GPSuploadtime_add,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if((temp_16 > 0xFFFE)||((temp_16 < 0x0005)))//(temp[0]>120)||(temp[0]<20)) //ACC��״̬��λ����Ϣ�ϴ����ȡ5--0xFFFEs
	 {
     temp[0] =0x00; //λ����Ϣ��������Ϊ90s 
		 temp[1] =0x5A; //
		 SPI_Flash_SafeWrite(temp,ACCOFF_GPSuploadtime_add,0x02);
		 ACCOFF_LocationUptime = 0x5a;  //*100;
		 
   }
	 else
	 {
     ACCOFF_LocationUptime = temp_16;  //*100;
		 //oil_LperH_F = LocationUptime/3;
	 }
/***��ȡλ����Ϣ�ϴ�ʱ��**END*****************/  
	 
/***�������ϴ�ʱ��*START******************/ 
   memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,upheartime_addr,0x04);
	 upheartime = ((temp[0]<<24)|(temp[1]<<16)|(temp[2]<<8)|temp[3]) ; //��4��u8���͵���ת��u32
   //if((temp[0]>120)||(temp[0]<40)) //�ж��������ʱ��20--120s
	 if((upheartime >60)&&(upheartime < 30)) //�ж������ϴ����ʱ�䣬�Ƿ�����
	 {
     //temp[0] =0x28; //λ����Ϣ��������Ϊ60s 
		 upheartime = (u32)0x28; //���������������Ϊ40s
		 temp[0] =0x00;
		 temp[1] =0x00;
		 temp[2] =0x00;
		 temp[3] =0x28;
		 SPI_Flash_SafeWrite(temp,upheartime_addr,0x04);
		 //upheartime = temp[0]*100;		
   }
	// else
	 // upheartime =temp[0]*100;	 
/***�������ϴ�ʱ��**END*****************/  	 

/***������Ӧ��ʱʱ��*START******************/ 
   memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,upheart_timeout_addr,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if((temp_16>0x258)||(temp_16<0x3c)||(temp[0]==0xff)||(temp[1]==0xff)) //����Ӧ��ʱʱ�䣬��ֵ��ΧӦ����60s--600s��
	 {//temp[0]*10s = ����Ӧ��ʱʱ��
     temp[0] =0x01; //������ʱʱ�䣬����Ϊ300s=5min; 0x1e*10=300s
		 temp[1] =0x2c;
		 SPI_Flash_SafeWrite(temp,upheart_timeout_addr,0x02);
		 upheart_timeout = 0x012c;		//300s,5min
   }
	 else
	  upheart_timeout = temp_16;	 
/***������Ӧ��ʱʱ��**END*****************/ 

/***��ȡ��������ʱ����*************************/	 
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,JinJi_addr,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if((temp_16>250)||(temp_16 < 2))        //((temp[0]>250)||(temp[0]<2)) //��������ʱ��������10sΪ��λ���2*10s�����250*10s;��ȡ��temp[0]��ֵ��ΧӦ����6--250��
	 {//temp[0]*10s = ��������ʱ����
     temp[0] =0;   //0x9; //��������ʱ������������Ϊ9*10s =90s.
		 temp[1] =0x0a; //Ĭ�Ͻ�������ʱ������10s
		 SPI_Flash_SafeWrite(temp,JinJi_addr,0x02);
		 JinJI_timeout = temp_16 ;		
		 //JinJi_JianGe = JinJI_timeout/5; //�������½�������ʱ����Ҫ���һ��ʱ���ڱ�,������ʱ��ȡ������������֮һ
		 Shake_JianGE = JinJI_timeout/5; //����������ʱ����Ҫ���һ��ʱ���ڱ�,������ʱ��ȡ������������֮һ
   }
	 else
	 {     
		 JinJI_timeout =temp_16 ;
		 //JinJi_JianGe = JinJI_timeout/5; //�������½�������ʱ����Ҫ���һ��ʱ���ڱ�,������ʱ��ȡ������������֮һ
		 Shake_JianGE = JinJI_timeout/5; //����������ʱ����Ҫ���һ��ʱ���ڱ�,������ʱ��ȡ������������֮һ
   }
	  
/***��ȡ��������ʱ����*************************/	 
	 
//����Χ���뾶
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,R_rail_addr,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if((temp_16 ==0xFFFF)||(temp_16 < 0x0a))  //����Χ����ȡֵ��Χ
	 {
     temp[0] =0;   
		 temp[1] =0x00; //Ĭ�ϵ���Χ���뾶��0�ף�����ʹ��
		 SPI_Flash_SafeWrite(temp,R_rail_addr,0x02);
		 R_rail = 0 ;			 
   }
	 else
	 {     
		 R_rail = temp_16 ;		
   }
	 
 //����ֵ
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,Over_Speed_addr,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if((temp_16 ==0xFFFF)||(temp_16 < 0x0a))  //����ֵ��ȡֵ��Χ
	 {
     temp[0] =0;   
		 temp[1] =0x00; //Ĭ�ϳ���ֵΪ0������ʹ��
		 SPI_Flash_SafeWrite(temp,Over_Speed_addr,0x02);
		 Over_Speed = 0 ;			 
   }
	 else
	 {     
		 Over_Speed = temp_16 ;		
   }
	 
 //���ٳ���ʱ��	 
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,Over_Speed_XDTime_addr,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if((temp_16 ==0xFFFF)||(temp_16 < 0x0a))  //����ֵ��ȡֵ��Χ
	 {
     temp[0] =0;   
		 temp[1] =0x00; //Ĭ�ϳ���ֵΪ0������ʹ��
		 SPI_Flash_SafeWrite(temp,Over_Speed_XDTime_addr,0x02);
		 Over_Speed_XDTime = 0 ;			 
   }
	 else
	 {     
		 Over_Speed_XDTime = temp_16 ;		
   }
	 
 //��������ʡ��ID
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,Province_ID_addr,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if(temp_16 ==0xFFFF)//||(temp_16 < 0x0a))  //ʡ��ID
	 {
     Province_ID[0] = 0;
		 Province_ID[1] = 0;
		 //SPI_Flash_SafeWrite(temp,Over_Speed_XDTime_addr,0x02);		 			 
   }
	 else
	 {     
		 Province_ID[0] = temp[0];	
		 Province_ID[1] = temp[1];
   }
	 
 //������������ID
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,City_ID_addr,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if(temp_16 ==0xFFFF)//||(temp_16 < 0x0a))  //ʡ��ID
	 {
     City_ID[0] = 0;
		 City_ID[1] = 0;
		 //SPI_Flash_SafeWrite(temp,Over_Speed_XDTime_addr,0x02);		 			 
   }
	 else
	 {     
		 City_ID[0] = temp[0];	
		 City_ID[1] = temp[1];
   }
	 
	//�����ճ��ȣ����ƾ�������
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,Vehicle_license_Plate_Len_addr,0x01);
	 bytetemp = temp[0] ;  //(temp[0]<<8)|temp[1] ;
   if(bytetemp >14)  //���Ƴ��ȳ���14�Ͳ���
	 {
      //���ƴ���Ĵ����� 
   }
	 else   //���Ƴ�����ȷ����ȡ���Ƶľ�������
	 {     
		 memset(temp,0x00,20);	 
     SPI_Flash_SafeRead(Vehicle_license_Plate,Vehicle_license_Plate_addr,bytetemp);  //������ֱ�Ӷ������ƴ洢������ 
   }
	 
	//������ɫ
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,Vehicle_license_Plate_colour_addr,0x01);
	 bytetemp = temp[0] ;  //(temp[0]<<8)|temp[1] ;
   if(bytetemp == 0xFF)  //������ɫֵΪ0xFF,��ʾ������ɫû�б��洢
	 {
      //������ɫ�������� 
   }
	 else   //������ɫ��ȷ����ȡ������ɫ�ľ�������	  
     Vehicle_license_Plate_colour = bytetemp;	
  
 //GNSS��λģʽ
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,GNSS_MODE_addr,0x01);
	 bytetemp = temp[0] ;  //(temp[0]<<8)|temp[1] ;
   if(bytetemp >3)  //������ɫֵΪ0xFF,��ʾ������ɫû�б��洢
	 {
      //������ɫ�������� 
   }
	 else   //������ɫ��ȷ����ȡ������ɫ�ľ�������	  
     GNSS_MODE = bytetemp;	
 
 //GNSS������
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,GNSS_Baud_addr,0x01);
	 bytetemp = temp[0] ;  //(temp[0]<<8)|temp[1] ;
   if(bytetemp >0x05)  //������ɫֵΪ0xFF,��ʾ������ɫû�б��洢
	 {
      //������ɫ�������� 
   }
	 else   //������ɫ��ȷ����ȡ������ɫ�ľ�������	  
     GNSS_Baud = bytetemp;	
	 
 //GNSSģ����ϸ��λ�������Ƶ��
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,GNSS_Hz_addr,0x01);
	 bytetemp = temp[0] ;  //(temp[0]<<8)|temp[1] ;
   if(bytetemp >0x04)  //������ɫֵΪ0xFF,��ʾ������ɫû�б��洢
	 {
      //������ɫ�������� 
   }
	 else   //������ɫ��ȷ����ȡ������ɫ�ľ�������	  
     GNSS_Hz = bytetemp;
		 
   
}
 



















