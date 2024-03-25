#include "stm32f10x.h"                  // Device header

uint8_t KeyNum=0;

extern int8_t height;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

#if 0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);

	EXTI_InitTypeDef EXTI_InitStructure;						//定义结构体变量
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;					//选择配置外部中断的1号线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//指定外部中断线使能
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//指定外部中断线为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//指定外部中断线为下降沿触发
	EXTI_Init(&EXTI_InitStructure);

	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//配置NVIC为分组2
																//即抢占优先级范围：0~3，响应优先级范围：0~3
																//此分组配置在整个工程中仅需调用一次
																//若有多个中断，可以把此代码放在main函数内，while循环之前
																//若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//选择配置NVIC的EXTI1线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);	
#endif
	
}

#if 0
void EXTI1_IRQHandler (void)
{
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	{
		count++;
		EXTI_ClearITPendingBit(EXTI_Line1);
		if(height==0) KeyNum=1;
		else KeyNum=0;
	}
}
#endif

# if 1
void Key_GetNum(void)
{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
	{
		// Delay_ms(20);
		// while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);
		// Delay_ms(20);
		KeyNum=1;
	}

	else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
	{
		// Delay_ms(20);
		// while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0);
		// Delay_ms(20);
		KeyNum=2;
	}
	else KeyNum=0;
}
#endif
