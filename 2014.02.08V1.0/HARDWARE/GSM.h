

#ifndef __GSM_H
#define __GSM_H

#include <stdlib.h>
#include "stm32f10x.h"
#include "sys.h"
#include "iwdg.h"
#include "usart.h"
#include "upload.h"
#include "sys_struct.h"
#include "alarm.h"


#define AT_Len          4
#define ATE0_Len        6
#define CPIN_Len        10  
#define CMGD_Len        13
#define CLIP_Len        11
#define CMIC_Len        12
#define CREG_Len        10
#define CGREG_Len       11

#define COPS_Len        10
#define CSQ_Len         8
#define SICS_con_Len    29
//#define SISS_conId_Len  19 //MG323
#define SISS_conId_Len  21 //MG301
//#define SISS_srv_Len    26 //MG323
#define SISS_srv_Len    30 //MG323
#define IOMODE_Len      15
#define SISO_Len        11
#define SISR_Len        16
#define GM301_CMGF_Len  11
#define GM301_CPMS_Len  10

#define power_on    PAout(1) = 1
#define power_off   PAout(1) = 0
#define TERM_ON     PBout(1)

#define SISW_OK 2

#define SISR_1  3 //SISR:0,1 �յ�GSM�����ϱ���GSM������������Ҫ��ȡ��
#define SISR_7E_OK 2 //^SISR:0,24, �յ�ƽ̨�����Ĳ�������7E
#define SISR_7E_EMPTY 1 //SISR:0,0 ����������
#define SISR_7E_UN 0 //û���յ�SISR�����ݡ�

#define TRUE   1
#define FALSE  0

#define SUB   1 //�·������ݱ��ְ���
#define UNSUB  0 //�·�������û�зְ�

#define CEND_NORMAL     0x01 // GSMͨ����������
#define CEND_UNUSUAL    0x02 // GSMͨ��û�н�ͨ�͹Ҷ���
#define CALL_ERROR      0x03 //����ʮ��û�յ�ORIG��CONF

#define HAVE_SM 0x01  //GSMģ���յ����ţ���Ҫ��Ƭ��ȥ��
#define REC_SM  0x02  //��Ƭ�����յ�����


extern void delay_Nx100ms(u8 n); //100ms��ʱ�������幷�����ŵ�while���档
extern u8 GSM_startup(void); //GSM�ϵ粢����AT�жϿ����Ƿ�ɹ����ɹ�:����1;   ʧ��:����0;
extern u8 GSM_Send_ATCommnd(u8 *p,u8 length,u8 delay_count);  //GSM ATָ����Ӻ��������Ͳ��ɹ��Ļ�������λ��ᡣ�ɹ�:����1; ʧ��:����0
extern u8 GSM_shutdown(void); //GSM�ػ�����,�ɹ�:����1;   ʧ��:����0;
extern u8 GSM_INIT(void); //GSM��ʼ������
extern u8 CREG_CGREG(void); //���GSMģ���Ƿ�ע����GSM��GPRS����
extern u8 MG301_SM_Init(void);  //MG301���ų�ʼ��
extern void GSM_Module_Start(void); //
extern u8 GPRS_Connect(void); //GPRS����
//extern u8 GSM_Send_Data(u8 *p,u8 sendlen,u8 *pid,u8 idlen);//*p:Ҫ���͵������������ݣ� *pid:��ӡ������2�����ݣ����涼�Ƕ�Ӧ�����ݳ���
extern u8 GSM_Send_Data(u8 *p,u8 sendlen);//*p:Ҫ���͵�������������
extern u8 GSM_Recivdata_deal(void);//Զ�����ݴ�����
extern void GSM_Ring(void);//���ͨ��
extern u8 Read_GSM(void); //��ȡGSM���ݺ���������ʱ����2.5s�����յ�GSM���ȡ��־
extern u8 GSM_Read_AT_SISR( u8 *p,u8 length,u8 delay_count); //ָ��ɹ�����1; ���ɹ�����0
extern u8 GSM_CALL(u8 * p_number, u8 number_len) ;//����绰����
extern void Check_Net(void); //����״̬������ͨ�����������ã�ͨ��������������ͨ·
extern u8 GM301_CPMS_deal(void); //AT+CPMS?  ������
//extern void Read_SM(char *p,u8 param_len); //���Ŷ�ȡ����
extern void Read_SM(u8 add); //���Ŷ�ȡ���� 
extern void SM_Data_Deal(void);
extern void ASCII2HEX(u8 *p_in,u8 *p_out,u8 len) ;//'AF'-->0xAF;
extern void SM_Deal() ;//���Ŵ����ܺ���������������������ȡ��ÿ�����ŵĴ�������ȫ��ɾ��
extern void Delete_AllSM(void); //����ɾ��������ɾ��������ȫ������

extern u8 ms100;//100ms��ʱ�������幷�����ŵ�while���档
extern u8  GSM_Reciev[1100];//GSM���յ����Է�����������
extern u16  GSM_Reciev_len ;//GSM���յ����Է����������ݳ���
extern u8  GSM_Ring_Number[16];//������ʾ����
extern u8  GSM_Ring_Number_len; //����������ʾ�ĺ��볤��

extern u8 temp_GSM_Reciev[1024] ; //��ʱ���ת��������
extern u16 temp_len ;  //ת�������ݳ���

extern u32 Body_state ;
extern u32 alarm_state ; //������־���ĸ��ֽ�3
//extern u32 mileage_km; //��̱���������ֳ��Ϳ��ܲ�֧��
//extern u16 oil_mass; //ʣ���������������ʣ���������������ֽڱ�ʾ������0x8000Ϊ%��ʾ��С��0x8000ΪL��ʾ
extern u8 oil_LperH_average; //L/H,������ƽ��ֵ
extern u8 oil_LperH; //L/H,��ǰ����ֵ,���ֵ�� 0xDF
extern u16 LocationUptime ; //�ϴ�λ����Ϣ��ʱ����
extern u16 ACCOFF_LocationUptime ;//ACC �ص�״̬��λ����Ϣ�ϱ�ʱ����
extern u32 upheartime ; //�����ϴ����
extern u16 upheart_timeout;//����Ӧ��ʱ
extern u8 oil_LperH_F; //�ϴ������ÿСʱ�ͺĵĲ�������

extern u16 upheartime_count; //����ʱ���ʱ
extern u16 upheart_timeout_count ;  //����Ӧ��ʱ��Ĭ����Ϊ5min

extern u16 LocationUptime_count ; //λ����Ϣ�ϴ���ʱ����

extern u16 JinJI_timeout ; //��������ʱ����
extern u16 JinJI_count ; //�������� ��ʱ��
extern u16 JinJi_LianXucount ; //�������½�������ʱ,�������

extern u16 CEND_time ; //��ȡCEND����������ͨ��ʱ�䣬�����0˵��ͨ��û�б���ͨ
extern u8 CALL_Timer ; //��绰�������õ��Ķ�ʱ��

extern u8 Check_Net_timeover ; //�����⣬��ʱ��λ

extern u8 PID_user_defined[103];  //PID�û��Զ���
extern u8 PID_user_defined_number ;//PID�û��Զ�����Ŀ��

#endif

