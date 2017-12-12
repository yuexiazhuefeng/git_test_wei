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

#ifndef __USART_H
#define __USART_H

#include <string.h>
#include <ctype.h>

#include "sys.h" 
#include "stm32f10x_usart.h" 
#include "timer2.h"
#include "IWDG.h"
#include "port.h"
#include "OBD.h"
#include "sys_struct.h"

#define  latitude_GPS   0x00   //γ��
#define  longitude_GPS  0x04   //����
#define  elevation_GPS  0x08   //�߳�
#define  velocity_GPS   0x0A   //�ٶ�
#define  direction_GPS  0x0C   //����
#define  UTC_GPS        0x0E   //ʱ��



   

/********sirf4 GPS��ʼ��ָ��*******************************/
//const    unsigned char sirf4_cmd00[] = {"$PSRF103,00,00,00,01*24\r\n"};    //$PSRF103,00,00,00,01*24
//const    unsigned char sirf4_cmd01[] = {"$PSRF103,01,00,00,01*25\r\n"};    //$PSRF103,01,00,00,01*25
//const    unsigned char sirf4_cmd02[] = {"$PSRF103,02,00,00,01*26\r\n"};    //$PSRF103,02,00,00,01*26
//const    unsigned char sirf4_cmd03[] = {"$PSRF103,03,00,00,01*27\r\n"};    //$PSRF103,03,00,00,01*27
//const    unsigned char sirf4_cmd04[] = {"$PSRF103,04,00,00,01*20\r\n"};    //$PSRF103,04,00,00,01*20
//const    unsigned char sirf4_cmd05[] = {"$PSRF103,05,00,00,01*21\r\n"};    //$PSRF103,05,00,00,01*21
//const    unsigned char sirf4_cmd06[] = {"$PSRF103,06,00,00,01*22\r\n"};    //$PSRF103,06,00,00,01*22
/********sirf4 GPS��ʼ��ָ��*******************************/
//const  u8 AT[4]={"AT\r\n"};

void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void Usart1_SendData(u8 * psend, u8 length);
void Usart2_SendData(u8 * psend, u8 length);
void Usart3_SendData(u8 * psend, u8 length);
void GPS_Flag_Clear (void); //GPS��־���
void sirf4Config(void);//sirf4 GPS��ʼ������

extern void wait(u32 count); //��С1ms

u8 change_gpsdata(void);
u32 shiliutoshi(u32 ret,u8 *p);

extern volatile u8 sendlengthU1;
extern u8 * psendU1;
extern volatile u8 sendlengthU2;
extern u8 * psendU2;
extern volatile u8 sendlengthU3;
extern u8 * psendU3;

extern u8 usart1_Rx_buffer[166] ; //GSM�����ݶ�ֻ��140����
extern u8 usart1_Rx_length ;    //��ǰ���յ����ݳ��ȣ���λ���ֽ�
extern u8 OBD_Package_len ;     //OBD���ݰ�����İ�������Ϣ
extern u8 usart1_Rx_backup[50] ; //�������������Ŵ���3�����е����ݣ��Ա����洦���á�
extern u8 USART1_Rx_timer ;      //��ʱ30ms��������մ���1���ջ��棬��ֹ���ţ���һЩ����Ҫ�ķ�������


extern u8 usart2_Rx_buffer[90];//GPS���ջ���
extern u8 usart2_Rx_number;

extern u8 usart3_Rx_buffer[1100] ; //GSM�����ݶ�ֻ��140����
extern u16 usart3_Rx_length ;
extern u8 usart3_Rx_backup[1100]; //�������������Ŵ���3�����е����ݣ��Ա����洦���á�
extern u16 usart3_Rx_backup_length;
extern u8 usart3_Rx_SM[150] ;  //���Ż���
extern u8 usart3_Rx_SM_len ;     //���ն��ų���
extern u8 USART3_Rx_timer ; //��ʱ30ms��������մ���3���ջ��棬��ֹ���ţ���һЩ����Ҫ�ķ�������

extern u8 GPS_DATA[20] ; //����������ڽ���������
extern u8 GPS_DATA_BACKUP[20] ;//�������ݽ����õ����� 
extern u8 GSM_Reciev[1100];//GSM���յ����Է�����������
extern u16 GSM_Reciev_len ;//GSM���յ����Է����������ݳ���
extern u8  GSM_Ring_Number[16];//������ʾ����
extern u8  GSM_Ring_Number_len; //����������ʾ�ĺ��볤��

/*
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
} GLOBALstruct_Flag;

extern GLOBALstruct_Flag  Flag_all;  

*/
#endif

