
#include "alarm.h"
u16 JinJi_liushui =0;  //�����Ϸ�������������Ϣ��ˮ��
u8  JinJi_Num = 0;     //�����������ʹ�����

u16 Shake_continu_timer = 0;  //�������½�������ʱ����Ҫ���һ��ʱ���ڱ�
u8 Shake_Num = 0;      //�𶯵�ƽ�½��ش������ƴ�
u16 Shake_LianXuCount = 0; //��ֹ�𶯱���̫Ƶ��
u16 Shake_JianGE =0;  //�����𶯱��������ʱ��
u8 Shake_sendNum =0;  //�𶯱���
u16 Shake_liushui =0; //��¼�Ϸ��𶯱�������Ϣ��ˮ��





void alarm(void)
{
/*	
	u8 temp[4] ={0};
  u8 *p1=0;
  u8 *p2=0;
  u8 *p3=0;
  if(timer2_100ms==500)
	{
		timer2_100ms = 0;
		if(OBD_Com_Flag.door||OBD_Com_Flag.ACC_alarm||OBD_Com_Flag.brake)
		{
	
			
			temp[3]= alarm_state&0x000000FF;
			temp[2]= (alarm_state>>8)&0x000000FF;
			temp[1]= (alarm_state>>16)&0x000000FF;
			temp[0]= (alarm_state>>24)&0x000000FF;
			p1 =&usart1_Rx_backup[3];
			p2 =&usart1_Rx_backup[2];
			p3 =&usart1_Rx_backup[1];
			Usart2_SendData(temp,4);
			Usart2_SendData((u8 *)"\r\n",2);
			
	 
		}
		temp[3]= Body_state&0x000000FF;
		temp[2]= (Body_state>>8)&0x000000FF;
		temp[1]= (Body_state>>16)&0x000000FF;
		temp[0]= (Body_state>>24)&0x000000FF;
		Usart2_SendData(temp,4);
		Usart2_SendData("\r\n",2);
  }
*/
//��������
  if(Flag_all.JinJi_Flag) //����н�������
	{
		alarm_state =alarm_state|((u32)0x01);  //����0λ��1
		if(!JinJi_Num)//&&(JinJi_LianXucount >= JinJi_JianGe))
		{
			JinJI_count = 0; 
		  send_upload(1); //���ͱ���
		               // JinJi_liushui = Head_mesg.liu_shui ; //�����Ϸ�������������Ϣ��ˮ�ţ��Ա��Ӧ����ˮ�����Աȣ��ж��ǲ��ǽ�������Ӧ��
		                 //OBD_Com_Flag.Have_alarm = 1 ; //������������־		
    }
		else if((JinJi_Num)&&(JinJI_count >= JinJI_timeout))
		{
      send_upload(1);  //���ͱ���
			JinJI_count = 0; 
    }
		if(JinJi_Num++>20) JinJi_Num=20;		
		JinJi_liushui = Head_mesg.liu_shui ; //�����Ϸ�������������Ϣ��ˮ�ţ��Ա��Ӧ����ˮ�����Աȣ��ж��ǲ��ǽ�������Ӧ��
			
  }

//OBD��⵽�ı���
  if(OBD_Com_Flag.alarm == LOCK)  //�������״̬�Ǿ���״̬
  {
    if((OBD_Com_Flag.Have_alarm)||((JinJI_count >= JinJI_timeout)&&((OBD_Com_Flag.door)||(OBD_Com_Flag.ACC_alarm)||(OBD_Com_Flag.brake)))) //�о������
		{      
		  send_upload(1);//����λ����Ϣ���ϱ����顣type������Ϊ1(1=�ϱ�����)
			OBD_Com_Flag.Have_alarm = 0; //�ϱ�һ�ξ��������ͽ��������㣬�����־��һ���Եġ�
			JinJI_count =0; //���ý�������������
    }
//�𶯱���
		if(Flag_all.Shake_Flag)
		{
			alarm_state =alarm_state|(((u32)0x01) <<16);  //����0λ��1
			if(!Shake_sendNum)//&&(JinJi_LianXucount >= JinJi_JianGe))
			{
				Shake_LianXuCount = 0; 
				send_upload(1); //���ͱ���
										 // JinJi_liushui = Head_mesg.liu_shui ; //�����Ϸ�������������Ϣ��ˮ�ţ��Ա��Ӧ����ˮ�����Աȣ��ж��ǲ��ǽ�������Ӧ��
											 //OBD_Com_Flag.Have_alarm = 1 ; //������������־		
			}
			else if((Shake_sendNum)&&(Shake_LianXuCount >= Shake_JianGE))
			{
				send_upload(1);  //���ͱ���
				Shake_LianXuCount = 0; 
			}
			if(Shake_sendNum++>20)  Shake_sendNum=20;		
			Shake_liushui = Head_mesg.liu_shui ; //�����Ϸ�������������Ϣ��ˮ�ţ��Ա��Ӧ����ˮ�����Աȣ��ж��ǲ��ǽ�������Ӧ��
				
		}	
  }
	
}


























