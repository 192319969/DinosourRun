#include "stm32f10x.h"
#include "OLED_Font.h"
#include "OLED_Game_Pic.h"
#include <stdlib.h>

int8_t height=0;

extern int8_t KeyNum;

/*引脚配置*/
#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x))

/*引脚初始化*/
void OLED_I2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

/**
  * @brief  I2C停止
  * @param  无
  * @retval 无
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
	OLED_W_SCL(0);
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x00);		//写命令
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x40);		//写数据
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/**
  * @brief  OLED显示地面
  * @param  无
  * @retval 无
  */
void OLED_DrawGround(void)
{
	static uint16_t GroundPos= 0;
	const uint16_t GroundLength = 1196;

	const uint8_t speed = 5;
	uint16_t i;
	OLED_SetCursor(7,0);
	for(i=0;i<128;i++)
	{
		OLED_WriteData(OLED_Ground[(GroundPos+i)%GroundLength]);
	}

	GroundPos=GroundPos+speed;
	// OLED_ShowString(1,1,"pos=");
	// OLED_ShowNum(1,5,pos,4);
	// OLED_ShowString(2,1,"GroundLen=");
	// OLED_ShowNum(2,11,GroundLength,4);

	if(GroundPos>GroundLength) GroundPos=0;
}

/**
  * @brief  OLED show cloud
  * @param  none
  * @retval none
  */
void OLED_DrawCloud(void)
{
	static int16_t CloudPos= 128;
	static uint8_t height=0;
	const int16_t CloudLength = 24;
	int16_t start=0;
	int16_t cursor_x=0;
	int16_t i=0;


	const int8_t speed = 3;

	if(CloudPos+CloudLength<=-speed)
	{
		CloudPos=128;
		height=rand()%3;
	}

	if(CloudPos<0)
	{
		start=-CloudPos;
		cursor_x=0;
	}
	else
	{
		start=0;
		cursor_x=CloudPos;
	}

	OLED_SetCursor(1+height,cursor_x);
	for(i=start;i<CloudLength+speed;i++)
	{
		if(CloudPos+i>127) break;
		if(i<CloudLength) OLED_WriteData(OLED_Cloud[i]);
		else OLED_WriteData(0x00);
	}

	CloudPos-=speed;
}


/**
  * @brief  OLED show dino
  * @param  无
  * @retval 无
  */
void OLED_DrawDino(void)
{
	static uint8_t dir=0;
	// static uint16_t DinoPos= 0;
	// const uint16_t DinoLength = 32;

	dir++;
	dir%=2;
	uint16_t i;

	if(dir==1)
	{
		OLED_SetCursor(6,10);
		for(i=0;i<16;i++)
		{
			OLED_WriteData(OLED_Dino[0][i*2]);
		}
		OLED_SetCursor(7,10);
		for(i=0;i<11;i++)
		{
			OLED_WriteData(OLED_Dino[0][i*2+1]);
		}
	}
	else
	{
		OLED_SetCursor(6,10);
		for(i=0;i<16;i++)
		{
			OLED_WriteData(OLED_Dino[1][i*2]);
		}
		OLED_SetCursor(7,10);
		for(i=0;i<11;i++)
		{
			OLED_WriteData(OLED_Dino[1][i*2+1]);
		}
	}
}

void OLED_DrawCactus(uint8_t* pcnt,uint8_t* pCactusPos, uint8_t reset)
{
	static int16_t CactusPos= 128;
	static uint8_t cnt=1;
	static int8_t CactusLength;

	static const int8_t speed = 6;

	int16_t start=0;
	int16_t cursor_x=0;
	int16_t i=0;

	if(reset)
	{
		CactusPos= 128;
		cnt=1;
		CactusLength=0;
		return;
	}

	CactusLength=8+cnt*8;


	if(CactusPos+CactusLength<=-speed)
	{
		CactusPos=128;
		cnt=rand()%4;
		CactusLength=8+cnt*8;
	}

	if(CactusPos<0)
	{
		start=-CactusPos;
		cursor_x=0;
	}
	else
	{
		start=0;
		cursor_x=CactusPos;
	}

	OLED_SetCursor(6,cursor_x);
	for(i=start;i<CactusLength+speed;i++)
	{
		if(CactusPos+i>127) break;
		if(i<CactusLength)
		{
			if(cnt==0) OLED_WriteData(OLED_CactusOne[2*i]);
			else if(cnt==1) OLED_WriteData(OLED_CactusTwo[2*i]);
			else if(cnt==2) OLED_WriteData(OLED_CactusThree[2*i]);
			else OLED_WriteData(OLED_CactusFour[2*i]);
		}
		else OLED_WriteData(0x00);
	}
	OLED_SetCursor(7,cursor_x);
	for(i=start;i<CactusLength+speed;i++)
	{
		if(CactusPos+i>127) break;
		if(i<CactusLength)
		{
			if(cnt==0) OLED_WriteData(OLED_CactusOne[2*i+1]);
			else if(cnt==1) OLED_WriteData(OLED_CactusTwo[2*i+1]);
			else if(cnt==2) OLED_WriteData(OLED_CactusThree[2*i+1]);
			else OLED_WriteData(OLED_CactusFour[2*i+1]);
		}
		else OLED_WriteData(0x00);
	}

	CactusPos-=speed;
	*pcnt=cnt;
	*pCactusPos=CactusPos;
}

void OLED_DrawCactusTwo(void)
{
	static int16_t CactusPos= 128;
	const int16_t CactusLength = 16;
	int16_t start=0;
	int16_t cursor_x=0;
	int16_t i=0;

	const int8_t speed = 3;

	if(CactusPos+CactusLength<=-speed) CactusPos=128;

	if(CactusPos<0)
	{
		start=-CactusPos;
		cursor_x=0;
	}
	else
	{
		start=0;
		cursor_x=CactusPos;
	}


	OLED_SetCursor(6,cursor_x);
	for(i=start;i<CactusLength+speed;i++)
	{
		if(CactusPos+i>127) break;
		if(i<16) OLED_WriteData(OLED_CactusTwo[2*i]);
		else OLED_WriteData(0x00);
	}
	OLED_SetCursor(7,cursor_x);
	for(i=start;i<CactusLength+speed;i++)
	{
		if(CactusPos+i>127) break;
		if(i<16) OLED_WriteData(OLED_CactusTwo[2*i+1]);
		else OLED_WriteData(0x00);
	}

	CactusPos-=speed;
}

#if 0
void OLED_DrawDinoJumpTest(void)
{
	uint8_t y,i,k;
	for(k=0;k<8;k++)
	{
		for(y=5;y<8;y++)
		{
			OLED_SetCursor(y,k*16);
			for(i=0;i<16;i++)
			{
				OLED_WriteData(OLED_DinoJump[k][i*3+y-5]);
			}
		}
	}
}
#endif

void OLED_DrawBlank(uint8_t height)
{
	uint8_t i;
	OLED_SetCursor(7-(height/8),10);
	for(i=0;i<16;i++)
	{
		OLED_WriteData(0x00);
	}
}

void OLED_DrawDinoJump(uint8_t reset)
{
	uint8_t speed_arr[]={1,1,3,3,4,4,5,6,7};
	static int8_t speed_idx=8;
	static uint8_t dir=0;

	uint8_t i=0;
	uint8_t y;
	uint8_t offset=0;
	if(reset)
	{
		speed_idx= 8;
		height=0;
		dir=0;

		return;
	}

	// OLED_ShowString(1,1,"dir=");
	// OLED_ShowNum(1,5,dir,1);
	// OLED_ShowString(1,7,"h=");
	// OLED_ShowNum(1,9,height,2);

	if(dir==0)
	{
		height+=speed_arr[speed_idx];
		speed_idx--;
		if(speed_idx<0) speed_idx=0;
	}
	else
	{
		height-=speed_arr[speed_idx];
		speed_idx++;
		if(speed_idx>8) speed_idx=8;
	}
	if(height>31)
	{
		height=31;
		dir=1;
	}
	if(height<=0)
	{
		KeyNum=0;
		height=0;
		dir=0;
	}
	if(height<=7) offset=0;
	else if(height<=15) offset=1;
	else if(height<=23) offset=2;
	else if(height<=31) offset=3;
	else offset=4;

	for(y=5;y<8;y++)
	{
		OLED_SetCursor(y-offset,10);
		for(i=0;i<16;i++)
		{
			OLED_WriteData(OLED_DinoJump[(height/4)][i*3+y-5]);
		}
	}
	if(dir==0) 
	{
		if(height>7)
			OLED_DrawBlank(height);
	}
	else OLED_DrawBlank(height+23);
}




/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//端口初始化
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}
