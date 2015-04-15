#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"

#define USART1_RX_LEN		256					//USART1���ջ����С
#define USART1_TX_LEN		800					//USART1���ͻ����С

#define USART2_RX_LEN		800					//USART2���ջ����С
#define USART2_TX_LEN		256					//USART2���ͻ����С


void Usart1_Init(void);				   				//USART1��ʼ����������
void Usart2_Init(void);								// USART2��ʼ��
void Usart_SendChar(USART_TypeDef* USARTx,char dat);		//USART�����ַ���������
char Usart_GetCahr(USART_TypeDef* USARTx);					//USART�����ַ���������
int Send (int ch);									//���ͺ���������stdio�ӿڣ�
int Get (void);										//���պ���������stdio�ӿڣ�

#endif
