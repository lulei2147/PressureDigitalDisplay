#include "usart.h"	 
#include <string.h>

uint8_t usartRevCmdBuffer[USART_REV_BUFFER_SIZE] = {0};
uint8_t usartRevBuffer[USART_REV_BUFFER_SIZE] = {0};
uint8_t usartRevBufferIdx = 0;

uint8_t usartRevStartFlag = 0;

extern uint16_t usartRevTimeCnt;

//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 0
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0) {}//循环发送,直到发送完毕   
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 

int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(USART2, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART2);
}
 

#if USART1_EN

u8 USART1_RX_BUF[USART1_RX_LEN];
u8 USART1_RX_STA = 0; //接收状态标记,bit7完成标志,bit6接收到0x0d,bit5~0接收到的有效字节数目

//串口初始化
void USART1_Init(u32 bound)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	 
	//时钟开启
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	//GPIO配置 TX(PA9),RX(PA10)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	//NVIC配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
  //串口配置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;   //USART 初始化设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启中断
	USART_Cmd(USART1,ENABLE);                    //使能串口 
}

void USART2_Init(u32 bound)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	 
	//时钟开启
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	//GPIO配置 TX(PA9),RX(PA10)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	//NVIC配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
  //串口配置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;   //USART 初始化设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStructure);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启中断
	USART_Cmd(USART2,ENABLE);                    //使能串口 
}

void USART_UpdateCmdBuffer(void)
{
	if((usartRevBuffer[0] != 0x00) && (usartRevStartFlag == 0))
	{
		memset(usartRevCmdBuffer, 0, USART_REV_BUFFER_SIZE);
		memcpy(usartRevCmdBuffer, usartRevBuffer, usartRevBufferIdx);
		memset(usartRevBuffer, 0, USART_REV_BUFFER_SIZE);
		
		usartRevBufferIdx = 0;
	}
}

//串口1中断服务程序
void USART2_IRQHandler(void) 
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		usartRevTimeCnt = 0;
		
		if(usartRevStartFlag == 0)
		{
			usartRevStartFlag = 1;
		}
		
		usartRevBuffer[usartRevBufferIdx] = USART_ReceiveData(USART2);
		usartRevBufferIdx++;
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
	
//	u8 res;
//	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//	{
//		res = USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据	
//		if((USART1_RX_STA & 0x80) == 0)//接收未完成
//		{
//			if(USART1_RX_STA & 0x40)//接收到了0x0d
//			{
//				if(res != 0x0a) USART1_RX_STA = 0;//接收错误,重新开始
//				else USART1_RX_STA |= 0x80;	//接收完成了 
//			}
//			else //还没收到0X0D
//			{	
//				if(res == 0x0d) USART1_RX_STA |= 0x40;
//				else
//				{
//					USART1_RX_BUF[USART1_RX_STA & 0X3f] = res ;
//					USART1_RX_STA++;
//					if(USART1_RX_STA > 63) USART1_RX_STA = 0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//		}   		 
//	} 
} 

#endif

//发送一个字节数据
void USART_SendByte(USART_TypeDef* USARTx,u8 Data)
{            
	USART_SendData(USARTx, Data);
	while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
}

//USARTx 发送一个字符串
//len 字符串长度，为0时自动计算字节数
void USART_SendStrN(USART_TypeDef* USARTx,u8 *Data,u16 len)
{
	if(len == 0) len = strlen((char*)Data);
	while(len--)		//判断是否到达字符数
	{				                          
		USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}

void Usart_WriteData(uint8_t *pData, uint8_t len)
{
	uint8_t i = 0;
	uint8_t *data = pData;
	
	for(i = 0; i < len; i++)
	{
		USART_SendByte(USART2, *(data + i));
	}
}

