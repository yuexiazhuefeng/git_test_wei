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

#include "stm32f10x_tim.h"
#include "timer2.h"
#include "main.h"

u32 gps_counter = 1;//GPS初始化，等待2S计数
u32 delaycounter = 0;
u16 Debug_waitcounter = 1;
u8 jinji_xiaodou_timer =0 ;//紧急报警消抖定时器
u8 OneSecond_Timer =0 ; //开辟一个1S定时器，
u16 PB0_high_timer;  //PB0高电平计时
u16 PB0_low_timer;   //PB0低电平计时
//u16 FiveSecond = 0; //5S定时器

void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM2初始化 
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向下计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM2, ENABLE);  //使能TIMx					 
}
//定时器2中断服务程序
void TIM2_IRQHandler(void)   //10ms发生一次TIM2中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
			
			if(gps_counter-- < 1)
			{
         gps_counter = 1;
      }
			if(Debug_waitcounter-- <1 )
			{
        Debug_waitcounter = 1;
      }
			if(ReTimeout++ >7) //定时 70ms，延时保证设置参数指令接收完成
			{
				ReTimeout = 7;
       
      }
			if(ms100++ >10) //100ms定时器，将清狗动作放到while外面。
			{
        ms100 = 10;
      }
			if(timer2_100ms++>500)//测试用定时器
			{
        timer2_100ms=500;
      }
			if(USART1_Rx_timer++ >3)
			{
        USART1_Rx_timer =3;
      }
			if(oil_sampling_time++ > 300)
			   oil_sampling_time = 300 ;
			
			if(USART3_Rx_timer++>20)
				USART3_Rx_timer =20;
			
			if(read_GSM_time++ >250)
				 read_GSM_time=250;
			
			if(authentication_count++ >1000 ) //终端重复注册时间周期，暂定10s
				authentication_count =1001;
			
      if(Debug_Timeout++ >30000) //进入工厂参数设置时间计时，5min
         Debug_Timeout =30000;	
//短信检测S			
			if((Flag_all.UART1_RING == 1)||(Flag_all.UART1_RING == 3)) //低电平
			{
				if(!PBin(0))
				{
					if(Flag_all.UART1_RING == 3)
					{  //有电话呼入
						if((PB0_high_timer<405)&&(PB0_high_timer > 395)&&(PB0_low_timer>95))
						{
              Flag_all.UART1_RING = 0;
						  //电话呼入标志置一
            }
					  else Flag_all.UART1_RING = 0;
						if((PB0_high_timer<395)&&(PB0_high_timer>10))//有效高电平时间过短
							Flag_all.UART1_RING = 0;
					}
          else if(PB0_low_timer++>105)//低电平最大长度是1S//&&(PB0_high_timer<405))
					{  //低电平过长
						Flag_all.UART1_RING = 0;
					}					
        }				        
      }
			else if(Flag_all.UART1_RING == 2) //高电平
			{ //接收到短信					
				if(PBin(0))
				{
          if((PB0_high_timer>405)&&(PB0_low_timer >95))
					{
						Flag_all.SM_FLAG = HAVE_SM;  //短信标志置1
						Flag_all.UART1_RING = 0;  
						PB0_low_timer = 0;
						PB0_high_timer = 0;
					}	
          if((PB0_high_timer++>405)||(PB0_low_timer <95))
             Flag_all.UART1_RING = 0 ;						
        }				      
      }
			if(Flag_all.UART1_RING==0) // 通过将Flag_all.UART1_RING置零来清零相关位
			{
        Flag_all.UART1_RING = 0;  
				PB0_low_timer = 0;
				PB0_high_timer = 0;
      }
//短信检测E		
			
			if(Flag_all.JinJi_Exti_Rising_flag)//外部中断接收到了PC3脚有电平变化，如果检测到PC3脚有电平变化
			{
        //if(PCin(3)) //紧急报警按下 PC3管脚是高(3.3v)--有紧急报警 ，PC3管脚是低(0v)--正常，没有紧急报警
				//if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)) //紧急报警按下 PC3管脚是高(3.3v)--有紧急报警 ，PC3管脚是低(0v)--正常，没有紧急报警
				if(GPIOC->IDR&((u32)GPIO_Pin_3)) //紧急报警按下 PC3管脚是高(3.3v)--有紧急报警 ，PC3管脚是低(0v)--正常，没有紧急报警
				{
					if(jinji_xiaodou_timer++>3)// 
					{
						Flag_all.JinJi_Flag = 1; //有紧急报警
						Flag_all.JinJi_Exti_Rising_flag = 0 ; //把边沿判断标志清零
						
						if(OBD_Com_Flag.ACC_state) //如果ACC为开
						{
              if(LocationUptime_count++ >LocationUptime) //上传位置信息，计时器
				        LocationUptime_count = LocationUptime;
            }
						else
						{
              if(LocationUptime_count++ >LocationUptime) //上传位置信息，计时器
				        LocationUptime_count = LocationUptime;
            }
						
					}
				}
				else
				{
					//if(JinJi_LianXucount++
					jinji_xiaodou_timer =0;
					Flag_all.JinJi_Exti_Rising_flag = 0 ; //把边沿判断标志清零
				}
      }			
//			if(JinJi_LianXucount++ >= JinJi_JianGe) //连续紧急报警的时间间隔
//				 JinJi_LianXucount = JinJi_JianGe; //
			
						
				
				
			
			if(Read_oilmass_time >=200) //定时2S
				Read_oilmass_time =200;
			
			if(JinJI_count++ >= JinJI_timeout) //上传紧急报警时间间隔到
				JinJI_count = JinJI_timeout;
			
		  if(Shake_Num)
			{
        if(Shake_continu_timer++ >5)
				{
          if(Shake_Num > 2)
					{
            Flag_all.Shake_Flag = 1;
						Shake_Num =0;
						Shake_continu_timer =0;
          }
					else
					{
            Shake_Num = 0;
						Shake_continu_timer =0;
          }
        }
      }
			if(Shake_LianXuCount++ >=Shake_JianGE)
				Shake_LianXuCount = Shake_JianGE;
			if(OneSecond_Timer++ > 100) //1S定时器，10ms会有一个定时中断，产生100次中断即是1S
			{
        OneSecond_Timer =0; //清空1s定时器，重新开始计时.
				//心跳计时
				if(upheartime_count++ > upheartime) //心跳计时， 可配置，默认40s
				  upheartime_count = upheartime;
			
        //位置信息上传时间计时				
				if(OBD_Com_Flag.ACC_state)	//ACC开的状态
				{
					if(LocationUptime_count++ >LocationUptime)
					{
						 LocationUptime_count = 0;//上传位置信息，计时器
						 Flag_all.Upload_flag =1; //上传位置信息间隔时间到，需要上传位置信息
					}	
				}
				else
				{
					if(LocationUptime_count++ >ACCOFF_LocationUptime)
					{
						 LocationUptime_count = 0;//上传位置信息，计时器
						 Flag_all.Upload_flag =1; //上传位置信息间隔时间到，需要上传位置信息
					}	
				}
				
				//紧急报警，定时器
				if(JinJI_count++ >= JinJI_timeout) //上传紧急报警时间间隔到
				  JinJI_count = JinJI_timeout;
				
				if(CALL_Timer++>200) //开始呼叫后，就计时，防止程序死在呼叫循环里
					CALL_Timer = 200;
				
				//心跳超时计时
				if(upheart_timeout_count++ > upheart_timeout) //心跳应答超时，可配置，默认300s = 5min;
				  upheart_timeout_count = upheart_timeout;
				//
				if(Check_Net_timeover++>10)
					Check_Net_timeover=11;
				
				if(test_OBD_PID_T++>10) //测试OBD PID读取
					test_OBD_PID_T = 10;
      }
			
      
				//LED1=!LED1;
	}
}
