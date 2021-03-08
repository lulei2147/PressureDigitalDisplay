#ifndef __USART_H_
#define __USART_H_

#include "sys.h"
#include "stdio.h"	

#define USART1_RX_LEN		64					//�����ջ����ֽ���
#define USART1_EN 			1					  //0,������;1,����.

#define USART_REV_BUFFER_SIZE                    (128)

extern u8	USART1_RX_BUF[USART1_RX_LEN];     //���ջ���,���USART1_RX_LEN�ֽ�
extern u8	USART1_RX_STA;         //����״̬���	

void USART1_Init(u32 bound);	//����1����������
void USART2_Init(u32 bound);
void USART_SendByte(USART_TypeDef* USARTx,u8 Data);
void USART_SendStrN(USART_TypeDef* USARTx, u8 *Data, u16 len);
void Usart_WriteData(uint8_t *pData, uint8_t len);

extern uint8_t usartRevCmdBuffer[USART_REV_BUFFER_SIZE];
extern uint8_t usartRevBuffer[USART_REV_BUFFER_SIZE];

void USART_UpdateCmdBuffer(void);

#endif

