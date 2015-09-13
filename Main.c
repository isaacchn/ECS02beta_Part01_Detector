/**********************************************************************************
 * 文件名  ：Main.c
 * 描述    ：获取DS18B20温度数据 通过CAN总线传输到上位机         
 * 实验平台：STC90C516RD+ MCP2515  
**********************************************************************************/
#include <ds18b20.h>
#include <MCP2515.h>
#include <reg51.h>
#include <string.h>
#include <DHT11.h>

#define T0_CLR 80	//T0达到该数值时清零 在110592晶振下结合初值50000大约是4.34秒

uint T0_count = 0;	//T0中断次数,定时清零
uchar T0_flag = 0;	//当T0中断达到一定次数时 T0_flag置1 采集ds18b20数据
//uint Tmp_18b20;		//从ds18b20获取的温度数据

/*******************************************************************************
* 函数名  :	T0_init 
* 描述    : T0定时器初始化
* 输入    :	
* 输出    :	
* 返回值  :	
* 说明    :	
*******************************************************************************/
void T0_init(void)
{
	TMOD &= 0xF0;
    TMOD |= 0x01;	//T0工作方式1
	TH0 = (65536-10000)/256;
	TL0 = (65536-10000)%256;	//初值
	TR0 = 1;	//启动T0
	ET0 = 1;	//开T0中断
	EA = 1;	//开总中断
}

/*******************************************************************************
* 函数名  :	GetTransferString 
* 描述    : 生成要通过CAN总线传输的字符串
* 输入    : 
* 输出    : 
* 返回值  : 字符串指针
* 说明    : 假如实际温度为 0000 0001 0101 1100 则输出的格式应为"TEMP=21.75"
*******************************************************************************/
uchar* GetTransferString(void)
{
	char tens_place,ones_place,decimal_1,decimal_2;	//温度数据十位 个位 小数第一位 小数第二位
	char RHtens_place,RHones_place;	//湿度数据十位 个位
	uchar Rh;	//湿度
	int ds18b20_int = ds18b20_getTemp();	//从DS18B20获取温度的整型数据
	uchar ds18b20_tmpl = ds18b20_getTMPL();	//从DS18B20获取TMPL 即温度的低八位 从而获得小数
	uchar decimal;	//假设输入为0110 1101 则decimal=75
	uchar CanSendBuffer[50] = " [T=";
	/*生成温度字符串*/
	if(ds18b20_int>=0)	//温度不为负数
	{
		tens_place = (char)((ds18b20_int%100)/10+48);
		ones_place = (char)(ds18b20_int%10+48);
	}
	else	//温度为负数
	{
		tens_place = (char)((-ds18b20_int%100)/10+48);
		ones_place = (char)(-ds18b20_int%10+48);
	}
	decimal = (ds18b20_tmpl%16)/8*50+(ds18b20_tmpl%8)/4*25;	//假设输入为0110 1101 则decimal=75
	decimal_1 = (char)(decimal/10+48);		
	decimal_2 = (char)(decimal%10+48);
	/*生成湿度字符串*/
	DHT_getDat();	//先由DHT11生成数据 随后才能获取
	Rh = DHT_getRHH();
	RHtens_place = (char)((Rh%100)/10+48);
	RHones_place = (char)(Rh%10+48);

	if(ds18b20_int>=0)	//温度不为负数
	{
		CanSendBuffer[4] = '+';
		CanSendBuffer[5] = tens_place;
		CanSendBuffer[6] = ones_place;
		CanSendBuffer[7] = '.';
		CanSendBuffer[8] = decimal_1;
		CanSendBuffer[9] = decimal_2;
		CanSendBuffer[10] = 'R';
		CanSendBuffer[11] = '=';
		CanSendBuffer[12] = RHtens_place;
		CanSendBuffer[13] = RHones_place;
		CanSendBuffer[14] = ']';
		CanSendBuffer[15] = '\0';
	}
	else	//温度为负数
	{
		CanSendBuffer[4] = '-';
		CanSendBuffer[5] = tens_place;
		CanSendBuffer[6] = ones_place;
		CanSendBuffer[7] = '.';
		CanSendBuffer[8] = decimal_1;
		CanSendBuffer[9] = decimal_2;
		CanSendBuffer[10] = 'R';
		CanSendBuffer[11] = '=';
		CanSendBuffer[12] = RHtens_place;
		CanSendBuffer[13] = RHones_place;
		CanSendBuffer[14] = ']';
		CanSendBuffer[15] = '\0';
	}
	return CanSendBuffer;
}
/*******************************************************************************
* 函数名  :	main 
* 描述    : 
* 输入    : 
* 输出    : 
* 返回值  : 
* 说明    : 
*******************************************************************************/
void main()
{
	uchar* CanSend;
	uchar CanSendLength;
	T0_init();
	MCP2515_Init();
	while(1)
	{
		if(T0_flag==1)	//计时达到一个周期
		{
			T0_flag = 0;
			CanSend = GetTransferString();	//生成要发送的字符串
			CanSendLength = strlen(CanSend);
			CAN_Send_Buffer(CanSend,CanSendLength);	//发送！
		}
	}
}
/*******************************************************************************
* 函数名  :	T0_timer 
* 描述    : T0中断服务程序
* 输入    : 
* 输出    : 
* 返回值  : 
* 说明    : 
*******************************************************************************/
void T0_timer() interrupt 1
{
	TH0 = (65536-50000)/256;
	TL0 = (65536-50000)%256;
	T0_count++;
	if(T0_count==T0_CLR)
	{
		T0_count = 0;
		T0_flag = 1;
	}	
}