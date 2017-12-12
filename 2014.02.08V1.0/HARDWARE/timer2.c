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

u32 gps_counter = 1;//GPS��ʼ�����ȴ�2S����
u32 delaycounter = 0;
u16 Debug_waitcounter = 1;
u8 jinji_xiaodou_timer =0 ;//��������������ʱ��
u8 OneSecond_Timer =0 ; //����һ��1S��ʱ����
u16 PB0_high_timer;  //PB0�ߵ�ƽ��ʱ
u16 PB0_low_timer;   //PB0�͵�ƽ��ʱ
//u16 FiveSecond = 0; //5S��ʱ��

void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM2��ʼ�� 
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���¼���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
}
//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)   //10ms����һ��TIM2�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
			
			if(gps_counter-- < 1)
			{
         gps_counter = 1;
      }
			if(Debug_waitcounter-- <1 )
			{
        Debug_waitcounter = 1;
      }
			if(ReTimeout++ >7) //��ʱ 70ms����ʱ��֤���ò���ָ��������
			{
				ReTimeout = 7;
       
      }
			if(ms100++ >10) //100ms��ʱ�������幷�����ŵ�while���档
			{
        ms100 = 10;
      }
			if(timer2_100ms++>500)//�����ö�ʱ��
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
			
			if(authentication_count++ >1000 ) //�ն��ظ�ע��ʱ�����ڣ��ݶ�10s
				authentication_count =1001;
			
      if(Debug_Timeout++ >30000) //���빤����������ʱ���ʱ��5min
         Debug_Timeout =30000;	
//���ż��S			
			if((Flag_all.UART1_RING == 1)||(Flag_all.UART1_RING == 3)) //�͵�ƽ
			{
				if(!PBin(0))
				{
					if(Flag_all.UART1_RING == 3)
					{  //�е绰����
						if((PB0_high_timer<405)&&(PB0_high_timer > 395)&&(PB0_low_timer>95))
						{
              Flag_all.UART1_RING = 0;
						  //�绰�����־��һ
            }
					  else Flag_all.UART1_RING = 0;
						if((PB0_high_timer<395)&&(PB0_high_timer>10))//��Ч�ߵ�ƽʱ�����
							Flag_all.UART1_RING = 0;
					}
          else if(PB0_low_timer++>105)//�͵�ƽ��󳤶���1S//&&(PB0_high_timer<405))
					{  //�͵�ƽ����
						Flag_all.UART1_RING = 0;
					}					
        }				        
      }
			else if(Flag_all.UART1_RING == 2) //�ߵ�ƽ
			{ //���յ�����					
				if(PBin(0))
				{
          if((PB0_high_timer>405)&&(PB0_low_timer >95))
					{
						Flag_all.SM_FLAG = HAVE_SM;  //���ű�־��1
						Flag_all.UART1_RING = 0;  
						PB0_low_timer = 0;
						PB0_high_timer = 0;
					}	
          if((PB0_high_timer++>405)||(PB0_low_timer <95))
             Flag_all.UART1_RING = 0 ;						
        }				      
      }
			if(Flag_all.UART1_RING==0) // ͨ����Flag_all.UART1_RING�������������λ
			{
        Flag_all.UART1_RING = 0;  
				PB0_low_timer = 0;
				PB0_high_timer = 0;
      }
//���ż��E		
			
			if(Flag_all.JinJi_Exti_Rising_flag)//�ⲿ�жϽ��յ���PC3���е�ƽ�仯�������⵽PC3���е�ƽ�仯
			{
        //if(PCin(3)) //������������ PC3�ܽ��Ǹ�(3.3v)--�н������� ��PC3�ܽ��ǵ�(0v)--������û�н�������
				//if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)) //������������ PC3�ܽ��Ǹ�(3.3v)--�н������� ��PC3�ܽ��ǵ�(0v)--������û�н�������
				if(GPIOC->IDR&((u32)GPIO_Pin_3)) //������������ PC3�ܽ��Ǹ�(3.3v)--�н������� ��PC3�ܽ��ǵ�(0v)--������û�н�������
				{
					if(jinji_xiaodou_timer++>3)// 
					{
						Flag_all.JinJi_Flag = 1; //�н�������
						Flag_all.JinJi_Exti_Rising_flag = 0 ; //�ѱ����жϱ�־����
						
						if(OBD_Com_Flag.ACC_state) //���ACCΪ��
						{
              if(LocationUptime_count++ >LocationUptime) //�ϴ�λ����Ϣ����ʱ��
				        LocationUptime_count = LocationUptime;
            }
						else
						{
              if(LocationUptime_count++ >LocationUptime) //�ϴ�λ����Ϣ����ʱ��
				        LocationUptime_count = LocationUptime;
            }
						
					}
				}
				else
				{
					//if(JinJi_LianXucount++
					jinji_xiaodou_timer =0;
					Flag_all.JinJi_Exti_Rising_flag = 0 ; //�ѱ����жϱ�־����
				}
      }			
//			if(JinJi_LianXucount++ >= JinJi_JianGe) //��������������ʱ����
//				 JinJi_LianXucount = JinJi_JianGe; //
			
						
				
				
			
			if(Read_oilmass_time >=200) //��ʱ2S
				Read_oilmass_time =200;
			
			if(JinJI_count++ >= JinJI_timeout) //�ϴ���������ʱ������
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
			if(OneSecond_Timer++ > 100) //1S��ʱ����10ms����һ����ʱ�жϣ�����100���жϼ���1S
			{
        OneSecond_Timer =0; //���1s��ʱ�������¿�ʼ��ʱ.
				//������ʱ
				if(upheartime_count++ > upheartime) //������ʱ�� �����ã�Ĭ��40s
				  upheartime_count = upheartime;
			
        //λ����Ϣ�ϴ�ʱ���ʱ				
				if(OBD_Com_Flag.ACC_state)	//ACC����״̬
				{
					if(LocationUptime_count++ >LocationUptime)
					{
						 LocationUptime_count = 0;//�ϴ�λ����Ϣ����ʱ��
						 Flag_all.Upload_flag =1; //�ϴ�λ����Ϣ���ʱ�䵽����Ҫ�ϴ�λ����Ϣ
					}	
				}
				else
				{
					if(LocationUptime_count++ >ACCOFF_LocationUptime)
					{
						 LocationUptime_count = 0;//�ϴ�λ����Ϣ����ʱ��
						 Flag_all.Upload_flag =1; //�ϴ�λ����Ϣ���ʱ�䵽����Ҫ�ϴ�λ����Ϣ
					}	
				}
				
				//������������ʱ��
				if(JinJI_count++ >= JinJI_timeout) //�ϴ���������ʱ������
				  JinJI_count = JinJI_timeout;
				
				if(CALL_Timer++>200) //��ʼ���к󣬾ͼ�ʱ����ֹ�������ں���ѭ����
					CALL_Timer = 200;
				
				//������ʱ��ʱ
				if(upheart_timeout_count++ > upheart_timeout) //����Ӧ��ʱ�������ã�Ĭ��300s = 5min;
				  upheart_timeout_count = upheart_timeout;
				//
				if(Check_Net_timeover++>10)
					Check_Net_timeover=11;
				
				if(test_OBD_PID_T++>10) //����OBD PID��ȡ
					test_OBD_PID_T = 10;
      }
			
      
				//LED1=!LED1;
	}
}
