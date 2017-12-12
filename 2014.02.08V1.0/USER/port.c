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
#include "port.h"

void PORT_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //使能ABCIO口
/*****GPIOA*********/	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PA1：GSM电源控制，1：GSM电源接通
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.1
 //GPIO_SetBits(GPIOA,GPIO_Pin_11);						 //PB.5 输出高
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PA11：断油电控制脚
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 	//开漏输出，5V兼容
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 GPIO_WriteBit(GPIOA,GPIO_Pin_1|GPIO_Pin_11,Bit_RESET); //管脚初始化为0；PA1-GSM3.3V控制脚，1,供电; 0,断电；
 //GPIO_SetBits(GPIOA,GPIO_Pin_11);
	
/*****GPIOB*********/		
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PB0--GSM的RING_N脚
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.0
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_15;				 //PB1--GSM的POWER_ON脚。PB15--GPS的电源控制脚
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.1和15
 GPIO_WriteBit(GPIOB,GPIO_Pin_1|GPIO_Pin_15,Bit_SET);   //PB1，PB15都设为1
 
/*****GPIOC*********/	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PC1，12V电源检测，断电报警 1：有12V，  0:12V断电报警。
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;    //下拉输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.0
 
/*PC3改为用中断驱动了 2014.9.18
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //PC3，劫警检测，1：无劫警，0:有劫警
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //上拉输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.0
*/

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8;				 //PC6,PC8, GPS,GSM的指示灯
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC
 GPIO_WriteBit(GPIOC,GPIO_Pin_6|GPIO_Pin_8,Bit_RESET);   //PC6,PC8, GPS,GSM的指示灯。初始化为0，灯灭。

}

void IOCheck()
{
  //紧急报警
	
}






