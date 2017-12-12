/******************************************************************************************
*  File Name                             :prot.h
*  Copyright                             :2014.8 wei
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
#ifndef __OBD_H
#define __OBD_H


#include "sys.h" 
#include "usart.h"
#include "GSM.h"
#include "ring.h"

#define LOCK    1
#define UNLOCK  0

#define BD_compile_data_len        6
#define OBD_Versionsi_len          6
#define OBD_Read_VIN_len           6
#define OBD_Read_SN_len            6
#define Read_Sleep_Parameter_len   6
#define Close_Sleep_Parameter_len  8
#define OBD_mileage_len            7
#define Read_oil_mass_len          7
#define Read_oil_LperH_len         7
#define Read_Body_state_len        6



#define OBD_compile_datai       0x33      //0x33OBDģ������ı�������
#define Versionsi               0x40      //0x40 OBDģ��汾��Ϣ
#define Support_PIDi            0x41      //0x41OBD֧�ֵ�PID������
#define OBDII_PIDi              0x42      //0x42��ȡOBDII PID������
#define Custom_PIDi             0x43      //0x43��ȡ�Զ���PID������
#define Read_Body_Statei        0x44      //0x44������״̬��Ϣ
#define Support_Freeze_Framei   0x45      //0x45��ȡ֧�ֵĶ���֡
#define Read_Freeze_Framei      0x46  	  //0x46��ȡ����֡
#define Read_Fault_Codei        0x47      //0x47��ȡ������
#define Clear_Fault_Codei       0x48      //0x48���յ��ɷ������ݵķ���
#define Read_VINi               0x49      //0x49��ȡ����VIN��
#define Set_Vehicle_Modeli      0x4B      //0x4B�궨����
#define Read_SNi                0x50      //0x50 ��ȡ��Ʒ���к�(SN),MCU��Ψһ��ʶ
#define Read_Main_Faulti        0x56      //0x56ȫ����Ҫϵͳ����ɨ��
#define Read_Faulti             0x57      //0x57��ȡĳһ��ϵͳ�Ĺ�����
#define Clear_Faulti            0x58      //0x58���ĳһ��ϵͳ�Ĺ�����
#define Body_Controli           0x23      //0x23�����������ƣ��ƣ��������пص�
#define Set_Sleep_Parameteri    0x24      //0x24����˯�߲���
#define Read_Sleep_Parameteri   0x25      //0x25��ȡ˯�߲���

#define OBD_compile_data_len  0x06  //��ȡ����ʱ��ָ���

#define H_TO_L 1
#define L_TO_H 2

#define OBD_BUF_LEN 521


typedef struct
{
     unsigned int OBD_compile_data : 1 ; //0x33OBDģ������ı�������
     unsigned int Versions : 1 ; //0x40 OBDģ��汾��Ϣ
	   unsigned int Support_PID : 1 ;  //0x41OBD֧�ֵ�PID������
     unsigned int OBDII_PID : 1 ; //0x42��ȡOBDII PID������
	   unsigned int Custom_PID : 1; //0x43��ȡ�Զ���PID������
	   unsigned int Read_Body_State  : 1; //0x44������״̬��Ϣ
	   unsigned int Support_Freeze_Frame : 1; //0x45��ȡ֧�ֵĶ���֡
     unsigned int Read_Freeze_Frame : 1;  	//0x46��ȡ����֡
	   unsigned int Read_Fault_Code : 1; //0x47��ȡ������
	   unsigned int Clear_Fault_Code : 1; //0x48���յ��ɷ������ݵķ���
	   unsigned int Read_VIN : 1; //0x49��ȡ����VIN��
	   unsigned int Set_Vehicle_Model : 1; //0x4B�궨����
	   unsigned int Read_SN : 1;//0x50 ��ȡ��Ʒ���к�(SN),MCU��Ψһ��ʶ
	   unsigned int Read_Main_Fault : 1;//0x56ȫ����Ҫϵͳ����ɨ��
	   unsigned int Read_Fault   : 1;//0x57��ȡĳһ��ϵͳ�Ĺ�����
	   unsigned int Clear_Fault   : 1;//0x58���ĳһ��ϵͳ�Ĺ�����
	   unsigned int Body_Control : 1;//0x23�����������ƣ��ƣ��������пص�
	   unsigned int Set_Sleep_Parameter   : 1;//0x24����˯�߲���
	   unsigned int Read_Sleep_Parameter   : 1;//0x25��ȡ˯�߲���
	   unsigned int PID_UpMode  : 2;  //PID�������ϴ���ʽ��0,Ĭ�ϣ�ȫ�����ϴ���; 1,�Ƽ�����PID���ϴ�; 2,ȫ�� 3,ȫ��
	   unsigned int OBD_Head : 1; //��������û�н��յ� 0x55 0xAA�ı�־ 1,�ѽ��յ�0x55 0xAA;  0,δ���յ�0x55 0xAA; 
		 unsigned int OBD_Received_Complete :1 ; //OBD�ʴ����ݽ������
		 unsigned int OBD_Received_Body : 1; //���յ������ϱ��ĳ���״̬��Ϣ
		 unsigned int OBD_Error : 1 ;  //���ص�7F����ָ��
	   unsigned int alarm :1;  //�����־��1:���뾯��;  0:�˳�����
		 unsigned int door : 1; //�Ƿ����ű�־
		 unsigned int brake : 1;//�Ƿ�ɲ����־
		 unsigned int ACC_alarm :1; //�Ƿ�ACC��־
		 unsigned int ACC_state :1; //�Ƿ�ACC��־
		 unsigned int OBD_error_type :2; //OBD���ش���ָ�0x00,У����͸�ʽ����; 0x01,�����ֲ�֧��; 0x02,�������ò���ȷ��֧��, 0x03,������ͨ��ʧ��
		 unsigned int OBD_Mileage_Support_FLag : 1 ;//֧����̶�ȡ�ı�־��0:֧��, 1:��֧�� ,Ĭ�϶���0����֧��
		 unsigned int OBD_Mileage_Receivded_FLag : 1 ;//��̶�ȡ��ɱ�־  1:��ȡ���   0:��ȡδ���
		 unsigned int OBD_oil_massSupport_FLag : 1 ;//֧��ʣ���ȡ�ı�־��0:֧��, 1:��֧�� ,Ĭ�϶���0����֧��
		 unsigned int OBD_oil_massReceivded_FLag : 1 ;//ʣ��������ȡ��ɱ�־  1:��ȡ���   0:��ȡδ���
		 unsigned int OBD_ACC_Turn : 2; //H_TO_L �ɿ���ɹ�,  L_TO_H �ɹر�ɿ�
		 unsigned int OBD_ReadBodyState_Flag : 1 ; // OBD��ȡ����״̬��Ϣ��MCU����ȥ����־
		 unsigned int OBD_BodyState_Turn_Flag : 1 ; // OBD�����ϱ�����״̬����ʱ�жϳ���״̬�б仯
		 unsigned int Have_alarm :  1 ; //�о����־
		 unsigned int FDoor_State_Flag : 1 ; //��״̬
		 unsigned int MDoor_State_Flag : 1 ; //��״̬
		 unsigned int BDoor_State_Flag : 1 ; //��״̬
		 unsigned int ACC_State_Flag : 1 ; //ACC״̬
		 unsigned int Brake_State_Flag : 1 ; //ɲ��״̬
		 unsigned int Shake_State_Flag : 1 ; //��״̬
		 //unsigned int Door_State_Flag : 1 ; //�о�����֣�
		 unsigned int OBD_Received_PID : 1; //�յ�����PID������
		 unsigned int OBD_PID_complete : 1; //PID�������������
} OBD_Flag;  //OBD������ȡ��־

extern OBD_Flag  OBD_Com_Flag; 

extern void OBD_init(void); //OBD��ʼ������
extern void OBD_Data_Deal(void);
extern void OBD_Error_Judge(void);   //����OBD����7F������Ϣ���ж���������
extern void OBD_Error_Judge_zi(void);  //����OBD����7F������Ϣ���ж���������00,01,02,03
extern u8 Mark_Car(u8 high, u8 low);  //�궨���ͺ���
extern u8 Body_Control(u8 comm, u8 parameter);  //������ƺ�����
extern u8 OBD_Sleep(u8 type,u8 com,u8 parameter);//˯�����ú���
extern void Body_state_deal(void);//����״̬��ȡ��������־��ȡ��
extern void mileage(void) ;//��ȡ������̣�����̱����
extern void Read_oil_mass(void); //��ȡʣ�����������������ֽڵı���oil_mass
extern u8 Read_oil_LperH(void); //��ȡL/H˲ʱ�ͺģ�����Ǹ���һ��֮�����ۼӺ����������ÿСʱ�ͺ�
extern void oil_average_LperH(void);  //ƽ���ͺļ������
extern void OBD_Read_Body_state(void); //��ȡ����״̬��Ϣ

//��OBDII PID����������
//������
//p_PID:PID��ŵ�ַ
//len_PID:��ȡPID����
extern void Read_PID(u8 *p_in,u8 len_in);

extern u16 oil_sampling_time ;  //L/H,����ʱ�䣬��Ϊ3��

extern u8 OBD_mileage_buffer_arry[4]; //��̴洢���飬��ȡ��������ĸ�char���͵����ݣ�ֱ��ת�嵽���������
extern u8 OBD_oil_mass_buffer_arry[2];//ʣ�������������飬��ȡ��ʣ������ֵ��2��char���͵����ݣ�ֱ��ת�Ʒŵ����������

extern u8 OBD_PID_Buffer[OBD_BUF_LEN];
extern RING_T ring_obd_buffer; //OBD���滷
#endif

