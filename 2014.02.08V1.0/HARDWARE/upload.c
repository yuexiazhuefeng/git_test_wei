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

Head Head_mesg; //消息头初始化结构体
REsponse TongYongResp;//通用应答函数，需要的参数用结构体来组织

volatile u8 ben_ji_hao[6]={0};//{0x01,0x83,0x56,0x95,0x78,0x28}; //数据类型BCD[6]
volatile u8 upload_arry[512] ={0}; //上传消息打包。
volatile u8 xiaoxi_head[13]={0}; //消息头处理数组
volatile u8 JianQuanMa[20] = {0};  //存放鉴权码.鉴权码的长度是动态的，这里设其最大长度是20字节，我们一般用6个字节。
volatile u8 JianQuanMa_Len = 0;   //收到的鉴权码的长度
volatile u16 liu_shui =0; //流水号
volatile u16 message_ID =0; //消息ID
volatile u16 message_property =0; //消息体属性
u16 authentication_count =0; //终端重复注册时间周期 ，先设为10s

volatile u32 alarm_flag = 0; //位置信息中的，报警标志
volatile u32 state_flag = 0; //位置信息中的，状态位

u16 R_rail = 0; //围栏半径，非法移位半径
u16 Over_Speed = 0;//超速值
u16 Over_Speed_XDTime = 0; //超速消抖时间，即超速维持时间超过此值时，会有超速报警
u8 Province_ID[2] = {0}; //车辆所在省域ID
u8 City_ID[2] = {0}; //车辆所在省域ID


u8 Vehicle_license_Plate_colour = 0 ;  //车牌颜色，按照JT/T415-2006的5.4.12
u8 Vehicle_license_Plate_Len  = 0; //车牌照长度
u8 Vehicle_license_Plate[14] ={0}; //车牌照，最长支持14位 

u8 GNSS_MODE=0; //bit0,0:禁用GPS定位, 1:启用GPS定位;   bit1, 0:禁用北斗定位, 1:启用北斗定位
u8 GNSS_Baud = 0xFF; //这里给他初始化为0xFF,因为，0x00有具体的含义; 0x00:4800; 0x01:9600; 0x02:19200; 0x03:38400; 0x04:57600; 0x05:115200
u8 GNSS_Hz = 0xFF;  //GNSS模块详细定位数据输出频率, 0x00:500ms; 0x01:1000ms(默认值); 0x02:2000ms; 0x03:3000ms; 0x04:4000ms
//u8 upload_arry[512] ={0};

//终端注册
//7E 01 00 00 36 01 31 00 00 00 20 00 01 00 22 00 64 37 30 34 34 34 46 48 54 2D 47 50 53 2F 50 4B 45 39 30 30 44 00 00 00 00 00 44 31 31 39 38 30 39 00 44 45 46 41 55 4C 54 2D 56 49 4E 39 39 39 39 39 39 17 7E
//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 
                   //0   1   2    3     4   5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23
//u8 test_arry[69]={0x7E,0x01,0x00,0x00,0x36,0x01,0x31,0x00,0x00,0x00,0x20,0x00,0x01,0x00,0x22,0x00,0x64,0x37,0x30,0x34,0x34,0x34,\
//                  0x46,0x48,0x54,0x2D,0x47,0x50,0x53,0x2F,0x50,0x4B,0x45,0x39,0x30,0x30,0x44,0x00,0x00,0x00,0x00,0x00,0x44,0x31,\
 //                 0x31,0x39,0x38,0x30,0x39,0x00,0x44,0x45,0x46,0x41,0x55,0x4C,0x54,0x2D,0x56,0x49,0x4E,0x39,0x39,0x39,0x39,0x39,0x39,0x17,0x7E};
void Terminal_authentication(void) //终端注册函数
{
	u8 temp_authentication_result =0;
	u8 temp_0x55 =0x55;
 /* u16 temp_len =0;  	
	//                                                                                                           0x30      0x34 
	                  //0     1   2    3     4   5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23
  u8 temp_zhuce[69]={0x7E,0x01,0x00,0x00,0x36,0x01,0x02,0x03,0x04,0x05,0x06,0x00,0x00,0x00,0x22,0x00,0x64,0x37,0x7e,0x34,0x7d,0x34,0x46,0x48,\
                     0x54,0x2D,0x47,0x50,0x53,0x2F,0x50,0x4B,0x45,0x39,0x30,0x30,0x44,0x00,0x00,0x00,0x00,0x00,0x44,0x31,0x31,0x39,0x38,0x30,\
		                 0x39,0x00,0x44,0x45,0x46,0x41,0x55,0x4C,0x54,0x2D,0x56,0x49,0x4E,0x39,0x39,0x39,0x39,0x39,0x39}; //用来注册的数组
  change_ben_ji_hao(temp_zhuce+5); //加本机号到消息头中。
	add_liu_shui_hao(temp_zhuce+11); //添加流水号			 
	temp_zhuce[68] = 0x7E; //添加结尾标识符
	temp_zhuce[67] = add_check_number(temp_zhuce,69);//添加校验码												 
  temp_len = package(upload_arry,temp_zhuce,69);										 
  GSM_Send_Data(upload_arry,temp_len); //发送数据到服务器				 */
	while(1)
	{
    if(Send_authentication()) //判断注册信息是否发送成功，GSM返回OK
	  {
			authentication_count = 0;
			while(authentication_count<1000)
			{
				IWDG_Feed();
        if((read_GSM_time>=250)||(Flag_all.haveGSMdata==SISR_1)) //定时到，或收到GSM主动上报
				{
          read_GSM_time =0;
					Flag_all.haveGSMdata = 0;
					if(Read_GSM()) //收到7E数据，要处理
					{
            temp_authentication_result=GSM_Recivdata_deal(); //处理
						temp_authentication_result -= 5;
						if((0==temp_authentication_result)||(3==temp_authentication_result))  //注册成功。目前服务器只返回0，没有做其他项，
						{
							SPI_Flash_SafeWrite((u8 *)(&(JianQuanMa_Len)),JianQuanMa_addr,0x01); //存储鉴权码长度
							SPI_Flash_SafeWrite((u8 *)JianQuanMa,JianQuanMa_addr+1,JianQuanMa_Len);//存储鉴权码
							SPI_Flash_SafeWrite((u8 *)(&(temp_0x55)),Terminal_auth_Flag_addr,0x01);//存储注册成功标志
							Flag_all.Terminal_auth_Flag = TRUE;					//成功标志置1；并存储
							return ;  //成功
						}
          }
        }
      }
    }
  }		
}

u8 Send_authentication(void) //发送注册信息
{
  u16 temp_len =0;  	
	//                                                                                                           0x30      0x34 
	                  //0     1   2    3     4   5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23
  u8 temp_zhuce[69]={0x7E,0x01,0x00,0x00,0x36,0x01,0x02,0x03,0x04,0x05,0x06,0x00,0x00,0x00,0x22,0x00,0x64,0x37,0x7e,0x34,0x7d,0x34,0x46,0x48,\
                     0x54,0x2D,0x47,0x50,0x53,0x2F,0x50,0x4B,0x45,0x39,0x30,0x30,0x44,0x00,0x00,0x00,0x00,0x00,0x44,0x31,0x31,0x39,0x38,0x30,\
		                 0x39,0x00,0x44,0x45,0x46,0x41,0x55,0x4C,0x54,0x2D,0x56,0x49,0x4E,0x39,0x39,0x39,0x39,0x39,0x39}; //用来注册的数组
  change_ben_ji_hao(temp_zhuce+5); //加本机号到消息头中。
	add_liu_shui_hao(temp_zhuce+11); //添加流水号			 
	temp_zhuce[68] = 0x7E; //添加结尾标识符
	temp_zhuce[67] = add_check_number(temp_zhuce,69);//添加校验码												 
  temp_len = package((u8 *)upload_arry,temp_zhuce,69);	
  upload_arry[temp_len]	= 0x1A;		 //MG323结尾要加ctrl+z 其ASCII码为26即0x1a						 
  if(GSM_Send_Data((u8 *)upload_arry,temp_len+1))//发送数据到服务器		
     return 1;
  return 0;

}

//本机号添加程序，在要打包上传的数组中添加本机号。
//需要输入参数，in_p:本机号在要上传的数组中的位置。
void change_ben_ji_hao(u8 *in_p)  //把本机号加到待上传的包里
{
  u8 i =0;
	u8 *tem_p =in_p;
	for(i=0;i<6;i++)
	{
     *(tem_p+i)= ben_ji_hao[i];
  }
}

//添加消息流水号
//参数：输入参数为，流水号的存放指针
void add_liu_shui_hao(u8 *in_p)
{
  u8 *temp_p=in_p;
	Head_mesg.liu_shui++;
	*(temp_p)=(u8)(liu_shui>>8);
	*(temp_p+1) = (u8)(liu_shui);
}

//打包上传,负责转义以7E开头和7E结尾的数组
//参数：
//u8 *out_p处理后的包数据存放地址; u8 *in_p:待打包起始指针; u16 len:待打包长度，这个长度是不包括结尾的7e的，只到校验码
//返回：打包后的数据长度，这个长度的数据是可以直接上传到服务器的。
u16 package(u8 *out_p,u8 *in_p, u16 len)
{
  u8 *temp_in_p=in_p;
	u8 *temp_out_p=out_p;
	//upload_arry = in_p;
	u16 temp_len =len;
	u16 i=0;
	u16 j=1;
	temp_out_p[0] = *temp_in_p; //这里把指针直接像数组那样用。把开始的7E标示符直接放到输出位置
	for(i=1;i<len-1;i++,j++) //去头掐尾，去的头是起始处的7E标示符，掐的尾是，校验码
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
	temp_out_p[j++] =0x7e;  //添加结尾的7E标示符
	return j;
}

//添加包校验码。
//参数：
//u8 *in_p:起始指针即7E的指针; u16 len :待打包数据的总度，包括7E头和7E尾。这个长度，是没有进行转义前的长度。
//返回：校验码
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

//对收到的部标数据解析，
//0x7d 0x02-->0x7e
//0x7d 0x01-->0x7d
//参数，*p_in:解析前的数据位置的起始地址，  *p_out:解析后数据的存放位置的起始地址， len_in:解析前数据的长度
//返回值，返回转义后的数据长度。
u16 Bubiao_Reci_7E_Analysis(u8 *p_in,u8 *p_out,u16 len_in)
{
  u16 i=0;
	u16 j=0;
	if((*p_in==0x7e)&&(*(p_in+len_in-1)==0x7e)) //判断头尾都是0x7e,才会进入计算
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

void Terminal_JianQuan(void) //终端鉴权函数
{
	u8 temp_authentication_result =0;
	u8 temp_0x55 =0x55;  
	while(1)
	{
    if(Send_JianQuan()) //判断注册信息是否发送成功，GSM返回OK
	  {
			read_GSM_time =0;
			authentication_count = 0;
			while(authentication_count<1000)
			{
				IWDG_Feed();
        if((read_GSM_time>=250)||(Flag_all.haveGSMdata==SISR_1)) //定时到，或收到GSM主动上报
				{
          read_GSM_time =0;
					Flag_all.haveGSMdata = 0;
					if(Read_GSM()) //判断是否收到7E数据，要处理
					{
            temp_authentication_result=GSM_Recivdata_deal(); //调用处理函数，对收到的部标数据处理
						//temp_authentication_result -= 5;
						if(TRUE==temp_authentication_result)  //鉴权成功
						{
							Usart2_SendData("JianQuan success...\r\n",sizeof("JianQuan success...\r\n"));
							return ;  //成功
						}
          }
        }
      }
    }
  }		
}


//PPP包:[7E 01 02 00 06 01 32 05 14 48 12 00 02 42 59 46 55 55 53 71 7E ]-11:47:50  //终端鉴权，查考数据
//       0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 
u8 Send_JianQuan(void) //终端鉴权程序
{
  u8 temp_jianquan[40] ={0x7e}; //临时存储处理过程中的数据，完成后传递给打包函数，打包到 upload_arry[512]数组。
  u8 temp_len = 0;//临时数组中数据的长度
/*  message_ID = 0x0102; //终端鉴权消息ID：0x0102
  temp_jianquan[1]=0x01;
  temp_jianquan[2]=0x02;//添加消息ID
  message_property = JianQuanMa_Len; //消息体属性赋值，这里消息体的长度就是鉴权码的长度
  temp_jianquan[3]=(u8)(message_property>>8);
  temp_jianquan[4]=(u8)(message_property&0x00FF);
*/  
  Head_mesg.message_ID =0x0102; //赋值消息ID
  Head_mesg.message_property = (u16)JianQuanMa_Len; //赋值消息体属性
  Head_mesg.liu_shui +=1u;
  temp_len = Head_deal(Head_mesg,temp_jianquan);//返回的消息头+ 0x7e的长度
  memcpy(temp_jianquan+temp_len,(u8 *)JianQuanMa,JianQuanMa_Len); //添加鉴权码
  temp_len+=JianQuanMa_Len;
  temp_len++;
  //temp_jianquan[temp_len++]=0x7e;
  //temp_len++;
  temp_jianquan[temp_len-1]= add_check_number(temp_jianquan, (temp_len+1)); //添加校验码
  temp_len=package((u8 *)upload_arry,temp_jianquan,(u16)(temp_len+1));
  if(GSM_Send_Data((u8 *)upload_arry,temp_len))
		return TRUE;
	return FALSE;
}

void Terminal_JianQuan_authentication(void)  //对终端进行注册和鉴权
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

//消息头处理函数
//输入参数，Head类型的结构体指针
//输出参数，处理后数组中有效数据的长度
//这个函数，是处理完成后将消息头的数据存放到xiaoxi_head[13]数组中的，为了让该函数靠近可重入函数，处理过程中是先使用临时数组存放
//消息头数据的，在消息头数据处理完成后再一次性的转达全局变量的数组xiaoxi_head[13]中。其被多个函数同时调用时带来的风险
//   |ID | 属性| |终端手机号     | 流水号
//7E 01 00 00 36 01 83 56 95 78 28 00 01
//0  1  2  3  4  5  6  7  8  9  10 11 12
u8 Head_deal(Head in_head_mesg,u8 *p_in) //消息头处理函数
{
  u8 temp_head_arry[13]={0x7e};//定义的时候就把7E头放进去，用来临时存放消息头
  Head temp_Head_mesg = in_head_mesg; //将传递过来的结构体数据赋值给这个临时结构体，这样如果外部修改了全局结构体变量head_mesg，也不会影响到这里。
  u8 temp_len =0;    
  

  temp_head_arry[1]=(u8)(in_head_mesg.message_ID>>8);//消息ID
  temp_head_arry[2]=(u8)(in_head_mesg.message_ID&0x00ff);//消息ID

  temp_head_arry[3]=(u8)(in_head_mesg.message_property>>8);//消息体属性
  temp_head_arry[4]=(u8)(in_head_mesg.message_property&0x00ff);//消息体属性

  memcpy(temp_head_arry+5,(u8 *)ben_ji_hao,0x06);//终端手机号，它在程序运行期间都是不会变的，所以其在参数初始化的时候就已经设置好了

  temp_head_arry[11]=(u8)(in_head_mesg.liu_shui>>8);//流水号
  temp_head_arry[12]=(u8)(in_head_mesg.liu_shui&0x00ff);//流水号
  if(in_head_mesg.message_property&(0x0001<<13)) //如果第13位分包标志位1，此时消息头的字节长度是14个字节
		temp_len = 14;
	else temp_len = 13;//没有消息包分装项，此时字节数长度是13字节
  memcpy(p_in,temp_head_arry,temp_len); //把处理好的消息头放到调用函数时传递进来的指针处
	return temp_len;
}

u8 Upload_Heart(void) ////上传心跳程序
{
  if((Flag_all.SendHeart_Flag)&&(upheart_timeout_count >= upheart_timeout)) //心跳应答超时
	{
    Usart2_SendData("Heart ERROR Reset MCU...\r\n",sizeof("Heart ERROR Reset MCU...\r\n"));
    SystemReset(); //请求一次软件复位
  }
	if(upheartime_count >= upheartime ) //上传心跳计时到
	{
    upheartime_count =0;
		if(TRUE==Send_Heart())
		{
      Flag_all.SendHeart_Flag =1;  //心跳发出，这里
      return TRUE;
    }			
		else
			return FALSE;
  }
}


//PPP包:[7E 00 02 00 00 01 32 05 14 48 12 00 04 22 7E ]-11:48:32
u8 Send_Heart(void)  //上传心跳数据
{
  u8 temp_Heart[40] ={0x7e}; //临时存储处理过程中的数据，完成后传递给打包函数，打包到 upload_arry[512]数组。
  u8 temp_len = 0;//临时数组中数据的长度
  Head_mesg.message_ID =0x0002; //赋值消息ID
  Head_mesg.message_property = 0x0000; //赋值消息体属性
  Head_mesg.liu_shui +=1u;
  temp_len = Head_deal(Head_mesg,temp_Heart);//返回的消息头+ 0x7e的长度
  //memcpy(temp_jianquan+temp_len,(u8 *)JianQuanMa,JianQuanMa_Len); //添加鉴权码
  //temp_len+=JianQuanMa_Len;
  temp_len++;
 // temp_Heart[temp_len++]=0x7e;
  //temp_len++;
  temp_Heart[temp_len-1]= add_check_number(temp_Heart, temp_len+1); //添加校验码
  temp_len=package((u8 *)upload_arry,temp_Heart,(u16)(temp_len+1));
  if(GSM_Send_Data((u8 *)upload_arry,temp_len))
	{
    memset((u8 *)upload_arry,0x00,512); //清空上传数组
		return TRUE;
  }		
	return FALSE;
}

u8 Upload_Location(void) ////上传位置信息，不包含报警信息
{
//  if((Flag_all.SendLocation_Flag)&&(LocationUptime_count >= upheart_timeout)) //心跳应答超时
//	{
//    Usart2_SendData("Heart ERROR Reset MCU...\r\n",sizeof("Heart ERROR Reset MCU...\r\n"));
//    SystemReset(); //请求一次软件复位
//  }
	u16 temp_alarm_flag = alarm_flag;//保存报警位，上传位置信息时，不要上传报警位，下一步会把报警位清零
	alarm_flag =0 ; //上传位置信息时，不要上传报警位，这里把报警位清零
	//if(LocationUptime_count >= LocationUptime) //上传位置时间间隔到了，
	if(Flag_all.Upload_flag) //上传位置信息间隔时间到，需要上传位置信息
	{
    //LocationUptime_count =0;
		Flag_all.Upload_flag =0;
		if(TRUE==send_upload(0)) //发送位置信息
		{
      Flag_all.SendLocation_Flag =1;  //心跳发出，这里
		  alarm_flag = temp_alarm_flag;//把报警标志还原
      return TRUE;
    }			
		else
			return FALSE;
  }
}

//发送位置信息的发送函数
//函数返回值：TRUE:发送成功，这个成功指的是收到GSM模块返回的OK
//输入参数 type
//默认都为0
//1-报警位置信息，要处理报警位
//
u8 send_upload(u8 type)
{
//位置基本信息的长度是28字节	
  u8 temp_upload_msgbody_arry[240]={0};//临时存放消息体: 位置基本信息+附加信息，暂定其长度是240个字节
  u8 temp_upload_msgbody_Len=0; //消息体长度
  u8 temp_upload_arry[255] ={0x7e}; //上传数据的数据存放数组
  u8 temp_upload_Len =0; //上传数据的长度

  if(type ==1)
		u32_to_u8arry(alarm_state,temp_upload_msgbody_arry);//处理报警标志
	else if(type ==0)
    u32_to_u8arry(alarm_flag,temp_upload_msgbody_arry);//处理报警标志
  temp_upload_msgbody_Len =4; 

///////////////////读取状态位
	if(type ==1)
	{
   // OBD_Com_Flag.OBD_ReadBodyState_Flag = 1; //将主动读取车身状态位置1
	 // OBD_Read_Body_state();  //调用读取车身状态信息函数  
	 // OBD_Com_Flag.OBD_ReadBodyState_Flag = 0; //将主动读取车身状态位清除
		u32_to_u8arry(Body_state,temp_upload_msgbody_arry+4);//处理状态 
  }
	else if(type ==0)
	{ 
		OBD_Com_Flag.OBD_ReadBodyState_Flag = 1; //将主动读取车身状态位置1
		OBD_Read_Body_state();  //调用读取车身状态信息函数  
		OBD_Com_Flag.OBD_ReadBodyState_Flag = 0; //将主动读取车身状态位清除
		u32_to_u8arry(Body_state,temp_upload_msgbody_arry+4);//处理状态 
  }	
	temp_upload_msgbody_Len+=4;
	memcpy(temp_upload_msgbody_arry+8,GPS_DATA_BACKUP,20); //将处理完成的GPS数据放到临时存放消息体的数组里面
  temp_upload_msgbody_Len +=20;
////////////////////////////////////////////////////

//添加附加字段
//  if(OBD_Com_Flag.ACC_state) //添加里程，前判断ACC为开，才会进行下面动作
//	{	 //这个可以试着改为判断OBD是否睡眠，来操作下面的内容	
    mileage();//读取里程函数
		if(OBD_Com_Flag.OBD_Mileage_Receivded_FLag) //里程读取完成标志，下面是把里程作为附加字段加到位置上报信息中
    {
      temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x01;//添加附加信息ID 0x01--里程
			temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x04;//添加附加信息长度  里程有4个字节
			memcpy(temp_upload_msgbody_arry+temp_upload_msgbody_Len,OBD_mileage_buffer_arry,4); //添加里程内容，4个字节
      temp_upload_msgbody_Len +=4; //调整长度值
    }  //里程处理结束
		
		if(OBD_Com_Flag.OBD_ACC_Turn)//ACC有变化，需要上传剩余油量信息
		{
      if(OBD_Com_Flag.OBD_oil_massReceivded_FLag) //剩余油量读取成功，可以访问剩余油量缓存
			{
        temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0xEB;//添加附加信息ID 0x02--油量,对应车上里程表油量 	WORD,1/10L
			  temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x07;//添加附加信息长度  扩展油量信息长度是7字节
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x05;//
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x01;//				
			  memcpy(temp_upload_msgbody_arry+temp_upload_msgbody_Len,OBD_oil_mass_buffer_arry,2); //添加里程内容，4个字节
        temp_upload_msgbody_Len +=2; //调整长度值
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//	
      }
			else
			{
        Read_oil_mass(); //读取剩余油量函数
				if(OBD_Com_Flag.OBD_oil_massReceivded_FLag) //剩余油量读取成功，可以访问剩余油量缓存
			  {
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0xEB;//添加附加信息ID 0x02--油量,对应车上里程表油量 	WORD,1/10L
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x07;//添加附加信息长度  扩展油量信息长度是7字节
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x05;//
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x01;//				
					memcpy(temp_upload_msgbody_arry+temp_upload_msgbody_Len,OBD_oil_mass_buffer_arry,2); //添加里程内容，4个字节
					temp_upload_msgbody_Len +=2; //调整长度值
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//	
        }
      }
    }	//油量处理结束	    
 // }
//	if()
//添加附加字段

  Head_mesg.message_ID =0x0200; //赋值消息ID
  Head_mesg.message_property = (u16)temp_upload_msgbody_Len; //赋值消息体属性
  Head_mesg.liu_shui +=1u; //计算流水号

  temp_upload_Len = Head_deal(Head_mesg,temp_upload_arry);//返回的消息头+ 0x7e的长度
  memcpy(temp_upload_arry+temp_upload_Len,(u8 *)temp_upload_msgbody_arry,temp_upload_msgbody_Len); //添加基本位置信息
  temp_upload_Len+=temp_upload_msgbody_Len; //此时temp_upload_Len是没有转义前，不算校验码和结尾标志0x7e



  temp_upload_arry[temp_upload_Len]= add_check_number(temp_upload_arry, (temp_upload_Len+2)); //添加校验码
  temp_upload_Len = package((u8 *)upload_arry,temp_upload_arry,(u16)(temp_upload_Len+2)); //打包
  if(GSM_Send_Data((u8 *)upload_arry,temp_upload_Len+0))
		return TRUE;
	return FALSE;
}

//位置信息查询应答 ID
//函数返回值：TRUE:发送成功，这个成功指的是收到GSM模块返回的OK
//输入参数：u8 *p_in:应答流水号的地址
u8 response_upload(u8 *p_in)
{
//位置基本信息的长度是28字节	
  u8 temp_upload_msgbody_arry[240]={0};//临时存放消息体: 位置基本信息+附加信息，暂定其长度是240个字节
  u8 temp_upload_msgbody_Len=0; //消息体长度
  u8 temp_upload_arry[255] ={0x7e}; //上传数据的数据存放数组
  u8 temp_upload_Len =0; //上传数据的长度

  memcpy(temp_upload_msgbody_arry,p_in,2);//添加应答流水号
  temp_upload_msgbody_Len =2;
  u32_to_u8arry(alarm_flag,temp_upload_msgbody_arry+temp_upload_msgbody_Len);//处理报警标志
  temp_upload_msgbody_Len +=4; 

///////////////////读取状态位
  OBD_Com_Flag.OBD_ReadBodyState_Flag = 1; //将主动读取车身状态位置1
  OBD_Read_Body_state();  //调用读取车身状态信息函数  
  OBD_Com_Flag.OBD_ReadBodyState_Flag = 0; //将主动读取车身状态位清除
  u32_to_u8arry(Body_state,temp_upload_msgbody_arry+temp_upload_msgbody_Len);//处理状态
	temp_upload_msgbody_Len+=4;
	memcpy(temp_upload_msgbody_arry+temp_upload_msgbody_Len,GPS_DATA_BACKUP,20); //将处理完成的GPS数据放到临时存放消息体的数组里面
  temp_upload_msgbody_Len +=20;
////////////////////////////////////////////////////

//添加附加字段
  if(OBD_Com_Flag.ACC_state) //添加里程，前判断ACC为开，才会进行下面动作
	{		
    mileage();//读取里程函数
		if(OBD_Com_Flag.OBD_Mileage_Receivded_FLag) //里程读取完成标志，下面是把里程作为附加字段加到位置上报信息中
    {
      temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x01;//添加附加信息ID 0x01--里程
			temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x04;//添加附加信息长度  里程有4个字节
			memcpy(temp_upload_msgbody_arry+temp_upload_msgbody_Len,OBD_mileage_buffer_arry,4); //添加里程内容，4个字节
      temp_upload_msgbody_Len +=4; //调整长度值
    }  //里程处理结束
		
		if(OBD_Com_Flag.OBD_ACC_Turn)//ACC有变化，需要上传剩余油量信息
		{
      if(OBD_Com_Flag.OBD_oil_massReceivded_FLag) //剩余油量读取成功，可以访问剩余油量缓存
			{
        temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0xEB;//添加附加信息ID 0x02--油量,对应车上里程表油量 	WORD,1/10L
			  temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x07;//添加附加信息长度  扩展油量信息长度是7字节
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x05;//
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x01;//				
			  memcpy(temp_upload_msgbody_arry+temp_upload_msgbody_Len,OBD_oil_mass_buffer_arry,2); //添加里程内容，4个字节
        temp_upload_msgbody_Len +=2; //调整长度值
				temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//	
      }
			else
			{
        Read_oil_mass(); //读取剩余油量函数
				if(OBD_Com_Flag.OBD_oil_massReceivded_FLag) //剩余油量读取成功，可以访问剩余油量缓存
			  {
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0xEB;//添加附加信息ID 0x02--油量,对应车上里程表油量 	WORD,1/10L
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x07;//添加附加信息长度  扩展油量信息长度是7字节
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x05;//
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x01;//				
					memcpy(temp_upload_msgbody_arry+temp_upload_msgbody_Len,OBD_oil_mass_buffer_arry,2); //添加里程内容，4个字节
					temp_upload_msgbody_Len +=2; //调整长度值
					temp_upload_msgbody_arry[temp_upload_msgbody_Len++] =0x00;//	
        }
      }
    }	//油量处理结束	    
  }
//	if()
//添加附加字段

  Head_mesg.message_ID =0x0201; //位置信息查询应答ID
  Head_mesg.message_property = (u16)temp_upload_msgbody_Len; //赋值消息体属性
  Head_mesg.liu_shui +=1u; //计算流水号

  temp_upload_Len = Head_deal(Head_mesg,temp_upload_arry);//返回的消息头+ 0x7e的长度
  memcpy(temp_upload_arry+temp_upload_Len,(u8 *)temp_upload_msgbody_arry,temp_upload_msgbody_Len); //添加基本位置信息
  temp_upload_Len+=temp_upload_msgbody_Len; //此时temp_upload_Len是没有转义前，不算校验码和结尾标志0x7e



  temp_upload_arry[temp_upload_Len]= add_check_number(temp_upload_arry, (temp_upload_Len+2)); //添加校验码
  temp_upload_Len = package((u8 *)upload_arry,temp_upload_arry,(u16)(temp_upload_Len+2)); //打包
  if(GSM_Send_Data((u8 *)upload_arry,temp_upload_Len+0))
		return TRUE;
	return FALSE;
}

//32位数，转换成4个8位数放到数组里面
//参数：u32 in, 需要转换的32位数,
//      u8 *out_p, 接收转后后的4个字节的数组
//说明, 输入的数是4字节32位的数，输出的是四个字节的数组，将32位数的低位放在数组的高元素中，这样看起来是直接分割的。
void u32_to_u8arry(u32 in, u8 *out_p)
{
  u8 i=0;
	if(0x04 != sizeof(in))//判断数输入的数是不是32位，即4个字节。
	  return;
	for(i=0;i<4;i++)
	 *(out_p+(3-i)) =(u8)(in>>(i*8)&(0x000000FF));
}

//车身状态发生变化，立即上传位置信息
//是对这个变量的操作OBD_Com_Flag.OBD_BodyState_Turn_Flag
void BodyChang_UpLoad(void)
{
  if(OBD_Com_Flag.OBD_BodyState_Turn_Flag)
	{
    send_upload(0);
		OBD_Com_Flag.OBD_BodyState_Turn_Flag = 0 ;
		
  }
}

//终端通用应答函数
//输入参数是个结构体，包含了应答流水号，应答ID，应答结果
//type: 0:成功/确认;  1:失败;  2:消息有误;  3:不支持
u8 Terminal_TongYong_response(REsponse TongYongResp)
{
  u8 temp_upload_respbody_arry[5]={0};//临时存放消息体: 位置基本信息+附加信息，暂定其长度是240个字节  
  u8 temp_upload_arry[20] ={0x7e}; //上传数据的数据存放数组
  u8 temp_upload_Len =0; //上传数据的长度

//处理通用应答消息体，共5个字节 0-1:应答流水号， 2-3:应答ID， 4:结果
  memcpy(temp_upload_respbody_arry,TongYongResp.pliu_shui,2);//添加应答流水号
  memcpy(temp_upload_respbody_arry+2,TongYongResp.pmessage_ID,2);//添加应答ID
  temp_upload_respbody_arry[4] = TongYongResp.type ;//添加结果

//处理消息头
  Head_mesg.message_ID =0x0001; //终端通用应答ID
  Head_mesg.message_property = (u16)0x05; //赋值消息体属性
  Head_mesg.liu_shui +=1u; //计算流水号

  temp_upload_Len = Head_deal(Head_mesg,temp_upload_arry);//返回的消息头+ 0x7e的长度
  memcpy(temp_upload_arry+temp_upload_Len,(u8 *)temp_upload_respbody_arry,0x05); //添加应答消息体
  temp_upload_Len+=0x05; //此时0x05是没有转义前，不算校验码和结尾标志0x7e



  temp_upload_arry[temp_upload_Len]= add_check_number(temp_upload_arry, (temp_upload_Len+2)); //添加校验码
  temp_upload_Len = package((u8 *)upload_arry,temp_upload_arry,(u16)(temp_upload_Len+2)); //打包
  if(GSM_Send_Data((u8 *)upload_arry,temp_upload_Len+0))
		return TRUE;
	return FALSE;
}


//参数设置函数
//输入参数：
//u8 *p:  有效参数起始地址，即设置终端参数消息体的起始字节--参数总数字段对应的地址
//u16 len: 长度，
////PPP包:[7E 81 03 00 3C 01 78 20 00 00 09 00 1A 07 00 00 00 57 04 00 00 00 00 00 00 00 58 04 00 00 00 00 00 00 00 59 04 00 00 00 00 00 00 00 5A 04 00 00 00 00 00 00 00 5B 02 00 00 00 00 00 5C 02 00 00 00 00 00 80 04 00 00 00 00 7C 7E ]-15:06:14

void Parameter_Set(u8 *p,u16 len)
{ 
	u8  Para_Num =0 ; //参数总数
	u32 Para_ID = 0; //参数ID, DWORD类型
  u8  Para_len = 0; //参数长度，一个参数ID对应一个参数长度
	u8 i =0;
	u8 j =0 ;
	u32 temp_32 = 0; //用来临时处理32位的数
	u16 temp_16 =0;  //用来临时处理16位的数
	u8 temp_arry[5] ={0}; //用来处理
	Para_Num = *p; //提取参数总长度
	++p;
	for(i=0;i<Para_Num;i++)
	{
    Para_ID = (p[0]<<24)|(p[1]<<16)|(p[2]<<8)|p[3];
		Para_len = p[4];
		p +=5;  //ID+1固定长度为5，将指针跳过ID+参数长度段，指向参数的起始地址
		switch(Para_ID)
		{
			//PPP?:[7E 81 03 00 0A 01 78 20 00 00 09 00 27 01 00 00 00 01 04 00 00 02 24 DD 7E ]-10:13:09
       case 0x00000001:    SPI_Flash_SafeWrite(p,upheartime_addr,Para_len);  //发送心跳间隔
				 break ;
			 case 0x00000011:      //主服务器无线通信拨号用户名，即,GPRS拨号用户名
				 break ;
			 case 0x00000012:      //主服务器无线通信拨号密码，即,GPRS拨号密码
				 break ;
			 //PPP?:[7E 81 03 00 15 01 78 10 00 00 00 00 8C 01 00 00 00 13 0F 32 32 30 2E 31 37 38 2E 30 30 30 2E 30 35 36 4C 7E ]-14:31:40
			 //此条发送的时候输入的:220.178.0.56,  服务器会自动补成:220.178.000.056
			 case 0x00000013:      SPI_Flash_SafeWrite(&Para_len,IPaddress_addr,1);  //IP长度
			                       SPI_Flash_SafeWrite(p,IPaddress_addr+1,Para_len);//主服务器IP地址
				 break ;
			 //PPP?:[7E 81 03 00 0A 01 78 10 00 00 00 01 9A 01 00 00 00 19 04 00 00 25 C4 87 7E ]-16:50:14
			 case 0x00000019:    temp_16 = ((p[2]<<8)|p[3]);  //虽然这个参数是四个字节的，但是端口号只用最低的两个字节
			  //服务器UDP端口	   //第一种：加3移位法 得到16位2进制数转换成BCD码
			                       temp_32 =0;  j =0;			                       
			                       for(j=0;j<16;j++)
			                       {
                               temp_32 = temp_32 << 1;
															 temp_32 |=(u32)((temp_16>>(15-j))&0x0001); //需要左移16次
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
															 SPI_Flash_SafeWrite(&j,PortNumber_addr,1);  //PORT长度
			                         SPI_Flash_SafeWrite(temp_arry,PortNumber_addr+2,j);//PORT
                             }
                             else if(temp_arry[1]>0x30) 
                             {
                               j = 0x04;
															 SPI_Flash_SafeWrite(&j,PortNumber_addr,1);  //PORT长度
			                         SPI_Flash_SafeWrite(temp_arry+1,PortNumber_addr+2,j);//PORT
                             }
														 else if(temp_arry[2]>0x30) 
                             {
                               j = 0x03;
															 SPI_Flash_SafeWrite(&j,PortNumber_addr,1);  //PORT长度
			                         SPI_Flash_SafeWrite(temp_arry+2,PortNumber_addr+2,j);//PORT
                             }														 
														 else if(temp_arry[3]>0x30) 
                             {
                               j = 0x02;
															 SPI_Flash_SafeWrite(&j,PortNumber_addr,1);  //PORT长度
			                         SPI_Flash_SafeWrite(temp_arry+3,PortNumber_addr+2,j);//PORT
                             }
														 else if(temp_arry[4]>0x30) 
                             {
                               j = 0x01;
															 SPI_Flash_SafeWrite(&j,PortNumber_addr,1);  //PORT长度
			                         SPI_Flash_SafeWrite(temp_arry+4,PortNumber_addr+2,j);//PORT
                             }
									/* 第二种：直接除法得到16位2进制数转换成BCD码
            								 temp_arry[0] = temp_16/10000;
														 temp_arry[1] = (temp_16/1000) - ((temp_16/10000)*10);
														 temp_arry[2] = (temp_16/100) - ((temp_16/1000)*10);
														 temp_arry[3] = (temp_16/10) - ((temp_16/100)*10);
														 temp_arry[4] = (temp_16/1) - ((temp_16/10)*10);  */
				                                //服务器UDP端口				 
				 break ;
														 
			 case 0x00000027:      //休眠时汇报时间间隔，单位为秒(S), >0
				                     temp_32 = (p[0]<<24)|(p[1]<<16)|(p[2]<<8)|p[3] ;
			                       if((temp_32 > 0x0000FFFF)||(temp_32 < (u32)0x05))
															 break ;
														 SPI_Flash_SafeWrite(p+2,ACCOFF_GPSuploadtime_add,0x02);
				 break ;
			 case 0x00000028:      temp_32 = (p[0]<<24)|(p[1]<<16)|(p[2]<<8)|p[3] ;
			                       if((temp_32 > 250)||(temp_32 < 2))
															 break ;
														 SPI_Flash_SafeWrite(p+2,JinJi_addr,0x02);//紧急报警时汇报时间间隔，单位为秒(S), >0
				 break ;
														 //PPP?:[7E 81 03 00 08 01 78 10 00 00 00 03 A9 01 00 00 00 31 02 00 64 1F 7E ]-16:27:22
			 case 0x00000031:      //电子围栏半径(非法位移阈值，譬如，车违停被拖车了)，单位为米
				                     temp_16 = (p[0]<<8)|p[1] ;
				                     if((temp_16 ==0xFFFF)||(temp_16 < 0x0a))  //电子围栏，取值范围
														   break ;
														 SPI_Flash_SafeWrite(p,R_rail_addr,0x02);														 
				 break ;
														 
			 case 0x00000040:      //(刘：暂时不做)设置监控平台电话号码
				 break ;
       case 0x00000041:      //(刘:暂时不做)复位电话号码，可采用此电话号码拨打终端电话让终端复位
				 break ;
			 case 0x00000042:      //恢复出厂设置电话号码，可采用此电话号码拨打终端电话，让终端回复出厂设置
				 break ;
			 case 0x00000043:      //监控平台SMS电话号码
				 break ;
			 case 0x00000044:      //接收终端SMS文本报警号码，即有警情时，终端往这个号码发送报警短信
				 break ;
			 case 0x00000048:      //监听电话号码，
				 break ;
			 case 0x00000055:      //最高速度，单位为公里每小时(km/h),就是超速值
				                     temp_32 = (p[0]<<24)|(p[1]<<16)|(p[2]<<8)|p[3] ;
				                     if((temp_32 ==0xFFFF)||(temp_32 < 0x0a))   //超速值，取值范围
															 break ;
														 SPI_Flash_SafeWrite(p+2,Over_Speed_addr,0x02);
				 break ;
       case 0x00000056:      //超速持续时间，单位为秒(s),一般设为10s
				                     temp_32 = (p[0]<<24)|(p[1]<<16)|(p[2]<<8)|p[3] ;
				                     if((temp_32 ==0xFFFF)||(temp_32 < 0x0a))  //超速值，取值范围
														   break ;															 
														 SPI_Flash_SafeWrite(p+2,Over_Speed_XDTime_addr,0x02);
				 break ;
			 case 0x00000081:      //车辆所在省域ID
				                     temp_16 = (p[0]<<8)|p[1] ;
				                     if(temp_16 ==0xFFFF)  //||(temp_32 < 0x0a))  //超速值，取值范围
														   break ;															 
														 SPI_Flash_SafeWrite(p,Province_ID_addr,0x02);
				 break ;
       case 0x00000082:      //车辆所在市域ID
				                     temp_16 = (p[0]<<8)|p[1] ;
				                     if(temp_16 ==0xFFFF)  //||(temp_32 < 0x0a))  //超速值，取值范围
														   break ;															 
														 SPI_Flash_SafeWrite(p,City_ID_addr,0x02);
				 break ; 
			 case 0x00000083:      //公安交通管理部门颁发的机动车号牌，这个长度是不定的
				                      if((Para_len>0)&&(Para_len < 14))
															{
                                SPI_Flash_SafeWrite(&Para_len,Vehicle_license_Plate_Len_addr,0x01);
																SPI_Flash_SafeWrite(p,Vehicle_license_Plate_addr,Para_len);
                              }			 
				 break ;
															
			 case 0x00000084:      //车牌颜色，按照JT/T415-2006的5.4.12
				                      if(p[0] != 0xff)  SPI_Flash_SafeWrite(p,Vehicle_license_Plate_colour_addr,0x01);                             			  
				 break ;
       
			 case 0x00000090:      //GNSS定位模式 bit0,0:禁用GPS定位, 1:启用GPS定位;   bit1, 0:禁用北斗定位, 1:启用北斗定位
				                     if(p[0] <= 0x03)  SPI_Flash_SafeWrite(p,GNSS_MODE_addr,0x01);
				 break ;
			 
			 case 0x00000091:      //GNSS波特率，定义如下:0x00-4800; 0x01-9600; 0x02-19200; 0x03-38400; 0x04-57600; 0x05-115200
				                     if(p[0] <= 0x05)  SPI_Flash_SafeWrite(p,GNSS_Baud_addr,0x01);
				 break ;
			 
      
			 case 0x00000092:      if(p[0] <= 0x04)  SPI_Flash_SafeWrite(p,GNSS_Hz_addr,0x01);  ////GNSS模块详细定位数据输出频率 
				 break ;

    }
		
	  p +=Para_len; //将指针指向下一个参数的ID
		
  }
	

}




























