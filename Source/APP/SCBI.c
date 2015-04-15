#include "SCBI.h"

//��ȡSCBI��Ϣ
//gpsx:nmea��Ϣ�ṹ��
//buf:���յ���GPS���ݻ������׵�ַ 
void SCBI_Analysis(scbi_msg *scbi,u8 *buf)
{
  	u8 *p1,p2[8],*p3,dx;			 
	u8 posx;
	p3=p2;    
	p1=(u8*)strstr((const char *)buf,"$SCBI");							 
	posx=NMEA_Comma_Pos(p1,1);								//�ٶ�
	if(posx!=0XFF)
	{
		scbi->Speed=NMEA_Str2num(p1+posx,&dx);
	}
	posx=NMEA_Comma_Pos(p1,2);
	if(posx!=0XFF)
	{
		scbi->Extend=NMEA_Str2num(p1+posx,&dx);				//��չ��
	}
	posx=NMEA_Comma_Pos(p1,3);
	p1=p1+posx;
	if(posx!=0XFF)
	{
		while(1)
		{
			if(*p1=='*'||*p1==',')
			{
				*p3=0;
				break;
			}
			*p3++=*p1++;
		}
		if(!strcmp((const char *)p2,"0FF"))
		{
			scbi->LED_State=0;				//LED�ر�
		}
		else if(!strcmp((const char *)p2,"ON"))
		{
			 scbi->LED_State=1;			  //LED��
		}
		else
		{
			scbi->LED_State=0;				//����
		}
	}
} 
