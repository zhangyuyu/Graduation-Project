#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"

#define USART1_RX_LEN		256					//USART1接收缓冲大小
#define USART1_TX_LEN		800					//USART1发送缓冲大小

#define USART2_RX_LEN		800					//USART2接收缓冲大小
#define USART2_TX_LEN		256					//USART2发送缓冲大小


void Usart1_Init(void);				   				//USART1初始化函数声明
void Usart2_Init(void);								// USART2初始化
void Usart_SendChar(USART_TypeDef* USARTx,char dat);		//USART发送字符函数声明
char Usart_GetCahr(USART_TypeDef* USARTx);					//USART接收字符函数声明
int Send (int ch);									//发送函数声明（stdio接口）
int Get (void);										//接收函数声明（stdio接口）

#endif
