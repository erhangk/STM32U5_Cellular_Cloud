/**
  ******************************************************************************
  * @file    app_config.h
  * @author  GPM Application Team
  * @brief   Manage application configuration in secure storage
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the ST_LICENSE file
  * in the root directory of this software component.
  * If no ST_LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
  
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
  
#define CONFIG_SUCCESS 0
#define CONFIG_ERROR   (!CONFIG_SUCCESS)

/* Exported functions --------------------------------------------------------*/
uint32_t config_init(void);
char     config_display_menu(void);
uint32_t config_process_command(char command);
uint32_t config_get_data(void);


#ifdef __cplusplus
}
#endif

#endif /* APP_CONFIG_H */

