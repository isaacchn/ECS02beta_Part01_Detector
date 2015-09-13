#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint  unsigned int

#define jump_ROM 0xCC
#define start 0x44
#define read_EEROM 0xBE


uchar TMPH,TMPL;
int TMP;

sbit DQ = P3^5;            //DS18B20数据口

void ds18b20_delay(uint N)
{
	int i;
	for(i=0; i<N; i++)
	;
}

uchar ds18b20_Reset(void)
{
	uchar deceive_ready;
	DQ = 0;
	ds18b20_delay(29);
	DQ = 1;
	ds18b20_delay(3);
	deceive_ready = DQ;
	ds18b20_delay(25);
	return(deceive_ready);
}

uchar ds18b20_read_bit(void)
{
	uchar i;
	DQ = 0;
	DQ = 1;
	for(i=0; i<3; i++);
	return(DQ);
}

void ds18b20_write_bit(uchar bitval)
{
	DQ=0;
	if(bitval==1)
		DQ=1;
	ds18b20_delay(5);
	DQ=1;
}

uchar ds18b20_read_byte(void)
{
	uchar i,m,receive_data;
	m = 1;
	receive_data = 0;
	for(i=0; i<8; i++)
	{
		if(ds18b20_read_bit())
		{
			receive_data = receive_data + (m << i);
		}
		ds18b20_delay(6);
	}
	return(receive_data);
}

void ds18b20_write_byte(uchar val)
{
	uchar i,temp;
	for(i=0; i<8; i++)
	{
		temp = val >> i;
		temp = temp & 0x01;
		ds18b20_write_bit(temp);
		ds18b20_delay(5);
	}
}
/*
**	返回ds18b20测得温度值
*/
int ds18b20_getTemp(void)
{
	ds18b20_Reset();
	ds18b20_write_byte(jump_ROM);
	ds18b20_write_byte(start);
	ds18b20_Reset();
	ds18b20_write_byte(jump_ROM);
	ds18b20_write_byte(read_EEROM);
	TMPL = ds18b20_read_byte();
	TMPH = ds18b20_read_byte();
	if(TMPH>=0xf8)	//TMPH高4位为1111 温度为零下
	{
		return	(0-(TMPL/16+(TMPH%0xf8)*16))*100;
	}
	else	//温度零上
	{
		return TMPL/16+TMPH*16;
	}
}
uchar ds18b20_getTMPL(void)
{
	return TMPL;
}