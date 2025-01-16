/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define fel_led_Pin GPIO_PIN_0
#define fel_led_GPIO_Port GPIOA
#define le_led_Pin GPIO_PIN_1
#define le_led_GPIO_Port GPIOA
#define fel_Pin GPIO_PIN_2
#define fel_GPIO_Port GPIOA
#define le_Pin GPIO_PIN_3
#define le_GPIO_Port GPIOA
#define menu_Pin GPIO_PIN_4
#define menu_GPIO_Port GPIOA
#define vaszon_le_Pin GPIO_PIN_5
#define vaszon_le_GPIO_Port GPIOA
#define vaszon_fel_Pin GPIO_PIN_6
#define vaszon_fel_GPIO_Port GPIOA
#define motor_le_Pin GPIO_PIN_7
#define motor_le_GPIO_Port GPIOA
#define motor_fel_Pin GPIO_PIN_1
#define motor_fel_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
