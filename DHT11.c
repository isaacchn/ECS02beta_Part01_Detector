/*
@author Isaac
@date 2014/9/7
@comment �ο�����������д DHT11�������� ����ͨ��
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
		//��ʱ������forѭ��
		if(FLAG==1)break;
		//�ж�����λ��0����1

		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1

		DHT_tempData<<=1;
		DHT_tempData|=DHT_tempBit;        //0
	}//rof
}//End of DHT_readByte()

void DHT_getDat(void)
{
	//��������18ms
	DHT_P20=0;
	DHT_Delay100us(180);
	DHT_P20=1;
	//������������������ ������ʱ20us
	DHT_Delay10us();
	DHT_Delay10us();
	DHT_Delay10us();
	DHT_Delay10us();
	//������Ϊ���� �жϴӻ���Ӧ�ź�
	DHT_P20=1;
	//�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������
	if(!DHT_P20)		 //T !
	{
		FLAG=2;
		//�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����
		while((!DHT_P20)&&FLAG++);
		FLAG=2;
		//�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
		while((DHT_P20)&&FLAG++);
		//���ݽ���״̬
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
		//����У��
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
//���Գ���
void main()
{
	while(1)
	{
		//������ʪ�ȶ�ȡ�ӳ���
		DHT_getDat();
		lcd1602_init();
		lcd1602_char(1,4,DHT_TMPH/10%10+48);
		lcd1602_char(1,5,DHT_TMPH%10+48);
		lcd1602_char(1,7,DHT_RHH/10%10+48);
		lcd1602_char(1,8,DHT_RHH%10+48);
		//��ȡģ���������ڲ���С�� 2S
		DHT_Delay100us(20000);
	}//elihw

}// main
*/