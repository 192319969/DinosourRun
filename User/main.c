#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

extern int8_t height;
uint8_t category=0;
uint8_t CactusPos=0;
uint8_t CactusLength=0;

uint8_t score=0;
uint8_t highest=0;
uint8_t failed=0;

int main(void)
{
	OLED_Init();
	Key_Init();

	
	while(1)
	{
		if(failed==1)
		{
			OLED_ShowString(2,4,"Game Over!");
			Key_GetNum();
			if(KeyNum==2)
			{
				if(score>highest) highest=score;
				failed=0;
				score=0;
				height=0;
				OLED_DrawCactus(&category,&CactusPos,1);
				OLED_DrawDinoJump(1);
				OLED_Clear();
			}
		}
		else
		{
			score++;
			OLED_DrawCloud();
			OLED_DrawGround();

			OLED_DrawCactus(&category,&CactusPos,0);
			Key_GetNum();

			if(height>0||KeyNum==1) 
			{
				KeyNum=0;
				OLED_DrawDinoJump(0);
			}
			else OLED_DrawDino();
			CactusLength=8+category*8;

			if((height<16)&&((CactusPos>=8&&CactusPos<=24)||(CactusPos+CactusLength>=8&&CactusPos+CactusLength<=24)))
			{
				failed=1;
			}


			OLED_ShowString(1,1,"Score:");
			if(score>highest) highest=score;
			OLED_ShowNum(1,8,highest,4);
			OLED_ShowNum(1,13,score,4);
			Delay_ms(50);
		}


	}
}

