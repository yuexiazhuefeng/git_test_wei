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

u16 timer2_100ms = 0;      //定时器2  100ms定时器
u16 oil_sampling_time =0;  //L/H,采样时间，设为3秒
u8  read_GSM_time =0; //读GSM的间隔时间
u8  Read_oilmass_time =0; //读取剩余油量定时器
u32 read_reset =0;  //读复位源
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
void wait(u32 count) //count 为1ms
{
  gps_counter = count;
	while(gps_counter !=1)
	{
    IWDG_Feed();
  }	
}

u8 hex2acii_arry[16] ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
u8 reset_source[8] = {0}; //用来存放复位源
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
	 memset(&Flag_all,0x00,sizeof(GLOBALstruct_Flag));//对定义了标志符的结构体初始化
	 memset(&OBD_Com_Flag,0x00,sizeof(OBD_Flag));//对定义了标志符的结构体初始化
	
	 
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
 
	 Debug();//进入参数设置模式
	 uart3_init(115200);  //串口3是GSM通信的，GSM用的是9600
	 uart2_init(4800);  //串口2是GPS通信的，GPS用的是4800.
   sirf4Config();//初始化GPS模块，
	 
	 Usart3_SendData((u8 *)"Reset Source: ",sizeof("Reset Source: "));
	 Usart3_SendData((reset_source),0x08);
	 Usart3_SendData((u8 *)"\r\n",0x02);
   OBD_init(); //OBD初始化程序
/******测试SPI FLASH****************************/		
	 //test_SPI();
	 //init_arry();
/**********************************/	
/******GSM模块初始化****************************/		 
//  GSM_startup();
//	delay_Nx100ms(50);
//	GSM_shutdown();
//	GSM_INIT();
	GSM_Module_Start();
/**********************************/	
/*********参数初始化*************************/	

  Parameter_Init(); //参数初始化，主要是读取存放在SPI FLASH中的参数

 // Terminal_JianQuan_authentication(); //对车辆注册和鉴权
/***************************************************/
	 while(1)
		{      
			IWDG_Feed();
			//delay_Nx100ms(50);
			//read_reset =RCC->CSR;
		
		  // OBD_PID_Test();  //测试PID数据流读取函数，
//			TESET_GSM_Send();
//			GSM_Ring();
			//alarm();
//			oil_average_LperH();
//			mileage(); //读取里程，单纯测试这个函数
//			Read_oil_mass();
		  Upload_Location();//上传位置信息
				IWDG_Feed();
			Upload_Heart(); //上穿心跳
				IWDG_Feed();			
			alarm();//检查警情
			  IWDG_Feed();		
			BodyChang_UpLoad(); //检测有么有车身状态发生改变，如果有，立即上传位置信息
			  IWDG_Feed();
			if(Flag_all.haveGSMdata == SISR_7E_OK) //只要收到部标信息(即,7E开头和7E结尾的数据)，立即调用部标消息处理函数
			{
        GSM_Recivdata_deal(); //调用部标消息处理函数
      }
			if(Flag_all.Check_Net_Flag)  //有检测网络状态标志，这里去检测终端与服务器的连接状态
			{
        //调用网络状态检测程序，通过发送心跳程序来完成         
      }
			if(Flag_all.SM_FLAG == HAVE_SM) //模块接收到短信
			{
        Flag_all.SM_FLAG = 0;
				SM_Deal();  //短信处理函数
      }
			//
			if((OBD_Com_Flag.ACC_state)&&(Read_oilmass_time >=200)) //ACC看来，定时2S读一次剩余油量
			{
        Read_oilmass_time =0;//清零定时器
				Read_oil_mass(); //读取剩余油量函数
      }
			if((read_GSM_time>=250)||(Flag_all.haveGSMdata==SISR_1))  //周期3s读一次GSM，如果收到GSM主动上报的有消息标志，也会立即读取
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
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	  Exit_INIT() ; //外部中断初始化
	  delay_init();
	
	
	 
	  PORT_Init();  //端口初始化
	 
	  IWDG_Init(4,1000); // 4,1000->1.6秒； Tout=((4*2^prer)*rlr)/40 (ms) 独立看门狗初始化
	  TIM2_Int_Init(999,719);//100Khz的计数频率 计数到1000为10ms产生一次中断
	 IWDG_Feed();
	  SPI_Flash_Init(); //spi flash w25q64的操作初始化
	 IWDG_Feed();
	  uart1_init(19200); //串口1是OBD通信的，OBD用的是19200.
	 IWDG_Feed();
	  //uart2_init(9600);  //串口2是GPS通信的，GPS用的是4800.
	 uart2_init(4800);  //串口2是GPS通信的，GPS用的是4800.
	 IWDG_Feed();
	  uart3_init(9600);  //串口3是GSM和参数设置通道，参数设置用的是9600，GSM用的是115200
	 IWDG_Feed();
	 SPI_check();	
}


//参数初始化程序，
//注册，鉴权，提取
void Parameter_Init(void)
{
  u8 temp[20] ={0};
  u8 bytetemp = 0;
  u16 temp_16 = 0;

/****读取PID数据流配置S*******************************************************/
    SPI_Flash_SafeRead((u8 *)(&(PID_user_defined_number)),PID_addr,0x01);
    if((PID_user_defined_number>0)&&(PID_user_defined_number < 104))
		   SPI_Flash_SafeRead(PID_user_defined,PID_addr+1,PID_user_defined_number);
		else 
		{
      PID_user_defined_number = 0;
			memset(PID_user_defined,0x00,103);
    }
/****读取PID数据流配置E*******************************************************/
/**********读取本机号***START*****************/
  memset(temp,0x00,20);
  SPI_Flash_SafeRead(temp,BenJiHao_addr,1); //读取本机号长度
	if((temp[0] > '0') && (temp[0] <= '9'))
		bytetemp = (temp[0] - 0x30);
	else if((temp[0] >= 'A') && (temp[0] <= 'F'))
		bytetemp = (temp[0] - 0x37);
	else 
	{
		Usart2_SendData("BenJiHao ERROR, Reset BenJiHao\r\n",sizeof("BenJiHao ERROR, Reset BenJiHao\r\n"));
		memset((u8 *)ben_ji_hao,0x00,6);//本机号错误的时候，把本机号都设为0x00
		return ;
	}
	SPI_Flash_SafeRead((u8 *)ben_ji_hao,BenJiHao_addr+1,bytetemp); //把本机号读出来，放到ben_ji_hao数组中.
	/**********读取本机号****END*****************/
	
/**********注册********************判断是否要注册*******/
//SPI_Flash_SafeWrite((u8 *)(&(temp_0x55)),Terminal_auth_Flag_addr,0x01);//存储注册成功标志
	memset(temp,0x00,20);	
  SPI_Flash_SafeRead(temp,Terminal_auth_Flag_addr,0x01);
  if(0x55 != temp[0])//之前没有注册成功
	{
    Terminal_authentication();//进入注册程序，直到注册成功
		//SystemReset(); //测试复位函数
  }
  else
	{
    SPI_Flash_SafeRead((u8 *)(&(JianQuanMa_Len)),JianQuanMa_addr,0x01); //读出鉴权码长度，JianQuanMa_Len
		SPI_Flash_SafeRead((u8 *)JianQuanMa,JianQuanMa_addr+1,JianQuanMa_Len);//读出鉴权码，JianQuanMa[]
		//SPI_Flash_SafeWrite((u8 *)(&(temp_0x55)),Terminal_auth_Flag_addr,0x01);//存储注册成功标志
		Flag_all.Terminal_auth_Flag = TRUE; //终端注册成功的标志
  }
/**********判断是否要注册****END*****************/

/**********判断是否要鉴权****START*****************/	
	if(Flag_all.Terminal_JianQuan_Flag != TRUE)
	{
    if(Flag_all.Terminal_auth_Flag == TRUE) //如果注册成功，进入鉴权
		{
			Terminal_JianQuan();
		}
		else
		{
			Usart2_SendData("Zhu ce fail reset...\r\n",sizeof("Zhu ce fail reset...\r\n"));
			SystemReset();
		}
  }

/**********判断是否要鉴权****END*****************/
	

/***读取位置信息上传时间*START******************/  
   memset(temp,0x00,20);		
   SPI_Flash_SafeRead(temp,GPSuploadtime,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;  //
   if((temp_16 > 250)||(temp_16 < 5))//((temp[0]>120)||(temp[0]<20)) //ACC开状态下位置信息上传间隔取5--250s
	 {
     temp[1] =0x3c; //位置信息错误，重置为60s 
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
//ACC关的状态下，位置上报时间间隔	 
	 memset(temp,0x00,20);		
   SPI_Flash_SafeRead(temp,ACCOFF_GPSuploadtime_add,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if((temp_16 > 0xFFFE)||((temp_16 < 0x0005)))//(temp[0]>120)||(temp[0]<20)) //ACC关状态下位置信息上传间隔取5--0xFFFEs
	 {
     temp[0] =0x00; //位置信息错误，重置为90s 
		 temp[1] =0x5A; //
		 SPI_Flash_SafeWrite(temp,ACCOFF_GPSuploadtime_add,0x02);
		 ACCOFF_LocationUptime = 0x5a;  //*100;
		 
   }
	 else
	 {
     ACCOFF_LocationUptime = temp_16;  //*100;
		 //oil_LperH_F = LocationUptime/3;
	 }
/***读取位置信息上传时间**END*****************/  
	 
/***读心跳上传时间*START******************/ 
   memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,upheartime_addr,0x04);
	 upheartime = ((temp[0]<<24)|(temp[1]<<16)|(temp[2]<<8)|temp[3]) ; //将4个u8类型的数转换u32
   //if((temp[0]>120)||(temp[0]<40)) //判断心跳间隔时间20--120s
	 if((upheartime >60)&&(upheartime < 30)) //判断心跳上传间隔时间，是否正常
	 {
     //temp[0] =0x28; //位置信息错误，重置为60s 
		 upheartime = (u32)0x28; //心跳间隔有误，重置为40s
		 temp[0] =0x00;
		 temp[1] =0x00;
		 temp[2] =0x00;
		 temp[3] =0x28;
		 SPI_Flash_SafeWrite(temp,upheartime_addr,0x04);
		 //upheartime = temp[0]*100;		
   }
	// else
	 // upheartime =temp[0]*100;	 
/***读心跳上传时间**END*****************/  	 

/***读心跳应答超时时间*START******************/ 
   memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,upheart_timeout_addr,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if((temp_16>0x258)||(temp_16<0x3c)||(temp[0]==0xff)||(temp[1]==0xff)) //心跳应答超时时间，该值范围应该是60s--600s，
	 {//temp[0]*10s = 心跳应答超时时间
     temp[0] =0x01; //心跳超时时间，重置为300s=5min; 0x1e*10=300s
		 temp[1] =0x2c;
		 SPI_Flash_SafeWrite(temp,upheart_timeout_addr,0x02);
		 upheart_timeout = 0x012c;		//300s,5min
   }
	 else
	  upheart_timeout = temp_16;	 
/***读心跳应答超时时间**END*****************/ 

/***读取紧急报警时间间隔*************************/	 
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,JinJi_addr,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if((temp_16>250)||(temp_16 < 2))        //((temp[0]>250)||(temp[0]<2)) //紧急报警时间间隔，以10s为单位最低2*10s，最高250*10s;读取到temp[0]的值范围应该是6--250，
	 {//temp[0]*10s = 紧急报警时间间隔
     temp[0] =0;   //0x9; //紧急报警时间间隔错误，重置为9*10s =90s.
		 temp[1] =0x0a; //默认紧急报警时间间隔，10s
		 SPI_Flash_SafeWrite(temp,JinJi_addr,0x02);
		 JinJI_timeout = temp_16 ;		
		 //JinJi_JianGe = JinJI_timeout/5; //连续按下紧急报警时，需要间隔一段时间在报,这个间隔时间取报警间隔的五分之一
		 Shake_JianGE = JinJI_timeout/5; //连发生紧急时，需要间隔一段时间在报,这个间隔时间取报警间隔的五分之一
   }
	 else
	 {     
		 JinJI_timeout =temp_16 ;
		 //JinJi_JianGe = JinJI_timeout/5; //连续按下紧急报警时，需要间隔一段时间在报,这个间隔时间取报警间隔的五分之一
		 Shake_JianGE = JinJI_timeout/5; //连发生紧急时，需要间隔一段时间在报,这个间隔时间取报警间隔的五分之一
   }
	  
/***读取紧急报警时间间隔*************************/	 
	 
//电子围栏半径
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,R_rail_addr,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if((temp_16 ==0xFFFF)||(temp_16 < 0x0a))  //电子围栏，取值范围
	 {
     temp[0] =0;   
		 temp[1] =0x00; //默认电子围栏半径是0米，即不使用
		 SPI_Flash_SafeWrite(temp,R_rail_addr,0x02);
		 R_rail = 0 ;			 
   }
	 else
	 {     
		 R_rail = temp_16 ;		
   }
	 
 //超速值
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,Over_Speed_addr,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if((temp_16 ==0xFFFF)||(temp_16 < 0x0a))  //超速值，取值范围
	 {
     temp[0] =0;   
		 temp[1] =0x00; //默认超速值为0，即不使用
		 SPI_Flash_SafeWrite(temp,Over_Speed_addr,0x02);
		 Over_Speed = 0 ;			 
   }
	 else
	 {     
		 Over_Speed = temp_16 ;		
   }
	 
 //超速持续时间	 
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,Over_Speed_XDTime_addr,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if((temp_16 ==0xFFFF)||(temp_16 < 0x0a))  //超速值，取值范围
	 {
     temp[0] =0;   
		 temp[1] =0x00; //默认超速值为0，即不使用
		 SPI_Flash_SafeWrite(temp,Over_Speed_XDTime_addr,0x02);
		 Over_Speed_XDTime = 0 ;			 
   }
	 else
	 {     
		 Over_Speed_XDTime = temp_16 ;		
   }
	 
 //车辆所在省域ID
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,Province_ID_addr,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if(temp_16 ==0xFFFF)//||(temp_16 < 0x0a))  //省域ID
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
	 
 //车辆所在市域ID
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,City_ID_addr,0x02);
	 temp_16 = (temp[0]<<8)|temp[1] ;
   if(temp_16 ==0xFFFF)//||(temp_16 < 0x0a))  //省域ID
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
	 
	//车牌照长度，车牌具体内容
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,Vehicle_license_Plate_Len_addr,0x01);
	 bytetemp = temp[0] ;  //(temp[0]<<8)|temp[1] ;
   if(bytetemp >14)  //车牌长度超过14就不对
	 {
      //车牌错误的处理方法 
   }
	 else   //车牌长度正确，读取车牌的具体内容
	 {     
		 memset(temp,0x00,20);	 
     SPI_Flash_SafeRead(Vehicle_license_Plate,Vehicle_license_Plate_addr,bytetemp);  //将车牌直接读到车牌存储区里面 
   }
	 
	//车牌颜色
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,Vehicle_license_Plate_colour_addr,0x01);
	 bytetemp = temp[0] ;  //(temp[0]<<8)|temp[1] ;
   if(bytetemp == 0xFF)  //车牌颜色值为0xFF,表示车牌颜色没有被存储
	 {
      //车牌颜色错误处理方法 
   }
	 else   //车牌颜色正确，读取车牌颜色的具体内容	  
     Vehicle_license_Plate_colour = bytetemp;	
  
 //GNSS定位模式
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,GNSS_MODE_addr,0x01);
	 bytetemp = temp[0] ;  //(temp[0]<<8)|temp[1] ;
   if(bytetemp >3)  //车牌颜色值为0xFF,表示车牌颜色没有被存储
	 {
      //车牌颜色错误处理方法 
   }
	 else   //车牌颜色正确，读取车牌颜色的具体内容	  
     GNSS_MODE = bytetemp;	
 
 //GNSS波特率
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,GNSS_Baud_addr,0x01);
	 bytetemp = temp[0] ;  //(temp[0]<<8)|temp[1] ;
   if(bytetemp >0x05)  //车牌颜色值为0xFF,表示车牌颜色没有被存储
	 {
      //车牌颜色错误处理方法 
   }
	 else   //车牌颜色正确，读取车牌颜色的具体内容	  
     GNSS_Baud = bytetemp;	
	 
 //GNSS模块详细定位数据输出频率
	 memset(temp,0x00,20);	 
   SPI_Flash_SafeRead(temp,GNSS_Hz_addr,0x01);
	 bytetemp = temp[0] ;  //(temp[0]<<8)|temp[1] ;
   if(bytetemp >0x04)  //车牌颜色值为0xFF,表示车牌颜色没有被存储
	 {
      //车牌颜色错误处理方法 
   }
	 else   //车牌颜色正确，读取车牌颜色的具体内容	  
     GNSS_Hz = bytetemp;
		 
   
}
 



















