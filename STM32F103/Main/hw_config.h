/**
  ******************************************************************************
  * @file    hw_config.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Hardware Configuration & Setup
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "usb_type.h"
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/

#define BULK_MAX_PACKET_SIZE  0x00000040

/*Unique Devices IDs register set (STM32F10x)*/

#define         ID1          (0x1FFFF7E8)
#define         ID2          (0x1FFFF7EC)
#define         ID3          (0x1FFFF7F0)

/* Define the STM32F10x hardware depending on the used evaluation board */
#if defined (USE_STM8S_D_STLINK)
/*
  #define USB_DISCONNECT                      GPIOA
  #define USB_DISCONNECT_PIN                  GPIO_Pin_8
  #define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOA
*/
/*
  #define GPIO_LED                            GPIOA
  #define GPIO_LED_PIN                        GPIO_Pin_8
  #define RCC_APB2Periph_GPIO_LED             RCC_APB2Periph_GPIOA
*/
#elif defined (USE_STLINK_CHINA)

  #define GPIO_LED                            GPIOA
  #define GPIO_LED_PIN                        GPIO_Pin_9
  #define RCC_APB2Periph_GPIO_LED             RCC_APB2Periph_GPIOA

#elif defined (USE_STM32F4_D_STLINK)
/*
  #define USB_DISCONNECT                      GPIOA
  #define USB_DISCONNECT_PIN                  GPIO_Pin_9
  #define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOA
*/
  #define GPIO_LED                            GPIOA
  #define GPIO_LED_PIN                        GPIO_Pin_9
  #define RCC_APB2Periph_GPIO_LED             RCC_APB2Periph_GPIOA

#elif defined (USE_STM32_MINI)
#if 1
  #define USB_DISCONNECT                      GPIOA
  #define USB_DISCONNECT_PIN                  GPIO_Pin_9
  #define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOA
#endif
  #define GPIO_LED                            GPIOC
  #define GPIO_LED_PIN                        GPIO_Pin_13
  #define RCC_APB2Periph_GPIO_LED             RCC_APB2Periph_GPIOC

#elif defined (USE_STM32_ELECTRON)
#if 0
  #define USB_DISCONNECT                      GPIOA
  #define USB_DISCONNECT_PIN                  GPIO_Pin_9
  #define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOA
#endif
 
  #define GPIO_LED                            GPIOA
  #define GPIO_LED_PIN                        GPIO_Pin_1
  #define RCC_APB2Periph_GPIO_LED             RCC_APB2Periph_GPIOA

#else
 #error "Missing define: USE_BOARD"
#endif

/* Exported functions ------------------------------------------------------- */
void Set_System(void);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);
void Get_SerialNum(void);
void Delay_ms(uint32_t Delay);
extern volatile uint32_t uwTick_ms;
uint32_t CDC_Send_DATA (uint8_t *ptrBuffer, uint8_t Send_length);
uint32_t CDC_Receive_DATA(void);
/* External variables --------------------------------------------------------*/

#endif  /*__HW_CONFIG_H*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
