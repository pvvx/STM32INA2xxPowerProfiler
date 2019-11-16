/*
  ============================================================================================
  HMC5883L device SMBus library code for ARM STM32F103xx is placed under the MIT license
  Copyright (c) 2012 Harinadha Reddy Chintalapalli

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  ================================================================================================
*/

#include <stm32f10x_i2c.h>

#include "smbus.h"

void SMBusInit(uint32_t speed)
{
    I2C_InitTypeDef SMBus_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable I2C and GPIO clocks */
    RCC_APB1PeriphClockCmd(SMBus_RCC_Periph, ENABLE);
    RCC_APB2PeriphClockCmd(SMBus_RCC_Port, ENABLE);

    /* Configure SMBus pins: SCL and SDA */
    GPIO_InitStructure.GPIO_Pin = SMBus_SCL_Pin | SMBus_SDA_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(SMBus_Port, &GPIO_InitStructure);

//    GPIO_PinAFConfig(SMBus_Port, SMBus_SCL_PinSource, SMBus_GPIO_AF);
//    GPIO_PinAFConfig(SMBus_Port, SMBus_SDA_PinSource, SMBus_GPIO_AF);

    /* SMBus configuration */
    SMBus_InitStructure.I2C_Mode = I2C_Mode_SMBusHost;
    SMBus_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    SMBus_InitStructure.I2C_OwnAddress1 = 0x01;
    SMBus_InitStructure.I2C_Ack = I2C_Ack_Enable;
    SMBus_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    SMBus_InitStructure.I2C_ClockSpeed = speed;

    /* Apply SMBus configuration after enabling it */
    I2C_Init(SMBus_NAME, &SMBus_InitStructure);

    I2C_Cmd(SMBus_NAME, ENABLE);
}


/**
 * @brief  Writes one byte to the peripheral device.
 * @param  slaveAddr : slave address.
 * @param  pBuffer : pointer to the buffer  containing the data to be written to the peripheral device.
 * @param  WriteAddr : address of the register in which the data will be written.
 * @retval None
 */
uint8_t SMBusWriteWord(uint8_t slaveAddr, uint16_t data, uint8_t WriteAddr)
{
	uint8_t ret = 0;
	do {
		uint32_t counter = SMBus_Max_Delay_Cycles;
		while (I2C_GetFlagStatus(SMBus_NAME, I2C_FLAG_BUSY) && counter) --counter;
		if(counter == 0) break;

		I2C_GenerateSTART(SMBus_NAME, ENABLE);
		counter = SMBus_Max_Delay_Cycles;
		while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_MODE_SELECT) && counter) --counter;
		if(counter == 0) break;

		I2C_Send7bitAddress(SMBus_NAME, slaveAddr, I2C_Direction_Transmitter);
		counter = SMBus_Max_Delay_Cycles;
		while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && counter) --counter;
		if(counter == 0) break;

		I2C_SendData(SMBus_NAME, WriteAddr);
		counter = SMBus_Max_Delay_Cycles;
		while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && counter) --counter;
		if(counter == 0) break;

		I2C_SendData(SMBus_NAME, data >> 8);
		counter = SMBus_Max_Delay_Cycles;
		while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && counter) --counter;
		if(counter == 0) break;

		I2C_SendData(SMBus_NAME, (data & 0xFF));
		I2C_GenerateSTOP(SMBus_NAME, ENABLE);
		ret = 1;
    } while(0);
	return ret;
}

/**
 * @brief  Reads a block of data from the peripheral device.
 * @param  slaveAddr  : slave address.
 * @param  pBuffer : pointer to the buffer that receives the data read from the peripheral device.
 * @param  ReadAddr : Peripheral device's internal address to read from.
 * @param  NumByteToRead : number of bytes to read from the peripheral device.
 * @retval None
 */
uint8_t SMBusReadWord(uint8_t slaveAddr, uint16_t* data, uint8_t ReadAddr)
{
	union {
		uint8_t uc[2];
		uint16_t ui;
	} buff;
	uint8_t ret = 0;
	uint32_t counter = SMBus_Max_Delay_Cycles;
	do {
		while (I2C_GetFlagStatus(SMBus_NAME, I2C_FLAG_BUSY) && counter) --counter;
		if(counter == 0) break;

		I2C_GenerateSTART(SMBus_NAME, ENABLE);

		counter = SMBus_Max_Delay_Cycles;
		while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_MODE_SELECT) && counter) --counter;
		if(counter == 0) break;
		
		I2C_Send7bitAddress(SMBus_NAME, slaveAddr, I2C_Direction_Transmitter);
	
		counter = SMBus_Max_Delay_Cycles;
		while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && counter) --counter;
		if(counter == 0) break;

		I2C_Cmd(SMBus_NAME, ENABLE);
		I2C_SendData(SMBus_NAME, ReadAddr);

		counter = SMBus_Max_Delay_Cycles;
		while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && counter) --counter;
		if(counter == 0) break;

		I2C_GenerateSTART(SMBus_NAME, ENABLE);

		counter = SMBus_Max_Delay_Cycles;
		while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_MODE_SELECT) && counter) --counter;
		if(counter == 0) break;

		I2C_Send7bitAddress(SMBus_NAME, slaveAddr, I2C_Direction_Receiver);

		counter = SMBus_Max_Delay_Cycles;
		while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && counter) --counter;
		if(counter == 0) break;

		counter = SMBus_Max_Delay_Cycles;
		while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_BYTE_RECEIVED) && counter) --counter;
		if(counter == 0) break;
		
		buff.uc[1] = I2C_ReceiveData(SMBus_NAME);
		
		I2C_AcknowledgeConfig(SMBus_NAME, DISABLE);
		I2C_GenerateSTOP(SMBus_NAME, ENABLE);
		
		counter = SMBus_Max_Delay_Cycles;
		while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_BYTE_RECEIVED) && counter) --counter;

		I2C_AcknowledgeConfig(SMBus_NAME, ENABLE);

		if(counter == 0) break;

		buff.uc[0] = I2C_ReceiveData(SMBus_NAME);

		*data = buff.ui;
		ret = 1;
	} while(0);
	return ret;
}

// Get SMBUS dev Alert addr
uint8_t SMBusReadAlert(uint8_t *paddr)
{
	uint32_t counter = SMBus_Max_Delay_Cycles;
	uint8_t ret = 0;
	do {
		while (I2C_GetFlagStatus(SMBus_NAME, I2C_FLAG_BUSY) && counter) --counter;
		if(counter == 0) break;

		I2C_GenerateSTART(SMBus_NAME, ENABLE);

		counter = SMBus_Max_Delay_Cycles;
		while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_MODE_SELECT) && counter) --counter;
		if(counter == 0) break;

		I2C_Send7bitAddress(SMBus_NAME, 0x18, I2C_Direction_Receiver);

		counter = SMBus_Max_Delay_Cycles;
		while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && counter) --counter;
		if(counter == 0) break;

		I2C_AcknowledgeConfig(SMBus_NAME, DISABLE);
		I2C_GenerateSTOP(SMBus_NAME, ENABLE);
		
		counter = SMBus_Max_Delay_Cycles;
		while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_BYTE_RECEIVED) && counter) --counter;

		I2C_AcknowledgeConfig(SMBus_NAME, ENABLE);

		if(counter == 0) break;

		*paddr = I2C_ReceiveData(SMBus_NAME);
		ret = 1;
	} while(0);
	return ret;
}

/*
// Send high-speed (HS) ?
void SMBusSendHS(uint8_t code)
{
	uint32_t counter = SMBus_Max_Delay_Cycles;
	I2C_GenerateSTART(SMBus_NAME, ENABLE);
	while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_MODE_SELECT) && counter) --counter;
	if(counter == 0) return;
	
	I2C_AcknowledgeConfig(SMBus_NAME, DISABLE);
    I2C_SendData(SMBus_NAME, code);

//    counter = SMBus_Max_Delay_Cycles;
//    while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_BYTE_TRANSMITTING) && counter) -- counter;

    I2C_GenerateSTOP(SMBus_NAME, ENABLE);		
	I2C_AcknowledgeConfig(SMBus_NAME, ENABLE);
	while (!I2C_CheckEvent(SMBus_NAME, I2C_EVENT_MASTER_MODE_SELECT) && counter) --counter;
}
*/
