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
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��ABCIO��
/*****GPIOA*********/	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PA1��GSM��Դ���ƣ�1��GSM��Դ��ͨ
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.1
 //GPIO_SetBits(GPIOA,GPIO_Pin_11);						 //PB.5 �����
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PA11�����͵���ƽ�
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 	//��©�����5V����
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 GPIO_WriteBit(GPIOA,GPIO_Pin_1|GPIO_Pin_11,Bit_RESET); //�ܽų�ʼ��Ϊ0��PA1-GSM3.3V���ƽţ�1,����; 0,�ϵ磻
 //GPIO_SetBits(GPIOA,GPIO_Pin_11);
	
/*****GPIOB*********/		
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PB0--GSM��RING_N��
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.0
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_15;				 //PB1--GSM��POWER_ON�š�PB15--GPS�ĵ�Դ���ƽ�
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.1��15
 GPIO_WriteBit(GPIOB,GPIO_Pin_1|GPIO_Pin_15,Bit_SET);   //PB1��PB15����Ϊ1
 
/*****GPIOC*********/	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PC1��12V��Դ��⣬�ϵ籨�� 1����12V��  0:12V�ϵ籨����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;    //��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.0
 
/*PC3��Ϊ���ж������� 2014.9.18
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //PC3���پ���⣬1���޽پ���0:�нپ�
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.0
*/

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8;				 //PC6,PC8, GPS,GSM��ָʾ��
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC
 GPIO_WriteBit(GPIOC,GPIO_Pin_6|GPIO_Pin_8,Bit_RESET);   //PC6,PC8, GPS,GSM��ָʾ�ơ���ʼ��Ϊ0������

}

void IOCheck()
{
  //��������
	
}






