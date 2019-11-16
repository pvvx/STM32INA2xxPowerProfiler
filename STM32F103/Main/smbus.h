#ifndef _SMBus_H_
#define _SMBus_H_

#include <inttypes.h>

#define SMBus_NAME             I2C2
#define SMBus_RCC_Periph       RCC_APB1Periph_I2C2
#define SMBus_Port             GPIOB
#define SMBus_SCL_Pin          GPIO_Pin_10
#define SMBus_SDA_Pin          GPIO_Pin_11
#define SMBus_SCL_PinSource    GPIO_PinSource10
#define SMBus_SDA_PinSource    GPIO_PinSource11
#define SMBus_RCC_Port         RCC_APB2Periph_GPIOB
#define SMBus_Speed            10000000
#define SMBus_GPIO_AF          GPIO_AF_I2C2
#define SMBus_Max_Delay_Cycles 5000 // 10000

extern void SMBusInit(uint32_t speed);
extern uint8_t SMBusWriteWord(uint8_t slaveAddr, uint16_t data, uint8_t WriteAddr);
extern uint8_t SMBusReadWord(uint8_t slaveAddr, uint16_t* data, uint8_t ReadAddr);
extern uint8_t SMBusReadAlert(uint8_t *paddr);
//extern void SMBusSendHS(uint8_t code);

#endif
