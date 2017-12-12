
#include "alarm.h"
u16 JinJi_liushui =0;  //保存上发紧急报警的消息流水号
u8  JinJi_Num = 0;     //紧急报警发送次数，

u16 Shake_continu_timer = 0;  //连续按下紧急报警时，需要间隔一段时间在报
u8 Shake_Num = 0;      //震动电平下降沿次数，计次
u16 Shake_LianXuCount = 0; //防止震动报警太频繁
u16 Shake_JianGE =0;  //限制震动报警的最短时间
u8 Shake_sendNum =0;  //震动报警
u16 Shake_liushui =0; //记录上发震动报警的消息流水号





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
//紧急报警
  if(Flag_all.JinJi_Flag) //如果有紧急报警
	{
		alarm_state =alarm_state|((u32)0x01);  //将第0位置1
		if(!JinJi_Num)//&&(JinJi_LianXucount >= JinJi_JianGe))
		{
			JinJI_count = 0; 
		  send_upload(1); //发送报警
		               // JinJi_liushui = Head_mesg.liu_shui ; //保存上方紧急报警的消息流水号，以便和应答流水号做对比，判断是不是紧急报警应答
		                 //OBD_Com_Flag.Have_alarm = 1 ; //有立即报警标志		
    }
		else if((JinJi_Num)&&(JinJI_count >= JinJI_timeout))
		{
      send_upload(1);  //发送报警
			JinJI_count = 0; 
    }
		if(JinJi_Num++>20) JinJi_Num=20;		
		JinJi_liushui = Head_mesg.liu_shui ; //保存上方紧急报警的消息流水号，以便和应答流水号做对比，判断是不是紧急报警应答
			
  }

//OBD检测到的报警
  if(OBD_Com_Flag.alarm == LOCK)  //如果机器状态是警戒状态
  {
    if((OBD_Com_Flag.Have_alarm)||((JinJI_count >= JinJI_timeout)&&((OBD_Com_Flag.door)||(OBD_Com_Flag.ACC_alarm)||(OBD_Com_Flag.brake)))) //有警情出现
		{      
		  send_upload(1);//发送位置信息，上报警情。type类型设为1(1=上报警情)
			OBD_Com_Flag.Have_alarm = 0; //上报一次警情后，这里就将警情清零，这个标志是一次性的。
			JinJI_count =0; //重置紧急报警计数器
    }
//震动报警
		if(Flag_all.Shake_Flag)
		{
			alarm_state =alarm_state|(((u32)0x01) <<16);  //将第0位置1
			if(!Shake_sendNum)//&&(JinJi_LianXucount >= JinJi_JianGe))
			{
				Shake_LianXuCount = 0; 
				send_upload(1); //发送报警
										 // JinJi_liushui = Head_mesg.liu_shui ; //保存上方紧急报警的消息流水号，以便和应答流水号做对比，判断是不是紧急报警应答
											 //OBD_Com_Flag.Have_alarm = 1 ; //有立即报警标志		
			}
			else if((Shake_sendNum)&&(Shake_LianXuCount >= Shake_JianGE))
			{
				send_upload(1);  //发送报警
				Shake_LianXuCount = 0; 
			}
			if(Shake_sendNum++>20)  Shake_sendNum=20;		
			Shake_liushui = Head_mesg.liu_shui ; //保存上方紧急报警的消息流水号，以便和应答流水号做对比，判断是不是紧急报警应答
				
		}	
  }
	
}


























