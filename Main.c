/**********************************************************************************
 * �ļ���  ��Main.c
 * ����    ����ȡDS18B20�¶����� ͨ��CAN���ߴ��䵽��λ��         
 * ʵ��ƽ̨��STC90C516RD+ MCP2515  
**********************************************************************************/
#include <ds18b20.h>
#include <MCP2515.h>
#include <reg51.h>
#include <string.h>
#include <DHT11.h>

#define T0_CLR 80	//T0�ﵽ����ֵʱ���� ��110592�����½�ϳ�ֵ50000��Լ��4.34��

uint T0_count = 0;	//T0�жϴ���,��ʱ����
uchar T0_flag = 0;	//��T0�жϴﵽһ������ʱ T0_flag��1 �ɼ�ds18b20����
//uint Tmp_18b20;		//��ds18b20��ȡ���¶�����

/*******************************************************************************
* ������  :	T0_init 
* ����    : T0��ʱ����ʼ��
* ����    :	
* ���    :	
* ����ֵ  :	
* ˵��    :	
*******************************************************************************/
void T0_init(void)
{
	TMOD &= 0xF0;
    TMOD |= 0x01;	//T0������ʽ1
	TH0 = (65536-10000)/256;
	TL0 = (65536-10000)%256;	//��ֵ
	TR0 = 1;	//����T0
	ET0 = 1;	//��T0�ж�
	EA = 1;	//�����ж�
}

/*******************************************************************************
* ������  :	GetTransferString 
* ����    : ����Ҫͨ��CAN���ߴ�����ַ���
* ����    : 
* ���    : 
* ����ֵ  : �ַ���ָ��
* ˵��    : ����ʵ���¶�Ϊ 0000 0001 0101 1100 ������ĸ�ʽӦΪ"TEMP=21.75"
*******************************************************************************/
uchar* GetTransferString(void)
{
	char tens_place,ones_place,decimal_1,decimal_2;	//�¶�����ʮλ ��λ С����һλ С���ڶ�λ
	char RHtens_place,RHones_place;	//ʪ������ʮλ ��λ
	uchar Rh;	//ʪ��
	int ds18b20_int = ds18b20_getTemp();	//��DS18B20��ȡ�¶ȵ���������
	uchar ds18b20_tmpl = ds18b20_getTMPL();	//��DS18B20��ȡTMPL ���¶ȵĵͰ�λ �Ӷ����С��
	uchar decimal;	//��������Ϊ0110 1101 ��decimal=75
	uchar CanSendBuffer[50] = " [T=";
	/*�����¶��ַ���*/
	if(ds18b20_int>=0)	//�¶Ȳ�Ϊ����
	{
		tens_place = (char)((ds18b20_int%100)/10+48);
		ones_place = (char)(ds18b20_int%10+48);
	}
	else	//�¶�Ϊ����
	{
		tens_place = (char)((-ds18b20_int%100)/10+48);
		ones_place = (char)(-ds18b20_int%10+48);
	}
	decimal = (ds18b20_tmpl%16)/8*50+(ds18b20_tmpl%8)/4*25;	//��������Ϊ0110 1101 ��decimal=75
	decimal_1 = (char)(decimal/10+48);		
	decimal_2 = (char)(decimal%10+48);
	/*����ʪ���ַ���*/
	DHT_getDat();	//����DHT11�������� �����ܻ�ȡ
	Rh = DHT_getRHH();
	RHtens_place = (char)((Rh%100)/10+48);
	RHones_place = (char)(Rh%10+48);

	if(ds18b20_int>=0)	//�¶Ȳ�Ϊ����
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
	else	//�¶�Ϊ����
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
* ������  :	main 
* ����    : 
* ����    : 
* ���    : 
* ����ֵ  : 
* ˵��    : 
*******************************************************************************/
void main()
{
	uchar* CanSend;
	uchar CanSendLength;
	T0_init();
	MCP2515_Init();
	while(1)
	{
		if(T0_flag==1)	//��ʱ�ﵽһ������
		{
			T0_flag = 0;
			CanSend = GetTransferString();	//����Ҫ���͵��ַ���
			CanSendLength = strlen(CanSend);
			CAN_Send_Buffer(CanSend,CanSendLength);	//���ͣ�
		}
	}
}
/*******************************************************************************
* ������  :	T0_timer 
* ����    : T0�жϷ������
* ����    : 
* ���    : 
* ����ֵ  : 
* ˵��    : 
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