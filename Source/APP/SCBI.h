#ifndef __SCBI_H
#define __SCBI_H
#include "stm32f10x.h"
#include "string.h"
#include "GPS.h"
typedef struct
{
  u16 Speed;
  u16 Extend;
  u16 LED_State;
}scbi_msg;
void SCBI_Analysis(scbi_msg *scbi,u8 *buf);		  //SCBI–≠“È∑÷Œˆ
#endif
