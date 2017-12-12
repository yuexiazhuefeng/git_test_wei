/******************************************************************************************
*  File Name                             :EXTI.c
*  Copyright                             :2014.9.18 wei
*  Module Name                           :Init port
*
*  CPU                                   :STM32F103RC
*  RTOS                                  :NULL
*
*  Create Date                           :2014/9/18
*  Author                                :wei
*  Abstract Dscription                   :
*
*
*----------------------Revision History-----------------------------
*  No   Version   Date        Revised By          Item           Description
*
*
*******************************************************************************/
#include "exti.h"

void Exit_INIT(void)
{
/*************PC3--	��������*******///
	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);  //����PC��AFIO����ʱ�ӣ�ֻҪ�ø��ù��ܣ�AFIO����ʱ�ӱ���Ҫ�򿪣�
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_IN_FLOATING;  //�������룬�Լ���ģ�����������������ӹ��ģ��������ܲ����ж�����״̬
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//GPIOC.3 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);  //PC3

  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;  //ѡ���ж���3
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	 //ģʽѡ���жϣ� �������¼�
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //����Ϊ������
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	//����PC3�ж�
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//����EXTI3
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
/************************************************************************************************************/		
//PB0-UART1_RING		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_IN_FLOATING;  //�������룬�Լ���ģ�����������������ӹ��ģ��������ܲ����ж�����״̬
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//GPIOC.3 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);  //PC3

  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;  //ѡ���ж���3
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	 //ģʽѡ���жϣ� �������¼�
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //����Ϊ�������½���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	//����PC3�ж�
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//����EXTI3
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);
/***********************************************************************************************************/	
//PA8�𶯱���			
		
		//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);  //����PC��AFIO����ʱ�ӣ�ֻҪ�ø��ù��ܣ�AFIO����ʱ�ӱ���Ҫ�򿪣�
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_IN_FLOATING;  //�������룬�Լ���ģ�����������������ӹ��ģ��������ܲ����ж�����״̬
	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	//GPIOA.8 �ж����Լ��жϳ�ʼ������
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);  //PA6��ֻ��Ҫѡ��˿�(A-G)���ж���(0-15),�����й̶��Ķ�Ӧ��ϵ

		EXTI_InitStructure.EXTI_Line=EXTI_Line8;  //ѡ���ж���3
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	 //ģʽѡ���жϣ� �������¼�
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //����Ϊ�½���
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
	//����PA8���ж�ͨ����EXTI9-5����һ���ж�ͨ������������Ҳ����һ���ж���ں���EXTI9_5_IRQHandler
	//������жϷ������У���ȥ�жϾ������ĸ��ж�Դ�������ж�
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//����EXTI3
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ�2�� 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					//�����ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
		NVIC_Init(&NVIC_InitStructure); 
}

void EXTI3_IRQHandler(void)  //PC3�жϷ������
{
   
  if(EXTI_GetITStatus(EXTI_Line3) != RESET)	  //���ָ����EXTI3��·�������������
	{	  
		Flag_all.JinJi_Exti_Rising_flag = 1;  //�б��ر仯
	}
	EXTI_ClearITPendingBit(EXTI_Line3);  //���EXTI0��·����λ
}

void EXTI0_IRQHandler(void)  //PC0�жϷ������GSM RING�ż�⣬�ж��Ƿ��ж��ź͵绰
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)	  //���ָ����EXTI0��·�������������
	{	  
		if(!PBin(0)) //���ŵ͵�ƽ
		{
      if(PB0_low_timer>95)
				Flag_all.UART1_RING = 3;  //��⵽�ڶ������ر仯���½���
			else
				Flag_all.UART1_RING = 1;  //�б��ر仯���½���
    }		 
	  else if(PBin(0)) //���Ÿߵ�ƽ
		 Flag_all.UART1_RING = 2;  //�б��ر仯��������
	}
	EXTI_ClearITPendingBit(EXTI_Line0);  //���EXTI0��·����λ

}
void EXTI9_5_IRQHandler (void)
{
  if(EXTI_GetITStatus(EXTI_Line8) != RESET)	  //���ָ����EXTI3��·�������������
	{	  
		if(Shake_Num++ >20)   //�б��ر仯 ,���20��Ϊ�ñ���Shake_Num�ܿأ����ȡ��ֵ��ֻҪ���ڶ�ʱ�����������ֵ������
		  Shake_Num =20; 
	}
	EXTI_ClearITPendingBit(EXTI_Line8);  //���EXTI0��·����λ

}



