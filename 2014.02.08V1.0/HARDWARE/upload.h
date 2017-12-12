/******************************************************************************************
*  File Name                             :uplod.h
*  Copyright                             :2014.8 wei
*  Module Name                           :Init port
*
*  CPU                                   :STM32F103RC
*  RTOS                                  :NULL
*
*  Create Date                           :2014/8/24
*  Author                                :wei
*  Abstract Dscription                   :
*
*
*----------------------Revision History-----------------------------
*  No   Version   Date        Revised By          Item           Description
*
*
*******************************************************************************/
#ifndef __UPLOAD_H
#define __UPLOAD_H
#include "stm32f10x.h"
#include "GSM.h"
#include "sys_struct.h"
#include "flash.h"
//#include "sys.h"
//#include "main.h"

//#define TRUE   1
//#define FALSE  0


extern void Terminal_authentication(void); //�ն�ע�ắ��
extern void change_ben_ji_hao(u8 *in_p);  //�ѱ����żӵ����ϴ��İ���
extern void add_liu_shui_hao(u8 *in_p);    //�����ˮ���ӳ���
extern u8 add_check_number(u8 *in_p, u16 len); //��Ӱ�У���롣������u8 *in_p:��ʼָ��; u16 len:�������ݵĳ���
//����ϴ�
//������
//u8 *out_p�����İ����ݴ�ŵ�ַ; u8 *in_p:�������ʼָ��; u16 len:���������
//���أ����������ݳ��ȣ�
extern u16 package(u8 *out_p,u8 *in_p, u16 len); //����ϴ� ������u8 *out_p�����İ����ݴ�ŵ�ַ; u8 *in_p:�������ʼָ��; u16 len:��������� 
                                                 //���أ����������ݳ��ȣ�
																								 
																								 //���յ��Ĳ������ݽ�����
//0x7d 0x02-->0x7e
//0x7d 0x01-->0x7d
//������*p_in:����ǰ������λ�õ���ʼ��ַ��  *p_out:���������ݵĴ��λ�õ���ʼ��ַ�� len_in:����ǰ���ݵĳ���
//����ֵ��
extern u16 Bubiao_Reci_7E_Analysis(u8 *p_in,u8 *p_out,u16 len_in);
extern u8  Send_authentication(void); //����ע����Ϣ
extern void Terminal_JianQuan(void); //�ն˼�Ȩ����
extern u8  Send_JianQuan(void); //�ն˼�Ȩ����
extern void Terminal_JianQuan_authentication(void);  //���ն˽���ע��ͼ�Ȩ
extern u8  Head_deal(Head in_head_mesg,u8 *p_in); //��Ϣͷ������

extern u8 Upload_Heart(void);//�ϴ���������
extern u8 Send_Heart(void);  //������������
extern u8 Upload_Location(void); ////�ϴ�λ����Ϣ��������������Ϣ

//32λ����ת����4��8λ���ŵ���������
//������u32 in, ��Ҫת����32λ��,
//      u8 *out_p, ����ת����4���ֽڵ�����
//˵��, ���������4�ֽ�32λ��������������ĸ��ֽڵ����飬��32λ���ĵ�λ��������ĸ�Ԫ���У�������������ֱ�ӷָ�ġ�
extern void u32_to_u8arry(u32 in, u8 *out_p);

//����λ����Ϣ�ķ��ͺ���
//��������ֵ��TRUE:���ͳɹ�������ɹ�ָ�����յ�GSMģ�鷵�ص�OK
extern u8 send_upload(u8 type);

//����״̬�����仯�������ϴ�λ����Ϣ
//�Ƕ���������Ĳ���OBD_Com_Flag.OBD_BodyState_Turn_Flag
extern void BodyChang_UpLoad(void);

//λ����Ϣ��ѯӦ�� ID
//��������ֵ��TRUE:���ͳɹ�������ɹ�ָ�����յ�GSMģ�鷵�ص�OK
//���������u8 *p_in:Ӧ����ˮ�ŵĵ�ַ
extern u8 response_upload(u8 *p_in);

//�ն�ͨ��Ӧ����
//��������Ǹ��ṹ�壬������Ӧ����ˮ�ţ�Ӧ��ID��Ӧ����
//type: 0:�ɹ�/ȷ��;  1:ʧ��;  2:��Ϣ����;  3:��֧��
extern u8 Terminal_TongYong_response(REsponse TongYongResp);


extern volatile u8 ben_ji_hao[6] ;//��������BCD[6]
extern volatile u8 upload_arry[512] ;//�ϴ���Ϣ�����
extern volatile u8 xiaoxi_head[13] ;//��Ϣͷ��������
extern volatile u8 JianQuanMa[20] ;  //��ż�Ȩ��.��Ȩ��ĳ����Ƕ�̬�ģ�����������󳤶���20�ֽڣ�����һ����6���ֽڡ�
extern volatile u8 JianQuanMa_Len ;   //�յ��ļ�Ȩ��ĳ���
extern volatile u16 liu_shui  ;//��ˮ��
extern volatile u16 message_ID ;
extern volatile u16 message_property;

extern volatile u32 alarm_flag ; //λ����Ϣ�еģ�������־
extern volatile u32 state_flag ; //λ����Ϣ�еģ�״̬λ

extern u16 R_rail ; //Χ���뾶���Ƿ���λ�뾶
extern u16 Over_Speed ;//����ֵ
extern u16 Over_Speed_XDTime ; //��������ʱ�䣬������ά��ʱ�䳬����ֵʱ�����г��ٱ���
extern u8 Province_ID[2] ; //��������ʡ��ID
extern u8 City_ID[2] ; //������������ID


extern u8 Vehicle_license_Plate_colour  ;  //������ɫ������JT/T415-2006��5.4.12
extern u8 Vehicle_license_Plate_Len  ; //�����ճ���
extern u8 Vehicle_license_Plate[14] ; //������

extern u8 GNSS_MODE ; //GNSS��λģʽ bit0,0:����GPS��λ, 1:����GPS��λ;   bit1, 0:���ñ�����λ, 1:���ñ�����λ
extern u8 GNSS_Baud  ; //GNSS������  ���������ʼ��Ϊ0xFF,��Ϊ��0x00�о���ĺ���; 0x00:4800; 0x01:9600; 0x02:19200; 0x03:38400; 0x04:57600; 0x05:115200
extern u8 GNSS_Hz  ;  //GNSSģ����ϸ��λ�������Ƶ�� GNSSģ����ϸ��λ�������Ƶ��, 0x00:500ms; 0x01:1000ms(Ĭ��ֵ); 0x02:2000ms; 0x03:3000ms; 0x04:4000ms

#endif
















