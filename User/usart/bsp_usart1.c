#include "bsp_usart1.h"

void USART_RCC_Configuration(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
}
void USART_GPIO_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
}

void USART1_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_RCC_Configuration();
	USART_GPIO_Configuration();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  USART_InitStructure.USART_BaudRate = 460800;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;                                  //USART_Parity_Even;	USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure); 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  
	USART_Cmd(USART1,ENABLE);//
	
	printf("上海奇宝智能科技有限公司\r\n");
	printf("QBS-SY-ZK01湿压全自动码垛机器人\r\n");
}

//重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到USART1 */
	USART1->SR;
	USART_SendData(USART1, (uint8_t) ch);
	
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		

	return (ch);
}

//重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
/*********************************************END OF FILE**********************/
