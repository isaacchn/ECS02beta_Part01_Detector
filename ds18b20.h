#ifndef _DS18B20_H_
#define _DS18B20_H_

#define uchar unsigned char
#define uint unsigned int

void ds18b20_delays();
void ds18b20_delay(uint N);
uchar ds18b20_Reset(void);
uchar ds18b20_read_bit(void);
void ds18b20_write_bit(uchar bitval);
uchar ds18b20_read_byte(void);
void ds18b20_write_byte(uchar val);
uint ds18b20_getTempMultiply16(void);
uchar ds18b20_getTMPL(void);
int ds18b20_getTemp(void);

#endif