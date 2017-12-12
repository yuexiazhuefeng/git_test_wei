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
/*************PC3--	紧急报警*******///
	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);  //开启PC，AFIO复用时钟，只要用复用功能，AFIO复用时钟必须要打开，
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_IN_FLOATING;  //浮空输入，自己想的，如果是上拉可能增加功耗，下拉可能不易判断输入状态
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//GPIOC.3 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);  //PC3

  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;  //选择中断线3
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	 //模式选择中断， 而不是事件
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //配置为上升沿
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	//配置PC3中断
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//操作EXTI3
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
/************************************************************************************************************/		
//PB0-UART1_RING		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_IN_FLOATING;  //浮空输入，自己想的，如果是上拉可能增加功耗，下拉可能不易判断输入状态
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//GPIOC.3 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);  //PC3

  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;  //选择中断线3
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	 //模式选择中断， 而不是事件
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //配置为上升和下降沿
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	//配置PC3中断
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//操作EXTI3
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);
/***********************************************************************************************************/	
//PA8震动报警			
		
		//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);  //开启PC，AFIO复用时钟，只要用复用功能，AFIO复用时钟必须要打开，
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_IN_FLOATING;  //浮空输入，自己想的，如果是上拉可能增加功耗，下拉可能不易判断输入状态
	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	//GPIOA.8 中断线以及中断初始化配置
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);  //PA6，只需要选择端口(A-G)和中断线(0-15),他们有固定的对应关系

		EXTI_InitStructure.EXTI_Line=EXTI_Line8;  //选择中断线3
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	 //模式选择中断， 而不是事件
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //配置为下降沿
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
	//配置PA8的中断通道，EXTI9-5共用一个中断通道，所以他们也共用一个中断入口函数EXTI9_5_IRQHandler
	//在这个中断服务函数中，再去判断具体是哪个中断源产生的中断
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//操作EXTI3
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级2， 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					//子优先级1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
		NVIC_Init(&NVIC_InitStructure); 
}

void EXTI3_IRQHandler(void)  //PC3中断服务程序
{
   
  if(EXTI_GetITStatus(EXTI_Line3) != RESET)	  //检查指定的EXTI3线路触发请求发生与否
	{	  
		Flag_all.JinJi_Exti_Rising_flag = 1;  //有边沿变化
	}
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除EXTI0线路挂起位
}

void EXTI0_IRQHandler(void)  //PC0中断服务程序，GSM RING脚检测，判断是否有短信和电话
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{	  
		if(!PBin(0)) //引脚低电平
		{
      if(PB0_low_timer>95)
				Flag_all.UART1_RING = 3;  //检测到第二个边沿变化，下降沿
			else
				Flag_all.UART1_RING = 1;  //有边沿变化，下降沿
    }		 
	  else if(PBin(0)) //引脚高电平
		 Flag_all.UART1_RING = 2;  //有边沿变化，上升沿
	}
	EXTI_ClearITPendingBit(EXTI_Line0);  //清除EXTI0线路挂起位

}
void EXTI9_5_IRQHandler (void)
{
  if(EXTI_GetITStatus(EXTI_Line8) != RESET)	  //检查指定的EXTI3线路触发请求发生与否
	{	  
		if(Shake_Num++ >20)   //有边沿变化 ,这个20是为让变量Shake_Num受控，随便取的值，只要大于定时器里面的限制值就行了
		  Shake_Num =20; 
	}
	EXTI_ClearITPendingBit(EXTI_Line8);  //清除EXTI0线路挂起位

}



