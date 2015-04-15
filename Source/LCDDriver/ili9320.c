#include "stm32f10x.h"
#include "ili9320.h"
#include "ili9320_font.h"

u16 DeviceCode;

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  vu16 LCD_REG;
  vu16 LCD_RAM;
} LCD_TypeDef;

/* LCD is connected to the FSMC_Bank1_NOR/SRAM4 and NE4 is used as ship select signal */
#define LCD_BASE    ((u32)(0x60000000 | 0x0C000000))
#define LCD         ((LCD_TypeDef *) LCD_BASE)

#define  ILI9320    0  /* 0x9320 */
#define  ILI9325    1  /* 0x9325 */
#define  ILI9328    2  /* 0x9328 */
#define  ILI9331    3  /* 0x9331 */
#define  SSD1298    4  /* 0x8999 */
#define  SSD1289    5  /* 0x8989 */
#define  ST7781     6  /* 0x7783 */
#define  LGDP4531   7  /* 0x4531 */
#define  SPFD5408B  8  /* 0x5408 */
#define  R61505U    9  /* 0x1505 0x0505 */	   
#define  HX8347D    10 /* 0x0047 */
#define  HX8347A    11 /* 0x0047 */	
#define  LGDP4535   12 /* 0x4535 */  
#define  SSD2119    13 /* 3.5 LCD 0x9919 */
////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* Function Name  : LCD_CtrlLinesConfig	 引脚IO配置
* Description    : Configures LCD Control lines (FSMC Pins) in alternate function
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG |
                         RCC_APB2Periph_AFIO, ENABLE);

  /* Set PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
     PD.10(D15), PD.14(D0), PD.15(D1) as alternate 
     function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11), PE.15(D12) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Set PF.00(A0 (RS)) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* Set PG.12(NE4 (LCD/CS)) as alternate function push pull - CE3(LCD /CS) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : //背光控制脚A1
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_LightPin(void)  
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_1);
}
/*******************************************************************************
* Function Name  : LCD_FSMCConfig	  FSMC配置
* Description    : Configures the Parallel interface (FSMC) for LCD(Parallel mode)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_FSMCConfig(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;

  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//使能FSMC时钟
	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到	
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
    

	writeTiming.FSMC_AddressSetupTime = 0x02;	 //0x01 地址建立时间（ADDSET）为1个HCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（A		
    writeTiming.FSMC_DataSetupTime = 0x05;		 ////0x03 数据保存时间为4个HCLK	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 

 
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //写时序

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置

   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // 使能BANK1
}


void LCD_X_Init(void)
{
  /*背光引脚初始化*/
  LCD_LightPin();

 /* Configure the LCD Control pins --------------------------------------------*/
  LCD_CtrlLinesConfig();

/* Configure the FSMC Parallel interface -------------------------------------*/
  LCD_FSMCConfig();
}


/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg,u16 LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
  LCD->LCD_REG = LCD_Reg;
  /* Write 16-bit Reg */
  LCD->LCD_RAM = LCD_RegValue;
}

/*******************************************************************************
* Function Name  : LCD_ReadReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
*******************************************************************************/
u16 LCD_ReadReg(u8 LCD_Reg)
{
  /* Write 16-bit Index (then Read Reg) */
  LCD->LCD_REG = LCD_Reg;
  /* Read 16-bit Reg */
  return (LCD->LCD_RAM);
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare
* Description    : Prepare to write to the LCD RAM.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
  LCD->LCD_REG = 0x0022;
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM
* Description    : Writes to the LCD RAM.
* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM(u16 RGB_Code)					 
{
  /* Write 16-bit GRAM Reg */
  LCD->LCD_RAM = RGB_Code;
}

/*******************************************************************************
* Function Name  : LCD_ReadRAM
* Description    : Reads the LCD RAM.
* Input          : None
* Output         : None
* Return         : LCD RAM Value.
*******************************************************************************/
u16 dummy;
u16 LCD_ReadRAM(void)
{
  
  /* Write 16-bit Index (then Read Reg) */
  LCD->LCD_REG = R34; /* Select GRAM Reg */
  /* Read 16-bit Reg */
  dummy = LCD->LCD_RAM; 
  return dummy;
}

void LCD_SetCursor( u16 Xpos, u16 Ypos )
{
    #if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )
	
 	u16 temp = Xpos;

			 Xpos = Ypos;
			 Ypos = ( MAX_X - 1 ) - temp;  

	#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )
		
	#endif

  switch( DeviceCode )
  {
     default:		 /* 0x9320 0x9325 0x9328 0x9331 0x5408 0x1505 0x0505 0x7783 0x4531 0x4535 */
          LCD_WriteReg(0x0020, Xpos );     
          LCD_WriteReg(0x0021, Ypos );     
	      break; 

     case SSD1298: 	 /* 0x8999 */
     case SSD1289:   /* 0x8989 */
	      LCD_WriteReg(0x004e, Xpos );      
          LCD_WriteReg(0x004f, Ypos );          
	      break;  

     case HX8347A: 	 /* 0x0047 */
     case HX8347D: 	 /* 0x0047 */
	      LCD_WriteReg(0x02, Xpos>>8 );                                                  
	      LCD_WriteReg(0x03, Xpos );  

	      LCD_WriteReg(0x06, Ypos>>8 );                           
	      LCD_WriteReg(0x07, Ypos );    
	
	      break;     
     case SSD2119:	 /* 3.5 LCD 0x9919 */
	      break; 
  }
}

void Delay(u32 nCount)
{
 u32 TimingDelay; 
 while(nCount--)
   {
    for(TimingDelay=0;TimingDelay<10000;TimingDelay++);
   }
}

/****************************************************************************
* 名    称：void ili9320_Initializtion()
* 功    能：初始化 LCD 控制器
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：ili9320_Initializtion();
****************************************************************************/
void ili9320_Initializtion()
{

    u16 i;
    LCD_X_Init();
    Delay(5); /* delay 50 ms */
    Delay(5); /* delay 50 ms */	

	DeviceCode = LCD_ReadReg(0x0000);   //读LCD驱动型号SSD1289
	if(DeviceCode==0x8989)
	{

		DeviceCode = 0x8989;
	    ili9320_Delay(5); /* ili9320_Delay 50 ms */
	    LCD_WriteReg(0x0000,0x0001);  
	    ili9320_Delay(5); /* ili9320_Delay 50 ms */			//start internal osc
	    
	    ili9320_Delay(5);
	
	    // power supply setting 
	    // set R07h at 0021h (GON=1,DTE=0,D[1:0]=01)
	    LCD_WriteReg(0x0007,0x0021);       
	    // set R00h at 0001h (OSCEN=1)       
	    LCD_WriteReg(0x0000,0x0001);       
	    // set R07h at 0023h (GON=1,DTE=0,D[1:0]=11)       
	    LCD_WriteReg(0x0007,0x0023);       
	    // set R10h at 0000h (Exit sleep mode)       
	    LCD_WriteReg(0x0010,0x0000);       
	    // Wait 30ms       
	    ili9320_Delay(30);       
	    // set R07h at 0033h (GON=1,DTE=1,D[1:0]=11)       
	    LCD_WriteReg(0x0007,0x0033);       
	    // Entry mode setting (R11h)       
	    // R11H Entry mode       
	    // vsmode DFM1 DFM0 TRANS OEDef WMode DMode1 DMode0 TY1 TY0 ID1 ID0 AM LG2 LG2 LG0       
	    //   0     1    1     0     0     0     0      0     0   1   1   0  0   0   0   0       
	    LCD_WriteReg(0x0011,0x6018);       
	    // LCD driver AC setting (R02h)       
	    LCD_WriteReg(0x0002,0x0600);       
	    // power control 1       
	    // DCT3 DCT2 DCT1 DCT0 BT2 BT1 BT0 0 DC3 DC2 DC1 DC0 AP2 AP1 AP0 0       
	    // 1     0    1    0    1   0   0  0  1   0   1   0   0   1   0  0       
	    // DCT[3:0] fosc/4 BT[2:0]  DC{3:0] fosc/4       
	    LCD_WriteReg(0x0003,0x0804);//0xA8A4  
	    LCD_WriteReg(0x000C,0x0000);//
	    LCD_WriteReg(0x000D,0x080C);//       
	    // power control 4       
	    // 0 0 VCOMG VDV4 VDV3 VDV2 VDV1 VDV0 0 0 0 0 0 0 0 0       
	    // 0 0   1    0    1    0    1    1   0 0 0 0 0 0 0 0       
	    LCD_WriteReg(0x000E,0x2900);       
	    LCD_WriteReg(0x001E,0x00B8);       
	    LCD_WriteReg(0x0001,0x2B3F);
	    LCD_WriteReg(0x0010,0x0000);       
	    LCD_WriteReg(0x0005,0x0000);       
	    LCD_WriteReg(0x0006,0x0000);       
	    LCD_WriteReg(0x0016,0xEF1C);       
	    LCD_WriteReg(0x0017,0x0003);       
	    LCD_WriteReg(0x0007,0x0233);//0x0233       
	    LCD_WriteReg(0x000B,0x0000|(3<<6));       
	    LCD_WriteReg(0x000F,0x0000);
	    LCD_WriteReg(0x0041,0x0000);       
	    LCD_WriteReg(0x0042,0x0000);       
	    LCD_WriteReg(0x0048,0x0000);       
	    LCD_WriteReg(0x0049,0x013F);       
	    LCD_WriteReg(0x004A,0x0000);       
	    LCD_WriteReg(0x004B,0x0000);       
	    LCD_WriteReg(0x0044,0xEF00);       
	    LCD_WriteReg(0x0045,0x0000);       
	    LCD_WriteReg(0x0046,0x013F);       
	    LCD_WriteReg(0x0030,0x0707);       
	    LCD_WriteReg(0x0031,0x0204);       
	    LCD_WriteReg(0x0032,0x0204);       
	    LCD_WriteReg(0x0033,0x0502);       
	    LCD_WriteReg(0x0034,0x0507);       
	    LCD_WriteReg(0x0035,0x0204);       
	    LCD_WriteReg(0x0036,0x0204);       
	    LCD_WriteReg(0x0037,0x0502);       
	    LCD_WriteReg(0x003A,0x0302);       
	    LCD_WriteReg(0x003B,0x0302);       
	    LCD_WriteReg(0x0023,0x0000);       
	    LCD_WriteReg(0x0024,0x0000);       
	    LCD_WriteReg(0x0025,0x8000);   // 65hz       
	    LCD_WriteReg(0x004f,0);        //       
	    LCD_WriteReg(0x004e,0);        //     
	      
	    ili9320_Delay(50);
	}
    for(i=50000;i>0;i--);
}

/****************************************************************************
* 名    称：void ili9320_SetCursor(u16 x,u16 y)
* 功    能：设置屏幕座标
* 入口参数：x      行座标
*           y      列座标
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetCursor(10,10);
****************************************************************************/
__inline void ili9320_SetCursor(u16 x,u16 y)
{
	if(DeviceCode==0x8989)
	{
	 	LCD_WriteReg(0x004e,y);        //行
    	LCD_WriteReg(0x004f,0x13f-x);  //列
	}
	else if(DeviceCode==0x9919)
	{
		LCD_WriteReg(0x004e,x);        //行
  		LCD_WriteReg(0x004f,y);        //列	
	}
	else
	{
  		LCD_WriteReg(0x0020,y);        //行
  		LCD_WriteReg(0x0021,0x13f-x);  //列
	}
}

/****************************************************************************
* 名    称：void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
* 功    能：设置窗口区域
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*           EndX       行结束座标
*           EndY       列结束座标
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetWindows(0,0,100,100)；
****************************************************************************/
__inline void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
{
  ili9320_SetCursor(StartX,StartY);
  LCD_WriteReg(0x0050, StartX);
  LCD_WriteReg(0x0052, StartY);
  LCD_WriteReg(0x0051, EndX);
  LCD_WriteReg(0x0053, EndY);
}

/****************************************************************************
* 名    称：void ili9320_Clear(u16 dat)
* 功    能：将屏幕填充成指定的颜色，如清屏，则填充 0xffff
* 入口参数：dat      填充值
* 出口参数：无
* 说    明：
* 调用方法：ili9320_Clear(0xffff);
****************************************************************************/
void ili9320_Clear(u16 Color)
{
  u32 index=0;
  ili9320_SetCursor(0,0); 
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  for(index=0;index<76800;index++)
   {
     LCD->LCD_RAM=Color;
   }
}

/****************************************************************************
* 名    称：u16 ili9320_GetPoint(u16 x,u16 y)
* 功    能：获取指定座标的颜色值
* 入口参数：x      行座标
*           y      列座标
* 出口参数：当前座标颜色值
* 说    明：
* 调用方法：i=ili9320_GetPoint(10,10);
****************************************************************************/
u16 ili9320_GetPoint(u16 x,u16 y)
{
  ili9320_SetCursor(x,y);
  return (ili9320_BGR2RGB(LCD_ReadRAM()));
}
/****************************************************************************
* 名    称：void ili9320_SetPoint(u16 x,u16 y,u16 point)
* 功    能：在指定座标画点
* 入口参数：x      行座标
*           y      列座标
*           point  点的颜色
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetPoint(10,10,0x0fe0);
****************************************************************************/
void ili9320_SetPoint(u16 x,u16 y,u16 point)
{
  if ( (x>320)||(y>240) ) return;
  ili9320_SetCursor(x,y);

  LCD_WriteRAM_Prepare();
  LCD_WriteRAM(point);
}

/****************************************************************************
* 名    称：void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* 功    能：在指定座标显示一个8x16点阵的ascii字符
* 入口参数：x          行座标
*           y          列座标
*           charColor  字符的颜色
*           bkColor    字符背景颜色
* 出口参数：无
* 说    明：显示范围限定为可显示的ascii码
* 调用方法：ili9320_PutChar(10,10,'a',0x0000,0xffff);
****************************************************************************/
void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
{
  u16 i=0;
  u16 j=0;
  
  u8 tmp_char=0;

  for (i=0;i<16;i++)
  {
    tmp_char=ascii_8x16[((c-0x20)*16)+i];
    for (j=0;j<8;j++)
    {
      if ( (tmp_char >> 7-j) & 0x01 == 0x01)
        {
          ili9320_SetPoint(x+j,y+i,charColor); // 字符颜色
        }
        else
        {
          ili9320_SetPoint(x+j,y+i,bkColor); // 背景颜色
        }
    }
  }
}

/****************************************************************************
* 名    称：u16 ili9320_BGR2RGB(u16 c)
* 功    能：RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
* 入口参数：c      BRG 颜色值
* 出口参数：RGB 颜色值
* 说    明：内部函数调用
* 调用方法：
****************************************************************************/
u16 ili9320_BGR2RGB(u16 c)
{
  u16  r, g, b, rgb;

  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;
  
  rgb =  (b<<11) + (g<<5) + (r<<0);

  return( rgb );
}

/****************************************************************************
* 名    称：void ili9320_BackLight(u8 status)
* 功    能：开、关液晶背光
* 入口参数：status     1:背光开  0:背光关
* 出口参数：无
* 说    明：
* 调用方法：ili9320_BackLight(1);
****************************************************************************/
void ili9320_BackLight(u8 status)
{
  if ( status >= 1 )
  {
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
  }
  else
  {
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
  }
}

/****************************************************************************
* 名    称：void ili9320_Delay(vu32 nCount)
* 功    能：延时
* 入口参数：nCount   延时值
* 出口参数：无
* 说    明：
* 调用方法：ili9320_Delay(10000);
****************************************************************************/
void ili9320_Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}
