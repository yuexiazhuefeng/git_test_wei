/******************************************************************************************
*  File Name                             :prot.h
*  Copyright                             :2014.8 wei
*  Module Name                           :Init port
*
*  CPU                                   :STM32F103RC
*  RTOS                                  :NULL
*
*  Create Date                           :2014/8/22
*  Author                                :wei
*  Abstract Dscription                   :
*
*
*----------------------Revision History-----------------------------
*  No   Version   Date        Revised By          Item           Description
*
*
*******************************************************************************/
#ifndef _SYS_STRUCT_H
#define _SYS_STRUCT_H

#include "stm32f10x.h"
#include "ring.h"
typedef struct 
{
     unsigned int GPSHead : 1 ; //"$GPRMC"  ��־
     unsigned int GPSreceiv_complete : 1 ; //GPS���ݽ�����ɱ�־
	   unsigned int GPSreceiv_eorr : 1 ;  //GPS���ݽ��մ���
     unsigned int GPSdeal_complete : 1 ; //GPS���ݴ������
	   unsigned int debugmodel : 1; //��������������ģʽ
	   unsigned int debughead  : 1; //����ָ��ͷ��־
	   unsigned int PC_Command : 1; //���յ�PC���͵Ĳ�������ָ��
     unsigned int OK : 1;  	//���յ�OK��־
	   unsigned int OK_O : 1; //���յ�O,
	   unsigned int SISW : 1; //���յ��ɷ������ݵķ���
	   unsigned int SISR : 1; //��������
	   unsigned int RING : 1; //�е绰����
	   unsigned int RING_S : 1;//���յ�RING
	   unsigned int jieting : 1;//������־ 
	   unsigned int guaji   : 1;//�һ���־
	   unsigned int COPS   : 1;//�ڼ��������Ӫ��
	   unsigned int haveGSMdata : 2 ;//GSM���������ݣ����ȡ��
	   unsigned int GSMData_Dealing :1 ;//0:û�����ݴ���  1:���ڴ���GSM���ݣ���Ҫ��GSM_Reciev[]������д���ݡ�
	   unsigned int Subpackage : 1;  //��Ϣ�ְ���־��SUB=1���ְ�,  UNSUB =0���޷ְ�
	   unsigned int GSM_Reciev_Flag : 1; //GSM_Reciev[]��������ܷ���ܴ����յ������ݣ���־��
	   unsigned int Terminal_auth_Flag :1; //�ж��ն��Ƿ�ע��ɹ���TRUE:�ɹ�; FALSE:ʧ��
		 unsigned int Terminal_JianQuan_Flag :1; //�ж��ն��Ƿ��Ȩ�ɹ��ɹ���TRUE:�ɹ�; FALSE:ʧ��
		 unsigned int SendHeart_Flag : 1 ; //����������־��1:��������;  0:�յ�����Ӧ��
		 unsigned int JinJi_Flag : 1; //����������־  1:�н�������,��ƽ̨û���˹����;  0:û�н�������
		 unsigned int JinJi_Switch_Flag : 1; //������������״̬ 1:������������  0:������������û��
		 unsigned int SendLocation_Flag : 1 ; // λ����Ϣ�ϴ���־, 1:λ����Ϣ��������û�յ�ƽ̨Ӧ��; 0:�յ�ƽ̨Ӧ��
		 unsigned int oil_LperH_average_Flag : 1 ; // ƽ���ͺĲ�����ɣ��ȴ���ȡ��
     unsigned int JinJi_Exti_Rising_flag : 1; //������������������ƽ����PC3����������ƽ
		 unsigned int Shake_Flag : 1;   //���𶯱�־
		 unsigned int Upload_flag : 1;  //�ϴ�λ����Ϣ���ʱ�䵽����Ҫ�ϴ�λ����Ϣ
		 unsigned int CEND_Flag :1 ;    //���յ�^CEND:1,6,67,16 ��ʾͨ������
		 unsigned int NO_CARRIER : 1;   //���յ�NO CARRIER����ʾͨ������
		 unsigned int CONN_Flag : 1;    //���յ�CONN,��ʾͨ����ͨ
		 unsigned int CALL_Flag : 1;    //���ű�־����ʾ�ն����ڽ��в��Ų���
		 unsigned int ORIG_Flag : 1;    //��ʾGSMģ���Ѿ��������ŵĶ�����
		 unsigned int NO_CARRIER_Flag : 1;    //ͨ�������ţ�����
		 unsigned int CONF_Flag : 1;    //ͨ�������ţ�����
		 unsigned int Check_Net_Flag : 1; //�������ͨ·�Ƿ�����������ͨ�������������״̬���
		 unsigned int NO_ANSWER :1; //��ʱ���˽�����
		 unsigned int Check_Net_Send :1; //��Check_Net()�Ӻ���������
		 unsigned int GM301_CPMS :1 ;//�ڷ���AT+CPMS?\r\n
		 unsigned int SM_FLAG : 1;//�ж�����Ҫ����
		 unsigned int GM301_CMGR : 1; //��ȡ����
		 unsigned int GM301_CMGR_UNread : 1; //����δ��,0:�Ѷ�,����Ҫ����; 1:δ������Ҫ����
		 unsigned int UART1_RING :2;//GSM��RING��
} GLOBALstruct_Flag;

extern GLOBALstruct_Flag  Flag_all;  

typedef struct 
{
//  u8  ben_ji_hao[6] ;//������,���ֽ�Ϊ��λ���������棬�ڴ�����ʱ����Խ�ʡһ���Ŀռ�
	u16 liu_shui ; //��ˮ��
  u16 message_ID ; //��ϢID
  u16 message_property ; //��Ϣ������	
	//����Ӧ�û����Ϣ��װ���  �ܰ���  �� �����
} Head;

extern Head Head_mesg;

typedef struct 
{
	u8 pliu_shui[2] ; //��ˮ��
  u8 pmessage_ID[2] ; //��ϢID
  u8 type ; //��Ϣ������	
	//����Ӧ�û����Ϣ��װ���  �ܰ���  �� �����
} REsponse;

extern REsponse TongYongResp;





#endif



