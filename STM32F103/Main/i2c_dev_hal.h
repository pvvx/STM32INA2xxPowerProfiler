//DEVICE I2C library for ARM STM32F103xx 
#ifndef __HAL_DEVICE_H
#define __HAL_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include <stm32f10x.h>

/**
 * @addtogroup  DEVICE_I2C_Define
 * @{
 */

#define DEVICE_I2C                  I2C2
#define DEVICE_I2C_RCC_Periph       RCC_APB1Periph_I2C2
#define DEVICE_I2C_Port             GPIOB
#define DEVICE_I2C_SCL_Pin          GPIO_Pin_10
#define DEVICE_I2C_SDA_Pin          GPIO_Pin_11
#define DEVICE_I2C_RCC_Port         RCC_APB2Periph_GPIOB
#define DEVICE_I2C_Speed            100000
	
#define DEVICE_DRY_Port             GPIOB
#define DEVICE_DRY_Pin              GPIO_Pin_14
#define DEVICE_DRY_PortSource       GPIO_PortSourceGPIOB
#define DEVICE_DRY_PinSource        GPIO_PinSource14
#define DEVICE_DRY_EXTI_Line        EXTI_Line14
#define DEVICE_DRY_EXTI_IRQn        EXTI15_10_IRQn
#define DEVICE_DRY_IRQHandler       EXTI15_10_IRQHandler
	

/**
 *@}
 *//* end of group DEVICE_I2C_Define */

#ifdef __cplusplus
}
#endif

#endif /* __HAL___DEVICE_H */

