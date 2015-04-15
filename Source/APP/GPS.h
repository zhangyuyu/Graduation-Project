#include "ili9320.h"
#ifndef __GPS_H_
#define __GPS_H_

#define uchar unsigned char
#define uint  unsigned int

typedef struct{
	int year;  
	int month; 
	int  day;
	int hour;
	int minute;
	int second;
}DATE_TIME;

typedef struct{
	double  latitude;  //经度
	double  longitude; //纬度
	double 	speed;      //速度
	double 	direction;  //航向
	double 	height;    //水平面高度
	double 	altitude;  //海拔
	double  HDOP;
	double  VDOP;
	char 	NS;
	char 	EW;
	char    posnum;
	DATE_TIME D;
}GPS_INFO;

void GPS_Init(void);
int GPS_RMC_Parse( u8 *line,GPS_INFO *GPS);
int GPS_GGA_Parse( u8 *line,GPS_INFO *GPS);

void Int_To_Str(int x,char *Str);

#endif  //__GPS_H_
