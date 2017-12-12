/******************************************************************************************
*  File Name                             :upload.c
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
#include "upload.h"

Head Head_mesg; //��Ϣͷ��ʼ���ṹ��
REsponse TongYongResp;//ͨ��Ӧ��������Ҫ�Ĳ����ýṹ������֯

volatile u8 ben_ji_hao[6]={0};//{0x01,0x83,0x56,0x95,0x78,0x28}; //��������BCD[6]
volatile u8 upload_arry[512] ={0}; //�ϴ���Ϣ�����
volatile u8 xiaoxi_head[13]={0}; //��Ϣͷ��������
volatile u8 JianQuanMa[20] = {0};  //��ż�Ȩ��.��Ȩ��ĳ����Ƕ�̬�ģ�����������󳤶���20�ֽڣ�����һ����6���ֽڡ�
volatile u8 JianQuanMa_Len = 0;   //�յ��ļ�Ȩ��ĳ���
volatile u16 liu_shui =0; //��ˮ��
volatile u16 message_ID =0; //��ϢID
volatile u16 message_property =0; //��Ϣ������
u16 authentication_count =0; //�ն��ظ�ע��ʱ������ ������Ϊ10s

volatile u32 alarm_flag = 0; //λ����Ϣ�еģ�������־
volatile u32 state_flag = 0; //λ����Ϣ�еģ�״̬λ

u16 R_rail = 0; //Χ���뾶���Ƿ���λ�뾶
u16 Over_Speed = 0;//����ֵ
u16 Over_Speed_XDTime = 0; //��������ʱ�䣬������ά��ʱ�䳬����ֵʱ�����г��ٱ���
u8 Province_ID[2] = {0}; //��������ʡ��ID
u8 City_ID[2] = {0}; //��������ʡ��ID


u8 Vehicle_license_Plate_colour = 0 ;  //������ɫ������JT/T415-2006��5.4.12
u8 Vehicle_license_Plate_Len  = 0; //�����ճ���
u8 Vehicle_license_Plate[14] ={0}; //�����գ��֧��14λ 

u8 GNSS_MODE=0; //bit0,0:����GPS��λ, 1:����GPS��λ;   bit1, 0:���ñ�����λ, 1:���ñ�����λ
u8 GNSS_Baud = 0xFF; //���������ʼ��Ϊ0xFF,��Ϊ��0x00�о���ĺ���; 0x00:4800; 0x01:9600; 0x02:19200; 0x03:38400; 0x04:57600; 0x05:115200
u8 GNSS_Hz = 0xFF;  //GNSSģ����ϸ��λ�������Ƶ��, 0x00:500ms; 0x01:1000ms(Ĭ��ֵ); 0x02:2000ms; 0x03:3000ms; 0x04:4000ms
//u8 upload_arry[512] ={0};

//�ն�ע��
//7E 01 00 00 36 01 31 00 00 00 20 00 01 00 22 00 64 37 30 34 34 34 46 48 54 2D 47 50 53 2F 50 4B 45 39 30 30 44 00 00 00 00 00 44 31 31 39 38 30 39 00 44 45 46 41 55 4C 54 2D 56 49 4E 39 39 39 39 39 39 17 7E
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 
                   //0   1   2    3     4   5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23
//u8 test_arry[69]={0x7E,0x01,0x00,0x00,0x36,0x01,0x31,0x00,0x00,0x00,0x20,0x00,0x01,0x00,0x22,0x00,0x64,0x37,0x30,0x34,0x34,0x34,\
//                  0x46,0x48,0x54,0x2D,0x47,0x50,0x53,0x2F,0x50,0x4B,0x45,0x39,0x30,0x30,0x44,0x00,0x00,0x00,0x00,0x00,0x44,0x31,\
 //                 0x31,0x39,0x38,0x30,0x39,0x00,0x44,0x45,0x46,0x41,0x55,0x4C,0x54,0x2D,0x56,0x49,0x4E,0x39,0x39,0x39,0x39,0x39,0x39,0x17,0x7E};
void Terminal_authentication(void) //�ն�ע�ắ��
{
	u8 temp_authentication_result =0;
	u8 temp_0x55 =0x55;
 /* u16 temp_len =0;  	
	//                                                                                                           0x30      0x34 
	                  //0     1   2    3     4   5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23
  u8 temp_zhuce[69]={0x7E,0x01,0x00,0x00,0x36,0x01,0x02,0x03,0x04,0x05,0x06,0x00,0x00,0x00,0x22,0x00,0x64,0x37,0x7e,0x34,0x7d,0x34,0x46,0x48,\
                     0x54,0x2D,0x47,0x50,0x53,0x2F,0x50,0x4B,0x45,0x39,0x30,0x30,0x44,0x00,0x00,0x00,0x00,0x00,0x44,0x31,0x31,0x39,0x38,0x30,\
		                 0x39,0x00,0x44,0x45,0x46,0x41,0x55,0x4C,0x54,0x2D,0x56,0x49,0x4E,0x39,0x39,0x39,0x39,0x39,0x39}; //����ע�������
  change_ben_ji_hao(temp_zhuce+5); //�ӱ����ŵ���Ϣͷ�С�
	add_liu_shui_hao(temp_zhuce+11); //�����ˮ��			 
	temp_zhuce[68] = 0x7E; //��ӽ�β��ʶ��
	temp_zhuce[67] = add_check_number(temp_zhuce,69);//���У����												 
  temp_len = package(upload_arry,temp_zhuce,69);										 
  GSM_Send_Data(upload_arry,temp_len); //�������ݵ�������				 */
	while(1)
	{
    if(Send_authentication()) //�ж�ע����Ϣ�Ƿ��ͳɹ���GSM����OK
	  {
			authentication_count = 0;
			while(authentication_count<1000)
			{
				IWDG_Feed();
        if((read_GSM_time>=250)||(Flag_all.haveGSMdata==SISR_1)) //��ʱ�������յ�GSM�����ϱ�
				{
          read_GSM_time =0;
					Flag_all.haveGSMdata = 0;
					if(Read_GSM()) //�յ�7E���ݣ�Ҫ����
					{
            temp_authentication_result=GSM_Recivdata_deal(); //����
						temp_authentication_result -= 5;
						if((0==temp_authentication_result)||(3==temp_authentication_result))  //ע��ɹ���Ŀǰ������ֻ����0��û���������
						{
							SPI_Flash_SafeWrite((u8 *)(&(JianQuanMa_Len)),JianQuanMa_addr,0x01); //�洢��Ȩ�볤��
							SPI_Flash_SafeWrite((u8 *)JianQuanMa,JianQuanMa_addr+1,JianQuanMa_Len);//�洢��Ȩ��
							SPI_Flash_SafeWrite((u8 *)(&(temp_0x55)),Terminal_auth_Flag_addr,0x01);//�洢ע��ɹ���־
							Flag_all.Terminal_auth_Flag = TRUE;					//�ɹ���־��1�����洢
							return ;  //�ɹ�
						}
          }
        }
      }
    }
  }		
}

u8 Send_authentication(void) //����ע����Ϣ
{
  u16 temp_len =0;  	
	//                                                                                                           0x30      0x34 
	                  //0     1   2    3     4   5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23
  u8 temp_zhuce[69]={0x7E,0x01,0x00,0x00,0x36,0x01,0x02,0x03,0x04,0x05,0x06,0x00,0x00,0x00,0x22,0x00,0x64,0x37,0x7e,0x34,0x7d,0x34,0x46,0x48,\
                     0x54,0x2D,0x47,0x50,0x53,0x2F,0x50,0x4B,0x45,0x39,0x30,0x30,0x44,0x00,0x00,0x00,0x00,0x00,0x44,0x31,0x31,0x39,0x38,0x30,\
		                 0x39,0x00,0x44,0x45,0x46,0x41,0x55,0x4C,0x54,0x2D,0x56,0x49,0x4E,0x39,0x39,0x39,0x39,0x39,0x39}; //����ע�������
  change_ben_ji_hao(temp_zhuce+5); //�ӱ����ŵ���Ϣͷ�С�
	add_liu_shui_hao(temp_zhuce+11); //�����ˮ��			 
	temp_zhuce[68] = 0x7E; //��ӽ�β��ʶ��
	temp_zhuce[67] = add_check_number(temp_zhuce,69);//���У����												 
  temp_len = package((u8 *)upload_arry,temp_zhuce,69);	
  upload_arry[temp_len]	= 0x1A;		 //MG323��βҪ��ctrl+z ��ASCII��Ϊ26��0x1a						 
  if(GSM_Send_Data((u8 *)upload_arry,temp_len+1))//�������ݵ�������		
     return 1;
  return 0;

}

//��������ӳ�����Ҫ����ϴ�����������ӱ����š�
//��Ҫ���������in_p:��������Ҫ�ϴ��������е�λ�á�
void change_ben_ji_hao(u8 *in_p)  //�ѱ����żӵ����ϴ��İ���
{
  u8 i =0;
	u8 *tem_p =in_p;
	for(i=0;i<6;i++)
	{
     *(tem_p+i)= ben_ji_hao[i];
  }
}

//�����Ϣ��ˮ��
//�������������Ϊ����ˮ�ŵĴ��ָ��
void add_liu_shui_hao(u8 *in_p)
{
  u8 *temp_p=in_p;
	Head_mesg.liu_shui++;
	*(temp_p)=(u8)(liu_shui>>8);
	*(temp_p+1) = (u8)(liu_shui);
}

//����ϴ�,����ת����7E��ͷ��7E��β������
//������
//u8 *out_p�����İ����ݴ�ŵ�ַ; u8 *in_p:�������ʼָ��; u16 len:��������ȣ���������ǲ�������β��7e�ģ�ֻ��У����
//���أ����������ݳ��ȣ�������ȵ������ǿ���ֱ���ϴ����������ġ�
u16 package(u8 *out_p,u8 *in_p, u16 len)
{
  u8 *temp_in_p=in_p;
	u8 *temp_out_p=out_p;
	//upload_arry = in_p;
	u16 temp_len =len;
	u16 i=0;
	u16 j=1;
	temp_out_p[0] = *temp_in_p; //�����ָ��ֱ�������������á��ѿ�ʼ��7E��ʾ��ֱ�ӷŵ����λ��
	for(i=1;i<len-1;i++,j++) //ȥͷ��β��ȥ��ͷ����ʼ����7E��ʾ��������β�ǣ�У����
	{
    if(((*(temp_in_p+i))==0x7e)||((*(temp_in_p+i))==0x7d))
		{
      if((*(temp_in_p+i))==0x7e)
			{
        temp_out_p[j++]= 0x7d;				
				temp_out_p[j] = 0x02;
      }
			else if((*(temp_in_p+i))==0x7d)
			{
        temp_out_p[j++]= 0x7d;				
				temp_out_p[j] = 0x01;
      } 
    }
		else
		{
      temp_out_p[j]= *(temp_in_p+i);
    }
  }
	temp_out_p[j++] =0x7e;  //��ӽ�β��7E��ʾ��
	return j;
}

//��Ӱ�У���롣
//������
//u8 *in_p:��ʼָ�뼴7E��ָ��; u16 len :��������ݵ��ܶȣ�����7Eͷ��7Eβ��������ȣ���û�н���ת��ǰ�ĳ��ȡ�
//���أ�У����
u8 add_check_number(u8 *in_p, u16 len)
{
  u8 temp_check;
	u8 *temp_p =in_p;
	u16 temp_len =len;
	u16 i =0;
	temp_check= *(temp_p+1);
	for(i=2;i<temp_len-2;i++)
	{
    temp_check= temp_check^(*(temp_p+i));		
  }
  return temp_check;
}

//���յ��Ĳ������ݽ�����
//0x7d 0x02-->0x7e
//0x7d 0x01-->0x7d
//������*p_in:����ǰ������λ�õ���ʼ��ַ��  *p_out:���������ݵĴ��λ�õ���ʼ��ַ�� len_in:����ǰ���ݵĳ���
//����ֵ������ת�������ݳ��ȡ�
u16 Bubiao_Reci_7E_Analysis(u8 *p_in,u8 *p_out,u16 len_in)
{
  u16 i=0;
	u16 j=0;
	if((*p_in==0x7e)&&(*(p_in+len_in-1)==0x7e)) //�ж�ͷβ����0x7e,�Ż�������
	{
		*p_out=0x7e;
    for(i=1,j=1;i<len_in;i++,j++)
		{
      if(*(p_in+i)==0x7d)
			{
				if(*(p_in+(++i))==0x01)
					*(p_out+j)= 0x7d;
				else if(*(p_in+i)==0x02)
					*(p_out+j)= 0x7e;
				else 
					return FALSE;
      }
			else
			  *(p_out+j)= *(p_in+i);
    }		
		return j;
  }
	return FALSE;
}

void Terminal_JianQuan(void) //�ն˼�Ȩ����
{
	u8 temp_authentication_result =0;
	u8 temp_0x55 =0x55;  
	while(1)
	{
    if(Send_JianQuan()) //�ж�ע����Ϣ�Ƿ��ͳɹ���GSM����OK
	  {
			read_GSM_time =0;
			authentication_count = 0;
			while(authentication_count<1000)
			{
				IWDG_Feed();
        if((read_GSM_time>=250)||(Flag_all.haveGSMdata==SISR_1)) //��ʱ�������յ�GSM�����ϱ�
				{
          read_GSM_time =0;
					Flag_all.haveGSMdata = 0;
					if(Read_GSM()) //�ж��Ƿ��յ�7E���ݣ�Ҫ����
					{
            temp_authentication_result=GSM_Recivdata_deal(); //���ô����������յ��Ĳ������ݴ���
						//temp_authentication_result -= 5;
						if(TRUE==temp_authentication_result)  //��Ȩ�ɹ�
						{
							Usart2_SendData("JianQuan success...\r\n",sizeof("JianQuan success...\r\n"));
							return ;  //�ɹ�
						}
          }
        }
      }
    }
  }		
}


//PPP��:[7E 01 02 00 06 01 32 05 14 48 12 00 02 42 59 46 55 55 53 71 7E ]-11:47:50  //�ն˼�Ȩ���鿼����
//       0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 
u8 Send_JianQuan(void) //�ն˼�Ȩ����
{
  u8 temp_jianquan[40] ={0x7e}; //��ʱ�洢��������е����ݣ���ɺ󴫵ݸ��������������� upload_arry[512]���顣
  u8 temp_len = 0;//��ʱ���������ݵĳ���
/*  message_ID = 0x0102; //�ն˼�Ȩ��ϢID��0x0102
  temp_jianquan[1]=0x01;
  temp_jianquan[2]=0x02;//�����ϢID
  message_property = JianQuanMa_Len; //��Ϣ�����Ը�ֵ��������Ϣ��ĳ��Ⱦ��Ǽ�Ȩ��ĳ���
  temp_jianquan[3]=(u8)(message_property>>8);
  temp_jianquan[4]=(u8)(message_property&0x00FF);
*/  
  Head_mesg.message_ID =0x0102; //��ֵ��ϢID
  Head_mesg.message_property = (u16)JianQuanMa_Len; //��ֵ��Ϣ������
  Head_mesg.liu_shui +=1u;
  temp_len = Head_deal(Head_mesg,temp_jianquan);//���ص���Ϣͷ+ 0x7e�ĳ���
  memcpy(temp_jianquan+temp_len,(u8 *)JianQuanMa,JianQuanMa_Len); //��Ӽ�Ȩ��
  temp_len+=JianQuanMa_Len;
  temp_len++;
  //temp_jianquan[temp_len++]=0x7e;
  //temp_len++;
  temp_jianquan[temp_len-1]= add_check_number(temp_jianquan, (temp_len+1)); //���У����
  temp_len=package((u8 *)upload_arry,temp_jianquan,(u16)(temp_len+1));
  if(GSM_Send_Data((u8 *)upload_arry,temp_len))
		return TRUE;
	return FALSE;
}

void Terminal_JianQuan_authentication(void)  //���ն˽���ע��ͼ�Ȩ
{
	while(Flag_all.Terminal_auth_Flag != TRUE)
	{    
		Terminal_authentication();
	}
  
	while(Flag_all.Terminal_JianQuan_Flag != TRUE)
	{    
	 Terminal_JianQuan();  
  }
	
}

//��Ϣͷ������
//���������Head���͵Ľṹ��ָ��
//����������������������Ч���ݵĳ���
//����������Ǵ�����ɺ���Ϣͷ�����ݴ�ŵ�xiaoxi_head[13]�����еģ�Ϊ���øú������������뺯�����������������ʹ����ʱ������
//��Ϣͷ���ݵģ�����Ϣͷ���ݴ�����ɺ���һ���Ե�ת��ȫ�ֱ���������xiaoxi_head[13]�С��䱻�������ͬʱ����ʱ�����ķ���
//   |ID | ����| |�ն��ֻ���     | ��ˮ��
//7E 01 00 00 36 01 83 56 95 78 28 00 01
//0  1  2  3  4  5  6  7  8  9  10 11 12
u8 Head_deal(Head in_head_mesg,u8 *p_in) //��Ϣͷ������
{
  u8 temp_head_arry[13]={0x7e};//�����ʱ��Ͱ�7Eͷ�Ž�ȥ��������ʱ�����Ϣͷ
  Head temp_Head_mesg = in_head_mesg; //�����ݹ����Ľṹ�����ݸ�ֵ�������ʱ�ṹ�壬��������ⲿ�޸���ȫ�ֽṹ�����head_mesg��Ҳ����Ӱ�쵽���
  u8 temp_len =0;    
  

  temp_head_arry[1]=(u8)(in_head_mesg.message_ID>>8);//��ϢID
  temp_head_arry[2]=(u8)(in_head_mesg.message_ID&0x00ff);//��ϢID

  temp_head_arry[3]=(u8)(in_head_mesg.message_property>>8);//��Ϣ������
  temp_head_arry[4]=(u8)(in_head_mesg.message_property&0x00ff);//��Ϣ������

  memcpy(temp_head_arry+5,(u8 *)ben_ji_hao,0x06);//�ն��ֻ��ţ����ڳ��������ڼ䶼�ǲ����ģ��������ڲ�����ʼ����ʱ����Ѿ����ú���

  temp_head_arry[11]=(u8)(in_head_mesg.liu_shui>>8);//��ˮ��
  temp_head_arry[12]=(u8)(in_head_mesg.liu_shui&0x00ff);//��ˮ��
  if(in_head_mesg.message_property&(0x0001<<13)) //�����13λ�ְ���־λ1����ʱ��Ϣͷ���ֽڳ�����14���ֽ�
		temp_len = 14;
	else temp_len = 13;//û����Ϣ����װ���ʱ�ֽ���������13�ֽ�
  memcpy(p_in,temp_head_arry,temp_len); //�Ѵ���õ���Ϣͷ�ŵ����ú���ʱ���ݽ�����ָ�봦
	return temp_len;
}

u8 Upload_Heart(void) ////�ϴ���������
{
  if((Flag_all.SendHeart_Flag)&&(upheart_timeout_count >= upheart_timeout)) //����Ӧ��ʱ
	{
    Usart2_SendData("Heart ERROR Reset MCU...\r\n",sizeof("Heart ERROR Reset MCU...\r\n"));
    SystemReset(); //����һ�������λ
  }
	if(upheartime_count >= upheartime ) //�ϴ�������ʱ��
	{
    upheartime_count =0;
		if(TRUE==Send_Heart())
		{
      Flag_all.SendHeart_Flag =1;  //��������������
      return TRUE;
    }			
		else
			return FALSE;
  }
}


//PPP��:[7E 00 02 00 00 01 32 05 14 48 12 00 04 22 7E ]-11:48:32
u8 Send_Heart(void)  //�ϴ���������
{
  u8 temp_Heart[40] ={0x7e}; //��ʱ�洢��������е����ݣ���ɺ󴫵ݸ��������������� upload_arry[512]���顣
  u8 temp_len = 0;//��ʱ���������ݵĳ���
  Head_mesg.message_ID =0x0002; //��ֵ��ϢID
  Head_mesg.message_property = 0x0000; //��ֵ��Ϣ������
  Head_mesg.liu_shui +=1u;
  temp_len = Head_deal(Head_mesg,temp_Heart);//���ص���Ϣͷ+ 0x7e�ĳ���
  //memcpy(temp_jianquan+temp_len,(u8 *)JianQuanMa,JianQuanMa_Len); //��Ӽ�Ȩ��
  //temp_len+=JianQuanMa_Len;
  temp_len++;
 // temp_Heart[temp_len++]=0x7e;
  //temp_len++;
  temp_Heart[temp_len-1]= add_check_number(temp_Heart, temp_len+1); //���У����
  temp_len=package((u8 *)upload_arry,temp_Heart,(u16)(temp_len+1));
  if(GSM_Send_Data((u8 *)upload_arry,temp_len))
	{
    memset((u8 *)upload_arry,0x00,512); //����ϴ�����
		return TRUE;
  }		
	return FALSE;
}

u8 Upload_Location(void) ////�ϴ�λ����Ϣ��������������Ϣ
{
//  if((Flag_all.SendLocation_Flag)&&(LocationUptime_count >= upheart_timeout)) //����Ӧ��ʱ
//	{
//    Usart2_SendData("Heart ERROR Reset MCU...\r\n",sizeof("Heart ERROR Reset MCU...\r\n"));
//    SystemReset(); //����һ�������λ
//  }
	u16 temp_alarm_flag = alarm_flag;//���汨��λ���ϴ�λ����Ϣʱ����Ҫ�ϴ�����λ����һ����ѱ���λ����
	alarm_flag =0 ; //�ϴ�λ����Ϣʱ����Ҫ�ϴ�����λ������ѱ���λ����
	//if(LocationUptime_count >= LocationUptime) //�ϴ�λ��ʱ�������ˣ�
	if(Flag_all.Upload_flag) //�ϴ�λ����Ϣ���ʱ�䵽����Ҫ�ϴ�λ����Ϣ
	{
    //LocationUptime_count =0;
		Flag_all.Upload_flag =0;
		if(TRUE==send_upload(0)) //����λ����Ϣ
		{
      Flag_all.SendLocation_Flag =1;  //��������������
		  alarm_flag = temp_alarm_flag;//�ѱ�����־��ԭ
      return TRUE;
    }			
		else
			return FALSE;
  }
}

//����λ����Ϣ�ķ��ͺ���
//��������ֵ��TRUE:���ͳɹ�������ɹ�ָ�����յ�GSMģ�鷵�ص�OK
//������� type
//Ĭ�϶�Ϊ0
//1-����λ����Ϣ��Ҫ������λ
//
u8 send_upload(u8 type)
{
//λ�û�����Ϣ�ĳ�����28�ֽ�	
  u8 temp_upload_msgbody_arry[240]={0};//��ʱ�����Ϣ��: λ�û�����Ϣ+������Ϣ���ݶ��䳤����240���ֽ�
  u8 temp_upload_msgbody_Len=0; //��Ϣ�峤��
  u8 temp_upload_arry[255] ={0x7e}; //�ϴ����ݵ����ݴ������
  u8 temp_upload_Len =0; //�ϴ����ݵĳ���

  if(type ==1)
		u32_to_u8arry(alarm_state,temp_upload_msgbody_arry);//��������־
	else if(type ==0)
    u32_to_u8arry(alarm_flag,temp_upload_msgbody_arry);//��������־
  temp_upload_msgbody_Len =4; 

///////////////////��ȡ״̬λ
	if(type ==1)
	{
   // OBD_Com_Flag.OBD_ReadBodyState_Flag = 1; //��������ȡ����״̬λ��1
	 // OBD_Read_Body_state();  //���ö�ȡ����״̬��Ϣ����  
	 // OBD_Com_Flag.OBD_ReadBodyState_Flag = 0; //��������ȡ����״̬λ���
		u32_to_u8arry(Body_state,temp_upload_msgbody_arry+4);//����״̬ 
  }
	else if(type ==0)
	{ 
		OBD_Com_Flag.OBD_ReadBodyState_Flag = 1; //��������ȡ����״̬λ��1
		OBD_Read_Body_state();  //���ö�ȡ����״̬��Ϣ����  
		OBD_Com_Flag.OBD_ReadBodyState_Flag = 0; //��������ȡ����״̬λ���
		u32_to_u8arry(Body_state,temp_upload_msgbody_arry+4);//����״̬ 
  }	
	temp_upload_msgbody_Len+=4;
	memcpy(temp_upload_msgbody_arry+8,GPS_DATA_BACKUP,20); //��������ɵ�GPS���ݷŵ���ʱ�����Ϣ�����������
  temp_upload_msgbody_Len +=20;
////////////////////////////////////////////////////

//��Ӹ����ֶ�
//  if(OBD_Com_Flag.ACC_state) //�����̣�ǰ�ж�ACCΪ�����Ż�������涯��
//	{	 //����������Ÿ�Ϊ�ж�OBD�Ƿ�˯�ߣ����������������	
    mileage();//��ȡ��̺���
		if(OBD_Com_Flag.OBD_Mileage_Receivded_FLag) //��̶�ȡ��ɱ�־�������ǰ������Ϊ�����ֶμӵ�λ���ϱ���Ϣ��
    {
      temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x01;//��Ӹ�����ϢID 0x01--���
			temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x04;//��Ӹ�����Ϣ����  �����4���ֽ�
			memcpy(temp_upload_msgbody_arry+temp_upload_msgbody_Len,OBD_mileage_buffer_arry,4); //���������ݣ�4���ֽ�
      temp_upload_msgbody_Len +=4; //��������ֵ
    }  //��̴������
		
		if(OBD_Com_Flag.OBD_ACC_Turn)//ACC�б仯����Ҫ�ϴ�ʣ��������Ϣ
		{
      if(OBD_Com_Flag.OBD_oil_massReceivded_FLag) //ʣ��������ȡ�ɹ������Է���ʣ����������
			{
        temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0xEB;//��Ӹ�����ϢID 0x02--����,��Ӧ������̱����� 	WORD,1/10L
			  temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x07;//��Ӹ�����Ϣ����  ��չ������Ϣ������7�ֽ�
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x05;//
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x01;//				
			  memcpy(temp_upload_msgbody_arry+temp_upload_msgbody_Len,OBD_oil_mass_buffer_arry,2); //���������ݣ�4���ֽ�
        temp_upload_msgbody_Len +=2; //��������ֵ
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//	
      }
			else
			{
        Read_oil_mass(); //��ȡʣ����������
				if(OBD_Com_Flag.OBD_oil_massReceivded_FLag) //ʣ��������ȡ�ɹ������Է���ʣ����������
			  {
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0xEB;//��Ӹ�����ϢID 0x02--����,��Ӧ������̱����� 	WORD,1/10L
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x07;//��Ӹ�����Ϣ����  ��չ������Ϣ������7�ֽ�
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x05;//
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x01;//				
					memcpy(temp_upload_msgbody_arry+temp_upload_msgbody_Len,OBD_oil_mass_buffer_arry,2); //���������ݣ�4���ֽ�
					temp_upload_msgbody_Len +=2; //��������ֵ
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//	
        }
      }
    }	//�����������	    
 // }
//	if()
//��Ӹ����ֶ�

  Head_mesg.message_ID =0x0200; //��ֵ��ϢID
  Head_mesg.message_property = (u16)temp_upload_msgbody_Len; //��ֵ��Ϣ������
  Head_mesg.liu_shui +=1u; //������ˮ��

  temp_upload_Len = Head_deal(Head_mesg,temp_upload_arry);//���ص���Ϣͷ+ 0x7e�ĳ���
  memcpy(temp_upload_arry+temp_upload_Len,(u8 *)temp_upload_msgbody_arry,temp_upload_msgbody_Len); //��ӻ���λ����Ϣ
  temp_upload_Len+=temp_upload_msgbody_Len; //��ʱtemp_upload_Len��û��ת��ǰ������У����ͽ�β��־0x7e



  temp_upload_arry[temp_upload_Len]= add_check_number(temp_upload_arry, (temp_upload_Len+2)); //���У����
  temp_upload_Len = package((u8 *)upload_arry,temp_upload_arry,(u16)(temp_upload_Len+2)); //���
  if(GSM_Send_Data((u8 *)upload_arry,temp_upload_Len+0))
		return TRUE;
	return FALSE;
}

//λ����Ϣ��ѯӦ�� ID
//��������ֵ��TRUE:���ͳɹ�������ɹ�ָ�����յ�GSMģ�鷵�ص�OK
//���������u8 *p_in:Ӧ����ˮ�ŵĵ�ַ
u8 response_upload(u8 *p_in)
{
//λ�û�����Ϣ�ĳ�����28�ֽ�	
  u8 temp_upload_msgbody_arry[240]={0};//��ʱ�����Ϣ��: λ�û�����Ϣ+������Ϣ���ݶ��䳤����240���ֽ�
  u8 temp_upload_msgbody_Len=0; //��Ϣ�峤��
  u8 temp_upload_arry[255] ={0x7e}; //�ϴ����ݵ����ݴ������
  u8 temp_upload_Len =0; //�ϴ����ݵĳ���

  memcpy(temp_upload_msgbody_arry,p_in,2);//���Ӧ����ˮ��
  temp_upload_msgbody_Len =2;
  u32_to_u8arry(alarm_flag,temp_upload_msgbody_arry+temp_upload_msgbody_Len);//��������־
  temp_upload_msgbody_Len +=4; 

///////////////////��ȡ״̬λ
  OBD_Com_Flag.OBD_ReadBodyState_Flag = 1; //��������ȡ����״̬λ��1
  OBD_Read_Body_state();  //���ö�ȡ����״̬��Ϣ����  
  OBD_Com_Flag.OBD_ReadBodyState_Flag = 0; //��������ȡ����״̬λ���
  u32_to_u8arry(Body_state,temp_upload_msgbody_arry+temp_upload_msgbody_Len);//����״̬
	temp_upload_msgbody_Len+=4;
	memcpy(temp_upload_msgbody_arry+temp_upload_msgbody_Len,GPS_DATA_BACKUP,20); //��������ɵ�GPS���ݷŵ���ʱ�����Ϣ�����������
  temp_upload_msgbody_Len +=20;
////////////////////////////////////////////////////

//��Ӹ����ֶ�
  if(OBD_Com_Flag.ACC_state) //�����̣�ǰ�ж�ACCΪ�����Ż�������涯��
	{		
    mileage();//��ȡ��̺���
		if(OBD_Com_Flag.OBD_Mileage_Receivded_FLag) //��̶�ȡ��ɱ�־�������ǰ������Ϊ�����ֶμӵ�λ���ϱ���Ϣ��
    {
      temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x01;//��Ӹ�����ϢID 0x01--���
			temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x04;//��Ӹ�����Ϣ����  �����4���ֽ�
			memcpy(temp_upload_msgbody_arry+temp_upload_msgbody_Len,OBD_mileage_buffer_arry,4); //���������ݣ�4���ֽ�
      temp_upload_msgbody_Len +=4; //��������ֵ
    }  //��̴������
		
		if(OBD_Com_Flag.OBD_ACC_Turn)//ACC�б仯����Ҫ�ϴ�ʣ��������Ϣ
		{
      if(OBD_Com_Flag.OBD_oil_massReceivded_FLag) //ʣ��������ȡ�ɹ������Է���ʣ����������
			{
        temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0xEB;//��Ӹ�����ϢID 0x02--����,��Ӧ������̱����� 	WORD,1/10L
			  temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x07;//��Ӹ�����Ϣ����  ��չ������Ϣ������7�ֽ�
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x05;//
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x01;//				
			  memcpy(temp_upload_msgbody_arry+temp_upload_msgbody_Len,OBD_oil_mass_buffer_arry,2); //���������ݣ�4���ֽ�
        temp_upload_msgbody_Len +=2; //��������ֵ
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//	
      }
			else
			{
        Read_oil_mass(); //��ȡʣ����������
				if(OBD_Com_Flag.OBD_oil_massReceivded_FLag) //ʣ��������ȡ�ɹ������Է���ʣ����������
			  {
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0xEB;//��Ӹ�����ϢID 0x02--����,��Ӧ������̱����� 	WORD,1/10L
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x07;//��Ӹ�����Ϣ����  ��չ������Ϣ������7�ֽ�
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x05;//
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x01;//				
					memcpy(temp_upload_msgbody_arry+temp_upload_msgbody_Len,OBD_oil_mass_buffer_arry,2); //���������ݣ�4���ֽ�
					temp_upload_msgbody_Len +=2; //��������ֵ
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//	
        }
      }
    }	//�����������	    
  }
//	if()
//��Ӹ����ֶ�

  Head_mesg.message_ID =0x0201; //λ����Ϣ��ѯӦ��ID
  Head_mesg.message_property = (u16)temp_upload_msgbody_Len; //��ֵ��Ϣ������
  Head_mesg.liu_shui +=1u; //������ˮ��

  temp_upload_Len = Head_deal(Head_mesg,temp_upload_arry);//���ص���Ϣͷ+ 0x7e�ĳ���
  memcpy(temp_upload_arry+temp_upload_Len,(u8 *)temp_upload_msgbody_arry,temp_upload_msgbody_Len); //��ӻ���λ����Ϣ
  temp_upload_Len+=temp_upload_msgbody_Len; //��ʱtemp_upload_Len��û��ת��ǰ������У����ͽ�β��־0x7e



  temp_upload_arry[temp_upload_Len]= add_check_number(temp_upload_arry, (temp_upload_Len+2)); //���У����
  temp_upload_Len = package((u8 *)upload_arry,temp_upload_arry,(u16)(temp_upload_Len+2)); //���
  if(GSM_Send_Data((u8 *)upload_arry,temp_upload_Len+0))
		return TRUE;
	return FALSE;
}

//32λ����ת����4��8λ���ŵ���������
//������u32 in, ��Ҫת����32λ��,
//      u8 *out_p, ����ת����4���ֽڵ�����
//˵��, ���������4�ֽ�32λ��������������ĸ��ֽڵ����飬��32λ���ĵ�λ��������ĸ�Ԫ���У�������������ֱ�ӷָ�ġ�
void u32_to_u8arry(u32 in, u8 *out_p)
{
  u8 i=0;
	if(0x04 != sizeof(in))//�ж�����������ǲ���32λ����4���ֽڡ�
	  return;
	for(i=0;i<4;i++)
	 *(out_p+(3-i)) =(u8)(in>>(i*8)&(0x000000FF));
}

//����״̬�����仯�������ϴ�λ����Ϣ
//�Ƕ���������Ĳ���OBD_Com_Flag.OBD_BodyState_Turn_Flag
void BodyChang_UpLoad(void)
{
  if(OBD_Com_Flag.OBD_BodyState_Turn_Flag)
	{
    send_upload(0);
		OBD_Com_Flag.OBD_BodyState_Turn_Flag = 0 ;
		
  }
}

//�ն�ͨ��Ӧ����
//��������Ǹ��ṹ�壬������Ӧ����ˮ�ţ�Ӧ��ID��Ӧ����
//type: 0:�ɹ�/ȷ��;  1:ʧ��;  2:��Ϣ����;  3:��֧��
u8 Terminal_TongYong_response(REsponse TongYongResp)
{
  u8 temp_upload_respbody_arry[5]={0};//��ʱ�����Ϣ��: λ�û�����Ϣ+������Ϣ���ݶ��䳤����240���ֽ�  
  u8 temp_upload_arry[20] ={0x7e}; //�ϴ����ݵ����ݴ������
  u8 temp_upload_Len =0; //�ϴ����ݵĳ���

//����ͨ��Ӧ����Ϣ�壬��5���ֽ� 0-1:Ӧ����ˮ�ţ� 2-3:Ӧ��ID�� 4:���
  memcpy(temp_upload_respbody_arry,TongYongResp.pliu_shui,2);//���Ӧ����ˮ��
  memcpy(temp_upload_respbody_arry+2,TongYongResp.pmessage_ID,2);//���Ӧ��ID
  temp_upload_respbody_arry[4] = TongYongResp.type ;//��ӽ��

//������Ϣͷ
  Head_mesg.message_ID =0x0001; //�ն�ͨ��Ӧ��ID
  Head_mesg.message_property = (u16)0x05; //��ֵ��Ϣ������
  Head_mesg.liu_shui +=1u; //������ˮ��

  temp_upload_Len = Head_deal(Head_mesg,temp_upload_arry);//���ص���Ϣͷ+ 0x7e�ĳ���
  memcpy(temp_upload_arry+temp_upload_Len,(u8 *)temp_upload_respbody_arry,0x05); //���Ӧ����Ϣ��
  temp_upload_Len+=0x05; //��ʱ0x05��û��ת��ǰ������У����ͽ�β��־0x7e



  temp_upload_arry[temp_upload_Len]= add_check_number(temp_upload_arry, (temp_upload_Len+2)); //���У����
  temp_upload_Len = package((u8 *)upload_arry,temp_upload_arry,(u16)(temp_upload_Len+2)); //���
  if(GSM_Send_Data((u8 *)upload_arry,temp_upload_Len+0))
		return TRUE;
	return FALSE;
}


//�������ú���
//���������
//u8 *p:  ��Ч������ʼ��ַ���������ն˲�����Ϣ�����ʼ�ֽ�--���������ֶζ�Ӧ�ĵ�ַ
//u16 len: ���ȣ�
////PPP��:[7E 81 03 00 3C 01 78 20 00 00 09 00 1A 07 00 00 00 57 04 00 00 00 00 00 00 00 58 04 00 00 00 00 00 00 00 59 04 00 00 00 00 00 00 00 5A 04 00 00 00 00 00 00 00 5B 02 00 00 00 00 00 5C 02 00 00 00 00 00 80 04 00 00 00 00 7C 7E ]-15:06:14

void Parameter_Set(u8 *p,u16 len)
{ 
	u8  Para_Num =0 ; //��������
	u32 Para_ID = 0; //����ID, DWORD����
  u8  Para_len = 0; //�������ȣ�һ������ID��Ӧһ����������
	u8 i =0;
	u8 j =0 ;
	u32 temp_32 = 0; //������ʱ����32λ����
	u16 temp_16 =0;  //������ʱ����16λ����
	u8 temp_arry[5] ={0}; //��������
	Para_Num = *p; //��ȡ�����ܳ���
	++p;
	for(i=0;i<Para_Num;i++)
	{
    Para_ID = (p[0]<<24)|(p[1]<<16)|(p[2]<<8)|p[3];
		Para_len = p[4];
		p +=5;  //ID+1�̶�����Ϊ5����ָ������ID+�������ȶΣ�ָ���������ʼ��ַ
		switch(Para_ID)
		{
			//PPP?:[7E 81 03 00 0A 01 78 20 00 00 09 00 27 01 00 00 00 01 04 00 00 02 24 DD 7E ]-10:13:09
       case 0x00000001:    SPI_Flash_SafeWrite(p,upheartime_addr,Para_len);  //�����������
				 break ;
			 case 0x00000011:      //������������ͨ�Ų����û�������,GPRS�����û���
				 break ;
			 case 0x00000012:      //������������ͨ�Ų������룬��,GPRS��������
				 break ;
			 //PPP?:[7E 81 03 00 15 01 78 10 00 00 00 00 8C 01 00 00 00 13 0F 32 32 30 2E 31 37 38 2E 30 30 30 2E 30 35 36 4C 7E ]-14:31:40
			 //�������͵�ʱ�������:220.178.0.56,  ���������Զ�����:220.178.000.056
			 case 0x00000013:      SPI_Flash_SafeWrite(&Para_len,IPaddress_addr,1);  //IP����
			                       SPI_Flash_SafeWrite(p,IPaddress_addr+1,Para_len);//��������IP��ַ
				 break ;
			 //PPP?:[7E 81 03 00 0A 01 78 10 00 00 00 01 9A 01 00 00 00 19 04 00 00 25 C4 87 7E ]-16:50:14
			 case 0x00000019:    temp_16 = ((p[2]<<8)|p[3]);  //��Ȼ����������ĸ��ֽڵģ����Ƕ˿ں�ֻ����͵������ֽ�
			  //������UDP�˿�	   //��һ�֣���3��λ�� �õ�16λ2������ת����BCD��
			                       temp_32 =0;  j =0;			                       
			                       for(j=0;j<16;j++)
			                       {
                               temp_32 = temp_32 << 1;
															 temp_32 |=(u32)((temp_16>>(15-j))&0x0001); //��Ҫ����16��
															 if(j >=15)
																 break ;
															// if(j>=2)
															 //{
																 if(((temp_32&0x0000000f)+3) > (u32)0x07)
																	  temp_32 += (u32)0x03;
                               //}
															 //if(j>=5)
															 //{
                                 if(((temp_32&0x000000f0)+((u32)0x30)) > (u32)0x70)
																	  temp_32 += (u32)0x30;
                              // }
															// if(j>=8)
															// {
                                 if(((temp_32&0x00000f00)+(u32)0x0300) > (u32)0x0700)
																	  temp_32 += (u32)0x0300;
                              // }
															// if(j>= 11)
															// {
                                 if(((temp_32&0x0000f000)+(u32)0x3000) > (u32)0x7000)
																	temp_32 += 0x3000; 
                               //}
                             }
														 temp_arry[0] = ((temp_32>>16)&((u32)0x0f))+0x30;//0x00000f0000) + 0x30;
														 temp_arry[1] = ((temp_32>>12)&((u32)0x0f))+0x30;
														 temp_arry[2] = ((temp_32>>8)&((u32)0x0f))+0x30;
														 temp_arry[3] = ((temp_32>>4)&((u32)0x0f))+0x30;
														 temp_arry[4] = (temp_32&((u32)0x0f))+0x30; 
                             if(temp_arry[0]>0x30) 	
                             {
                               j = 0x05;
															 SPI_Flash_SafeWrite(&j,PortNumber_addr,1);  //PORT����
			                         SPI_Flash_SafeWrite(temp_arry,PortNumber_addr+2,j);//PORT
                             }
                             else if(temp_arry[1]>0x30) 
                             {
                               j = 0x04;
															 SPI_Flash_SafeWrite(&j,PortNumber_addr,1);  //PORT����
			                         SPI_Flash_SafeWrite(temp_arry+1,PortNumber_addr+2,j);//PORT
                             }
														 else if(temp_arry[2]>0x30) 
                             {
                               j = 0x03;
															 SPI_Flash_SafeWrite(&j,PortNumber_addr,1);  //PORT����
			                         SPI_Flash_SafeWrite(temp_arry+2,PortNumber_addr+2,j);//PORT
                             }														 
														 else if(temp_arry[3]>0x30) 
                             {
                               j = 0x02;
															 SPI_Flash_SafeWrite(&j,PortNumber_addr,1);  //PORT����
			                         SPI_Flash_SafeWrite(temp_arry+3,PortNumber_addr+2,j);//PORT
                             }
														 else if(temp_arry[4]>0x30) 
                             {
                               j = 0x01;
															 SPI_Flash_SafeWrite(&j,PortNumber_addr,1);  //PORT����
			                         SPI_Flash_SafeWrite(temp_arry+4,PortNumber_addr+2,j);//PORT
                             }
									/* �ڶ��֣�ֱ�ӳ����õ�16λ2������ת����BCD��
            								 temp_arry[0] = temp_16/10000;
														 temp_arry[1] = (temp_16/1000) - ((temp_16/10000)*10);
														 temp_arry[2] = (temp_16/100) - ((temp_16/1000)*10);
														 temp_arry[3] = (temp_16/10) - ((temp_16/100)*10);
														 temp_arry[4] = (temp_16/1) - ((temp_16/10)*10);  */
				                                //������UDP�˿�				 
				 break ;
														 
			 case 0x00000027:      //����ʱ�㱨ʱ��������λΪ��(S), >0
				                     temp_32 = (p[0]<<24)|(p[1]<<16)|(p[2]<<8)|p[3] ;
			                       if((temp_32 > 0x0000FFFF)||(temp_32 < (u32)0x05))
															 break ;
														 SPI_Flash_SafeWrite(p+2,ACCOFF_GPSuploadtime_add,0x02);
				 break ;
			 case 0x00000028:      temp_32 = (p[0]<<24)|(p[1]<<16)|(p[2]<<8)|p[3] ;
			                       if((temp_32 > 250)||(temp_32 < 2))
															 break ;
														 SPI_Flash_SafeWrite(p+2,JinJi_addr,0x02);//��������ʱ�㱨ʱ��������λΪ��(S), >0
				 break ;
														 //PPP?:[7E 81 03 00 08 01 78 10 00 00 00 03 A9 01 00 00 00 31 02 00 64 1F 7E ]-16:27:22
			 case 0x00000031:      //����Χ���뾶(�Ƿ�λ����ֵ��Ʃ�磬��Υͣ���ϳ���)����λΪ��
				                     temp_16 = (p[0]<<8)|p[1] ;
				                     if((temp_16 ==0xFFFF)||(temp_16 < 0x0a))  //����Χ����ȡֵ��Χ
														   break ;
														 SPI_Flash_SafeWrite(p,R_rail_addr,0x02);														 
				 break ;
														 
			 case 0x00000040:      //(������ʱ����)���ü��ƽ̨�绰����
				 break ;
       case 0x00000041:      //(��:��ʱ����)��λ�绰���룬�ɲ��ô˵绰���벦���ն˵绰���ն˸�λ
				 break ;
			 case 0x00000042:      //�ָ��������õ绰���룬�ɲ��ô˵绰���벦���ն˵绰�����ն˻ظ���������
				 break ;
			 case 0x00000043:      //���ƽ̨SMS�绰����
				 break ;
			 case 0x00000044:      //�����ն�SMS�ı��������룬���о���ʱ���ն���������뷢�ͱ�������
				 break ;
			 case 0x00000048:      //�����绰���룬
				 break ;
			 case 0x00000055:      //����ٶȣ���λΪ����ÿСʱ(km/h),���ǳ���ֵ
				                     temp_32 = (p[0]<<24)|(p[1]<<16)|(p[2]<<8)|p[3] ;
				                     if((temp_32 ==0xFFFF)||(temp_32 < 0x0a))   //����ֵ��ȡֵ��Χ
															 break ;
														 SPI_Flash_SafeWrite(p+2,Over_Speed_addr,0x02);
				 break ;
       case 0x00000056:      //���ٳ���ʱ�䣬��λΪ��(s),һ����Ϊ10s
				                     temp_32 = (p[0]<<24)|(p[1]<<16)|(p[2]<<8)|p[3] ;
				                     if((temp_32 ==0xFFFF)||(temp_32 < 0x0a))  //����ֵ��ȡֵ��Χ
														   break ;															 
														 SPI_Flash_SafeWrite(p+2,Over_Speed_XDTime_addr,0x02);
				 break ;
			 case 0x00000081:      //��������ʡ��ID
				                     temp_16 = (p[0]<<8)|p[1] ;
				                     if(temp_16 ==0xFFFF)  //||(temp_32 < 0x0a))  //����ֵ��ȡֵ��Χ
														   break ;															 
														 SPI_Flash_SafeWrite(p,Province_ID_addr,0x02);
				 break ;
       case 0x00000082:      //������������ID
				                     temp_16 = (p[0]<<8)|p[1] ;
				                     if(temp_16 ==0xFFFF)  //||(temp_32 < 0x0a))  //����ֵ��ȡֵ��Χ
														   break ;															 
														 SPI_Flash_SafeWrite(p,City_ID_addr,0x02);
				 break ; 
			 case 0x00000083:      //������ͨ�����Ű䷢�Ļ��������ƣ���������ǲ�����
				                      if((Para_len>0)&&(Para_len < 14))
															{
                                SPI_Flash_SafeWrite(&Para_len,Vehicle_license_Plate_Len_addr,0x01);
																SPI_Flash_SafeWrite(p,Vehicle_license_Plate_addr,Para_len);
                              }			 
				 break ;
															
			 case 0x00000084:      //������ɫ������JT/T415-2006��5.4.12
				                      if(p[0] != 0xff)  SPI_Flash_SafeWrite(p,Vehicle_license_Plate_colour_addr,0x01);                             			  
				 break ;
       
			 case 0x00000090:      //GNSS��λģʽ bit0,0:����GPS��λ, 1:����GPS��λ;   bit1, 0:���ñ�����λ, 1:���ñ�����λ
				                     if(p[0] <= 0x03)  SPI_Flash_SafeWrite(p,GNSS_MODE_addr,0x01);
				 break ;
			 
			 case 0x00000091:      //GNSS�����ʣ���������:0x00-4800; 0x01-9600; 0x02-19200; 0x03-38400; 0x04-57600; 0x05-115200
				                     if(p[0] <= 0x05)  SPI_Flash_SafeWrite(p,GNSS_Baud_addr,0x01);
				 break ;
			 
      
			 case 0x00000092:      if(p[0] <= 0x04)  SPI_Flash_SafeWrite(p,GNSS_Hz_addr,0x01);  ////GNSSģ����ϸ��λ�������Ƶ�� 
				 break ;

    }
		
	  p +=Para_len; //��ָ��ָ����һ��������ID
		
  }
	

}




























