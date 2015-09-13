#ifndef _DHT11_H_
#define _DHT11_H_

#define uchar unsigned char
#define uint unsigned int

void DHT_Delay100us(uint j);
void  DHT_Delay10us(void);
void  DHT_readByte(void);
void DHT_getDat(void);
uchar DHT_getTMP(void);
uchar DHT_getRHH(void);

#endif