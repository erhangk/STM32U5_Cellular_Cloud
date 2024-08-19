/**
  ******************************************************************************
  * @file    stdout_usart.c 
  * @author  GPM Application Team
  * @brief   This file implements stdout USART channel
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
int fputc(int ch, FILE *f);
int _write(int file, char *ptr, int len);
#endif /* __CC_ARM */

extern UART_HandleTypeDef STDOUT_UART_HANDLER;

#ifdef __ICCARM__
int __write(int file, char *ptr, int len)
#else
int _write(int file, char *ptr, int len)
#endif
{
  HAL_UART_Transmit(&STDOUT_UART_HANDLER,(uint8_t *)ptr, len, 0xFFFFFFFF);
  
  return len;
}

#ifdef __CC_ARM
int fputc(int ch, FILE *f)
{
  _write(0, (char *)&ch, 1);
  
  return ch;
}
#endif

