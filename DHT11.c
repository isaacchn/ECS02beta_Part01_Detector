/*
@author Isaac
@date 2014/9/7
@comment 参考其他程序所写 DHT11驱动程序 测试通过
*/
#include <reg52.h>
#include <DHT11.h>

sbit  DHT_P20  = P3^6;
uchar  FLAG,k;
uchar  DHT_TMPH,DHT_TMPL,DHT_RHH,DHT_RHL,DHT_CHK;
uchar  DHT_TMPH_temp,DHT_TMPL_temp,DHT_RHH_temp,DHT_RHL_temp,DHT_CHK_temp;
uchar  DHT_tempData;
uchar  DHT_tempBit;

void DHT_Delay100us(uint j)
{
	uchar i;
	for(; j>0; j--)
	{
		for(i=0; i<27; i++);

	}
}
void  DHT_Delay10us(void)
{
	uchar i;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
}

void  DHT_readByte(void)
{
	uchar i;
	for(i=0; i<8; i++)
	{

		FLAG=2;
		while((!DHT_P20)&&FLAG++);
		DHT_Delay10us();
		DHT_Delay10us();
		DHT_Delay10us();
		DHT_tempBit=0;
		if(DHT_P20)DHT_tempBit=1;
		FLAG=2;
		while((DHT_P20)&&FLAG++);
		//超时则跳出for循环
		if(FLAG==1)break;
		//判断数据位是0还是1

		// 如果高电平高过预定0高电平值则数据位为 1

		DHT_tempData<<=1;
		DHT_tempData|=DHT_tempBit;        //0
	}//rof
}//End of DHT_readByte()

void DHT_getDat(void)
{
	//主机拉低18ms
	DHT_P20=0;
	DHT_Delay100us(180);
	DHT_P20=1;
	//总线由上拉电阻拉高 主机延时20us
	DHT_Delay10us();
	DHT_Delay10us();
	DHT_Delay10us();
	DHT_Delay10us();
	//主机设为输入 判断从机响应信号
	DHT_P20=1;
	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行
	if(!DHT_P20)		 //T !
	{
		FLAG=2;
		//判断从机是否发出 80us 的低电平响应信号是否结束
		while((!DHT_P20)&&FLAG++);
		FLAG=2;
		//判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
		while((DHT_P20)&&FLAG++);
		//数据接收状态
		DHT_readByte();
		DHT_RHH_temp=DHT_tempData;
		DHT_readByte();
		DHT_RHL_temp=DHT_tempData;
		DHT_readByte();
		DHT_TMPH_temp=DHT_tempData;
		DHT_readByte();
		DHT_TMPL_temp=DHT_tempData;
		DHT_readByte();
		DHT_CHK_temp=DHT_tempData;
		DHT_P20=1;
		//数据校验
		if((DHT_TMPH_temp+DHT_TMPL_temp+DHT_RHH_temp+DHT_RHL_temp)==DHT_CHK_temp)
		{
			DHT_RHH=DHT_RHH_temp;
			DHT_RHL=DHT_RHL_temp;
			DHT_TMPH=DHT_TMPH_temp;
			DHT_TMPL=DHT_TMPL_temp;
			DHT_CHK=DHT_CHK_temp;
		}//fi
	}//fi
}//End of DHT_getDat()
uchar DHT_getTMP(void)
{
	return DHT_TMPH;
}
uchar DHT_getRHH(void)
{
	return DHT_RHH;
}
/*
//测试程序
void main()
{
	while(1)
	{
		//调用温湿度读取子程序
		DHT_getDat();
		lcd1602_init();
		lcd1602_char(1,4,DHT_TMPH/10%10+48);
		lcd1602_char(1,5,DHT_TMPH%10+48);
		lcd1602_char(1,7,DHT_RHH/10%10+48);
		lcd1602_char(1,8,DHT_RHH%10+48);
		//读取模块数据周期不易小于 2S
		DHT_Delay100us(20000);
	}//elihw

}// main
*/