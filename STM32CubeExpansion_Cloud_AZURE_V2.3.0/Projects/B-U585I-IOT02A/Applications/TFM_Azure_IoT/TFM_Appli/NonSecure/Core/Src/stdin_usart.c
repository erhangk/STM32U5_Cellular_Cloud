/**
  ******************************************************************************
  * @file    stdin_uart.c
  * @author  GPM Application Team
  * @brief   This file implements stdin USART channel
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the ST_LICENSE file
  * in the root directory of this software component.
  * If no ST_LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "main.h"
#include <stdio.h>

#ifndef __ICCARM__ /* If not IAR */
int fgetc(FILE * stream);
int _read(int file, char *ptr, int len);
#endif

extern UART_HandleTypeDef STDIN_UART_HANDLER;

void stdin_init(void);

void stdin_init(void)
{
  /* This was intentionally left empty */
}

#ifdef __ICCARM__
int __read(int file, char *ptr, int len)
#else
int _read(int file, char *ptr, int len)
#endif
{
  int length = 0;
  
  int ch = 0;  
  do
  {
    HAL_UART_Receive(&STDIN_UART_HANDLER, (uint8_t *)&ch, 1, 0xFFFFFFFF);
    
    *ptr = ch;
    ptr++;
    length++;
  }while((length < len) && (*(ptr-1) != '\r'));
  
  return length;
}

#ifdef __CC_ARM /* Keil Compiler */
int fgetc(FILE * stream)
{
  char ch;
  _read(0, &ch, 1);
  
  return (int)ch;
}
#endif

