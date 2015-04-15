#include "GPS.h"
#include "string.h"
#include "ili9320.h"
#include "stdio.h"

uchar GetComma(uchar num,char* str);
static double Get_Double_Number(char *s);
static void UTC2BTC(DATE_TIME *GPS);

void GPS_Init(void)
{
//	Lcd_DispLine(0, 0, init1);
//	Lcd_DispLine(1, 0, init2);
//	Lcd_DispLine(2, 0, init3);
//	Lcd_DispLine(3, 0, init4);
}
GPS_INFO GPS;

/* ---------------------------分析$GPRMC----------------------------- */
int GPS_RMC_Parse(u8 *line,GPS_INFO *GPS)
{
	uchar ch, status, tmp;
	int i=4;
	char out[20];
	double speed_tmp;
	char *buf = line;
	ch = buf[5];
	status = buf[GetComma(2, buf)];
//	GUI_Text(140,50,(u8 *)"test for analyse",16,Red,White);
	if (ch == 'C')  //如果第五个字符是C，($GPRMC)       
	{
		if (status == 'A')  //如果数据有效，则分析      
		{
//			GUI_Text(140,50,(u8 *)"test for analyse",16,Red,White);
            
			GPS -> NS       = buf[GetComma(4, buf)];
			ili9320_PutChar(240,50,GPS -> NS,Red,White);

			GPS -> EW       = buf[GetComma(6, buf)];
			ili9320_PutChar(240,30,GPS -> EW,Red,White);

			GPS->latitude   = Get_Double_Number(&buf[GetComma(3, buf)]);	 //纬度
            sprintf(out,"%lf",GPS->latitude);
			GUI_gpsNum(140,50,(u8 *)out,Red,White);

			GPS->longitude  = Get_Double_Number(&buf[GetComma( 5, buf)]);	 //经度
			sprintf(out,"%lf",GPS->longitude);
			GUI_gpsNum(140,30,(u8 *)out,Red,White);

			speed_tmp      = Get_Double_Number(&buf[GetComma(7, buf)]);        //速度(单位：海里/时)   
			GPS->speed     = speed_tmp * 1.85;                                 //1海里=1.85公里 
		    sprintf(out,"%lf",GPS->speed);
			GUI_gpsNum(140,110,(u8 *)out,Red,White); 

			GPS->direction = Get_Double_Number(&buf[GetComma(8, buf)]);        //角度	   		
			sprintf(out,"%lf",GPS->direction);
			GUI_gpsNum(140,130,(u8 *)out,Red,White);

			GPS->D.hour    = (buf[7] - '0') * 10 + (buf[8] - '0');		       //时间   
			GPS->D.minute  = (buf[9] - '0') * 10 + (buf[10] - '0');
			GPS->D.second  = (buf[11] - '0') * 10 + (buf[12] - '0');
			tmp = GetComma(9, buf);
			GPS->D.day     = (buf[tmp + 0] - '0') * 10 + (buf[tmp + 1] - '0'); //日期   
			GPS->D.month   = (buf[tmp + 2] - '0') * 10 + (buf[tmp + 3] - '0');
			GPS->D.year    = (buf[tmp + 4] - '0') * 10 + (buf[tmp + 5] - '0')+2000;
		
			UTC2BTC(&GPS->D);                                                //UTC时间转换


		    /*显示日期 */
			GUI_Word(140,150,4,GPS->D.year,0,Red,White);	    
			GUI_Text(140+8*i++,150,"/",1,Red,White);
			GUI_Word(140+8*i++,150,2,GPS->D.month,0,Red,White);	  i++;
			GUI_Text(140+8*i++,150,"/",1,Red,White);
			GUI_Word(140+8*i++,150,2,GPS->D.day,0,Red,White);	  i++;
			/*显示时间 */
			GUI_Word(140+8*i++,150,2,GPS->D.hour,0,Red,White);
			GUI_Text(140+8*i++,150,":",1,Red,White);
			GUI_Word(140+8*i++,150,2,GPS->D.minute,0,Red,White);  i++;
			GUI_Text(140+8*i++,150,":",1,Red,White);
			GUI_Word(140+8*i,150,2,GPS->D.second,0,Red,White);  i=i+4;
			return 1;
		}		
	}
	
	return 0;
}

/* ---------------------------分析$GPGGA----------------------------- */
int GPS_GGA_Parse(u8 *line,GPS_INFO *GPS)
{
	uchar ch, status;
	char *buf = line;
	char out[20];
	ch = buf[4];
	status = buf[GetComma(2, buf)];

	if (ch == 'G')  //$GPGGA
	{
		if (status != ',')
		{
			GUI_Text(140,210,(u8 *)"VDOP:",5,Blue,White);

            GPS->posnum = buf[GetComma(6, buf)]; 	   	                 //参与定位的卫星数	
			ili9320_PutChar(140,170,GPS->posnum,Red,White);	

            GPS->HDOP = Get_Double_Number(&buf[GetComma(8, buf)]); 	   	 //水平精度因子	
			sprintf(out,"%lf",GPS->HDOP);
			GUI_gpsNum(140,190,(u8 *)out,Red,White);	
			
			GPS->VDOP = Get_Double_Number(&buf[GetComma(13, buf)]);	   	 //垂直精度因子		
			sprintf(out,"%lf",GPS->VDOP);
			GUI_gpsNum(140,210,(u8 *)out,Red,White);	    

			GPS->altitude = Get_Double_Number(&buf[GetComma(9, buf)]);	  //海拔
			sprintf(out,"%lf",GPS->altitude);
			GUI_gpsNum(140,70,(u8 *)out,Red,White);

			GPS->height = Get_Double_Number(&buf[GetComma(11, buf)]);	  //高度
			sprintf(out,"%lf",GPS->height);
			GUI_gpsNum(140,90,(u8 *)out,Red,White);

			return 1;
		}
	}
	
	return 0;
}

/* ---------------------------分析$GPGSV----------------------------- 
int GPS_GSV_Parse(u8 *line,GPS_INFO *GPS)
{
	uchar ch, status;
	char *buf = line;
	ch = buf[5];
	status = buf[GetComma(2, buf)];

	if (ch == 'V')  //$GPGSV
	{
		if (status != ',')
		{
            GPS->viewnum = buf[GetComma(2, buf)]; 	   	                 //视野内卫星数	
			ili9320_PutChar(140,190,GPS->viewnum,Red,White);	
			return 1;
		}
	}	
	return 0;
}
*/
/* ---------------------------分析$GPGSA----------------------------- 
int GPS_GSA_Parse(u8 *line,GPS_INFO *GPS)
{
	uchar ch, status;
	char *buf = line;
	ch = buf[5];
	status = buf[GetComma(2, buf)];

	if (ch == 'A')  //$GPGSA
	{
		if (status != ',')
		{
            GPS->fixnum = buf[GetComma(1, buf)]; 	   	                 //模式
			switch(GPS->fixnum)
			{ 
			    case '1': 			GUI_Text(140,170,"invalid",1,Red,White);
			    case '2': 			GUI_Text(140,170,"2D",1,Red,White);
			    case '3': 			GUI_Text(140,170,"3D",1,Red,White);
			}	
		}
	}	
	return 0;
}
*/

static double Str_To_Double(char *buf)
{
	double rev = 0;
	double dat;
	int integer = 1;
	char *str = buf;
	int i;
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
			case '.':
				dat = '.';
				break;
		}
		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str ++;
			continue;
		}
		if( integer == 1 )
		{
			rev = rev * 10 + dat;
		}
		else
		{
			rev = rev + dat / (10 * i);
			i = i * 10 ;
		}
		str ++;
	}
	return rev;
}
												

static double Get_Double_Number(char *s)
{
	char buf[10];
	uchar i;
	double rev;
	i=GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev=Str_To_Double(buf);
	return rev;	
}


uchar GetComma(uchar num,char *str)
{
	uchar i,j = 0;
	int len=strlen(str);

	for(i = 0;i < len;i ++)
	{
		if(str[i] == ',')
			j++;
		if(j == num)
			return i + 1;	
	}

	return 0;	
}


static void UTC2BTC(DATE_TIME *GPS)
{
	GPS->second ++;  
	if(GPS->second > 59)
	{
		GPS->second = 0;
		GPS->minute ++;
		if(GPS->minute > 59)
		{
			GPS->minute = 0;
			GPS->hour ++;
		}
	}	

    GPS->hour = GPS->hour + 8;
	if(GPS->hour > 23)
	{
		GPS->hour -= 24;
		GPS->day += 1;
		if(GPS->month == 2 ||
		   		GPS->month == 4 ||
		   		GPS->month == 6 ||
		   		GPS->month == 9 ||
		   		GPS->month == 11 )
		{
			if(GPS->day > 30)
			{
		   		GPS->day = 1;
				GPS->month++;
			}
		}
		else
		{
			if(GPS->day > 31)
			{	
		   		GPS->day = 1;
				GPS->month ++;
			}
		}
		if(GPS->year % 4 == 0 )
		{
	   		if(GPS->day > 29 && GPS->month == 2)
			{		
	   			GPS->day = 1;
				GPS->month ++;
			}
		}
		else
		{
	   		if(GPS->day > 28 &&GPS->month == 2)
			{
	   			GPS->day = 1;
				GPS->month ++;
			}
		}
		if(GPS->month > 12)
		{
			GPS->month -= 12;
			GPS->year ++;
		}		
	}
}

void Int_To_Str(int x,char *Str)
{
	int t;
	char *Ptr,Buf[5];
	int i = 0;
	Ptr = Str;
	if(x < 10)		// 当整数小于10时,转化为"0x"的格式
	{
		*Ptr ++ = '0';
		*Ptr ++ = x+0x30;
	}
	else
	{
		while(x > 0)
		{
			t = x % 10;
			x = x / 10;
			Buf[i++] = t+0x30;	// 通过计算把数字转化成ASCII码形式
		}
		i -- ;
		for(;i >= 0;i --) 		// 将得到的字符串倒序
		{
			*(Ptr++) = Buf[i];
		}
	}
	*Ptr = '\0';
}

