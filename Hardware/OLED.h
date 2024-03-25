#ifndef __OLED_H
#define __OLED_H

extern int8_t height;

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void OLED_DrawGround(void);
void OLED_DrawCloud(void);
void OLED_DrawDino(void);
void OLED_DrawCactusTwo(void);
void OLED_DrawDinoJump(uint8_t reset);
void OLED_DrawCactus(uint8_t* pcnt,uint8_t* pCactusPos, uint8_t reset);

#endif
