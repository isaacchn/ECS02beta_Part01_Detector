/*
@date 20140819
@author isaac
@comment ��1602Һ������̬��������д��
*/
#include <reg52.h>
#include <lcd1602.h>

/*
���ÿ�������1602�ӿڶ�Ӧ��ϵ
J17_1 -- VSS -- GND
J17_2 -- VDD -- 5V
J17_3 -- VO -- W1 ���ڶԱȶ�
J17_4 -- RS -- P25
J17_5 -- R/W -- P26
J17_6 -- E -- P27
J17_7 -- D0 -- P00
J17_8 -- D1 -- P01
J17_9 -- D2 -- P02
J17_10 -- d3 -- P03
J17_11 -- d4 -- P04
J17_12 -- d5 -- P05
J17_13 -- d6 -- P06
J17_14 -- d7 -- P07
J17_15 -- bla -- 5v
J17_16 -- blk -- gnd
*/
sbit lcd_en=P2^7;
sbit lcd_rw=P2^6;
sbit lcd_rs=P2^5;

void lcd1602_delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
void lcd1602_write_com(uchar com)//дָ���
{
	lcd_rs=0;
	lcd_rw=0;//ѡ��дָ��ģʽ
	P0=com;
	lcd1602_delay(5);
	lcd_en=1;
	lcd1602_delay(5);
	lcd_en=0;//E������
}
void lcd1602_write_data(uchar dat)//д���ݷ���
{
	lcd_rs=1;
	lcd_rw=0;
	P0=dat;
	lcd1602_delay(5);
	lcd_en=1;
	lcd1602_delay(5);
	lcd_en=0;
}
void lcd1602_init()//��ʼ��1602Һ��
{
	lcd1602_write_com(0x38);//0011 1000	����16*2��ʾ��5*7����8λ���ݽӿ�
	lcd1602_write_com(0x0c);//0000 1100 ���ÿ���ʾ������ʾ���
	lcd1602_write_com(0x06);//0000 0110 ����дһ���ַ����ַָ���1
	lcd1602_write_com(0x01);//0000 0001 ��ʾ��0������ָ����0
}
void lcd1602_string(uchar hang,uchar lie,uchar *p)
{
	uchar a,b=0;
	if(hang == 1) a = 0x80;
	if(hang == 2) a = 0xc0;
	a = a + lie - 1;
	while(1)
	{
		lcd1602_write_com(a++);		
		if((*p == '\0')||(b==16)) 
			break;
		b++;
		lcd1602_write_data(*p);
		p++;
	}
}
void lcd1602_char(uchar hang,uchar lie,uchar p)
{
	uchar a,b=0;
	if(hang == 1) a = 0x80;
	if(hang == 2) a = 0xc0;
	a = a + lie - 1;
	lcd1602_write_com(a++);		
	b++;
	lcd1602_write_data(p);
}
void lcd1602_clear()
{
	lcd1602_write_com(0x01);//0000 0001 ��ʾ��0������ָ����0
}
