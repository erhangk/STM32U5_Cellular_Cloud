/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    main.h
  * @author  GPM Application Team
  * @brief   Header for main.c file.
  *          This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32u5xx_hal_def.h"
#include "b_u585i_iot02a.h"  
#include "b_u585i_iot02a_env_sensors.h"
#include "b_u585i_iot02a_motion_sensors.h"
#if (USE_WIFI == 1)
#include "mx_wifi_conf.h"
#endif /* USE_WIFI == 1 */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern RNG_HandleTypeDef hrng;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define MXCHIP_SPI      hspi2
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void HAL_SPI_TransferCallback(void *hspi);

#if (MX_WIFI_USE_SPI == 1)
void mxchip_WIFI_ISR(uint16_t pin);
void nx_driver_emw3080_interrupt();
#endif /* MX_WIFI_USE_SPI */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SAMPLE_MAX_EXPONENTIAL_BACKOFF_IN_SEC 10
#define MXCHIP_FLOW_Pin GPIO_PIN_15
#define MXCHIP_FLOW_GPIO_Port GPIOG
#define MXCHIP_FLOW_EXTI_IRQn EXTI15_IRQn
#define MODEM_PWR_EN_Pin GPIO_PIN_4
#define MODEM_PWR_EN_GPIO_Port GPIOB
#define BUTTON_USER_Pin GPIO_PIN_13
#define BUTTON_USER_GPIO_Port GPIOC
#define BUTTON_USER_EXTI_IRQn EXTI13_IRQn
#define MODEM_RING_Pin GPIO_PIN_6
#define MODEM_RING_GPIO_Port GPIOE
#define MODEM_RING_EXTI_IRQn EXTI6_IRQn
#define MODEM_SIM_DATA_Pin GPIO_PIN_5
#define MODEM_SIM_DATA_GPIO_Port GPIOD
#define MODEM_SIM_SELECT_1_Pin GPIO_PIN_12
#define MODEM_SIM_SELECT_1_GPIO_Port GPIOC
#define MODEM_SIM_SELECT_0_Pin GPIO_PIN_2
#define MODEM_SIM_SELECT_0_GPIO_Port GPIOD
#define MODEM_DTR_Pin GPIO_PIN_9
#define MODEM_DTR_GPIO_Port GPIOC
#define MODEM_SIM_RST_Pin GPIO_PIN_1
#define MODEM_SIM_RST_GPIO_Port GPIOG
#define MODEM_SIM_CLK_Pin GPIO_PIN_0
#define MODEM_SIM_CLK_GPIO_Port GPIOG
#define MXCHIP_NOTIFY_Pin GPIO_PIN_14
#define MXCHIP_NOTIFY_GPIO_Port GPIOD
#define MXCHIP_NOTIFY_EXTI_IRQn EXTI14_IRQn
#define MODEM_UART_RTS_Pin GPIO_PIN_12
#define MODEM_UART_RTS_GPIO_Port GPIOD
#define MXCHIP_NSS_Pin GPIO_PIN_12
#define MXCHIP_NSS_GPIO_Port GPIOB
#define MODEM_UART_TX_Pin GPIO_PIN_8
#define MODEM_UART_TX_GPIO_Port GPIOD
#define MODEM_UART_RX_Pin GPIO_PIN_9
#define MODEM_UART_RX_GPIO_Port GPIOD
#define MODEM_UART_CTS_Pin GPIO_PIN_11
#define MODEM_UART_CTS_GPIO_Port GPIOD
#define MXCHIP_RESET_Pin GPIO_PIN_15
#define MXCHIP_RESET_GPIO_Port GPIOF
#define MODEM_RST_Pin GPIO_PIN_13
#define MODEM_RST_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* Environmental sensors instances */
#define HTS221_0        0
#define LPS22HH_0       1

/* Inertial sensors instances */
#define ISM330DHCX_0    0
#define IIS2MDC_0       1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
