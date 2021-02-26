#include "usart.h"	 
#include <string.h>

uint8_t usartRevCmdBuffer[USART_REV_BUFFER_SIZE] = {0};
uint8_t usartRevBuffer[USART_REV_BUFFER_SIZE] = {0};
uint8_t usartRevBufferIdx = 0;

uint8_t usartRevStartFlag = 0;

extern uint16_t usartRevTimeCnt;

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 0
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0) {}//ѭ������,ֱ���������   
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 

int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART2, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART2);
}
 

#if USART1_EN

u8 USART1_RX_BUF[USART1_RX_LEN];
u8 USART1_RX_STA = 0; //����״̬���,bit7��ɱ�־,bit6���յ�0x0d,bit5~0���յ�����Ч�ֽ���Ŀ

//���ڳ�ʼ��
void USART1_Init(u32 bound)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	 
	//ʱ�ӿ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	//GPIO���� TX(PA9),RX(PA10)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	//NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
  //��������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;   //USART ��ʼ������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//�����ж�
	USART_Cmd(USART1,ENABLE);                    //ʹ�ܴ��� 
}

void USART2_Init(u32 bound)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	 
	//ʱ�ӿ���
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	//GPIO���� TX(PA9),RX(PA10)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	//NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
  //��������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;   //USART ��ʼ������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStructure);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//�����ж�
	USART_Cmd(USART2,ENABLE);                    //ʹ�ܴ��� 
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

//����1�жϷ������
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
//	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		res = USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������	
//		if((USART1_RX_STA & 0x80) == 0)//����δ���
//		{
//			if(USART1_RX_STA & 0x40)//���յ���0x0d
//			{
//				if(res != 0x0a) USART1_RX_STA = 0;//���մ���,���¿�ʼ
//				else USART1_RX_STA |= 0x80;	//��������� 
//			}
//			else //��û�յ�0X0D
//			{	
//				if(res == 0x0d) USART1_RX_STA |= 0x40;
//				else
//				{
//					USART1_RX_BUF[USART1_RX_STA & 0X3f] = res ;
//					USART1_RX_STA++;
//					if(USART1_RX_STA > 63) USART1_RX_STA = 0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//		}   		 
//	} 
} 

#endif

//����һ���ֽ�����
void USART_SendByte(USART_TypeDef* USARTx,u8 Data)
{            
	USART_SendData(USARTx, Data);
	while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
}

//USARTx ����һ���ַ���
//len �ַ������ȣ�Ϊ0ʱ�Զ������ֽ���
void USART_SendStrN(USART_TypeDef* USARTx,u8 *Data,u16 len)
{
	if(len == 0) len = strlen((char*)Data);
	while(len--)		//�ж��Ƿ񵽴��ַ���
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

