#ifndef _LCD1602_H_
#define _LCD1602_H_

#define uint unsigned int
#define uchar unsigned char

/*
所用开发板与1602接口对应关系
J17_1 -- VSS -- GND
J17_2 -- VDD -- 5V
J17_3 -- VO -- W1 调节对比度
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

void lcd1602_delay(uint z);
void lcd1602_write_com(uchar com);
void lcd1602_write_data(uchar dat);
void lcd1602_string(uchar hang,uchar lie,uchar *p);
void lcd1602_init();
void lcd1602_char(uchar hang,uchar lie,uchar p);
void lcd1602_clear();

#endif
