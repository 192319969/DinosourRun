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

	EXTI_InitTypeDef EXTI_InitStructure;						//����ṹ�����
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;					//ѡ�������ⲿ�жϵ�1����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//ָ���ⲿ�ж���ʹ��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//ָ���ⲿ�ж���Ϊ�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//ָ���ⲿ�ж���Ϊ�½��ش���
	EXTI_Init(&EXTI_InitStructure);

	/*NVIC�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//����NVICΪ����2
																//����ռ���ȼ���Χ��0~3����Ӧ���ȼ���Χ��0~3
																//�˷������������������н������һ��
																//���ж���жϣ����԰Ѵ˴������main�����ڣ�whileѭ��֮ǰ
																//�����ö�����÷���Ĵ��룬���ִ�е����ûḲ����ִ�е�����
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;						//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ѡ������NVIC��EXTI1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
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
