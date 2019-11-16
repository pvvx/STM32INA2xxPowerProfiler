/**
 ******************************************************************************
 * @file	 main.c
 * @author	 MCD Application Team
 * @version V4.0.0
 * @date	 21-January-2013
 * @brief	 Virtual Com Port Demo main file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *		   http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

#include "rw_ini.h"
#include "smbus.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct {
	uint8_t dev_addr; //
	uint8_t reg_addr; //
} reg_rd_t;

typedef struct {
	uint8_t dev_addr; //
	uint8_t reg_addr; //
	uint16_t data; //
} reg_wr_t;

#define MAX_INIT_REGS 4
#define MAX_READ_REGS 4
//#define wr_data_max ((VIRTUAL_COM_PORT_DATA_SIZE-sizeof(blk_head_t))/sizeof(int16_t))

typedef struct {
	uint8_t rd_count; // 1
	uint8_t init_count; // 1
	uint16_t time; // 2
	uint16_t clk_khz; // 2
	reg_wr_t init[MAX_INIT_REGS]; // (1+1+2)*4
	reg_rd_t rd[MAX_READ_REGS]; // (1+1)*4
} dev_cfg_t; // 6+16+8 [30]

typedef struct {
	uint16_t id; // = 0x5555
	uint8_t cmd;	 // command
	uint8_t size;	 // data size
} blk_head_t;

typedef struct {
	blk_head_t head;
	union {
		uint8_t uc[VIRTUAL_COM_PORT_DATA_SIZE-sizeof(blk_head_t)];
		int8_t sc[VIRTUAL_COM_PORT_DATA_SIZE-sizeof(blk_head_t)];
		uint16_t ui[(VIRTUAL_COM_PORT_DATA_SIZE-sizeof(blk_head_t))/sizeof(uint16_t)];
		int16_t si[(VIRTUAL_COM_PORT_DATA_SIZE-sizeof(blk_head_t))/sizeof(int16_t)];
		uint32_t ud[(VIRTUAL_COM_PORT_DATA_SIZE-sizeof(blk_head_t))/sizeof(uint32_t)];
		int32_t sd[(VIRTUAL_COM_PORT_DATA_SIZE-sizeof(blk_head_t))/sizeof(uint32_t)];
		reg_wr_t reg;
	} data;
} blk_cio_t;

/* Private define ------------------------------------------------------------*/
#define Timer_SetPreriod_us(x)	TIM2->ARR = x-1 /* Set the Autoreload value */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
dev_cfg_t cfg_ini;
blk_cio_t Send_Buffer1;
blk_cio_t Send_BufferO;
uint32_t not_send_count;
uint32_t all_send_count;
//volatile uint32_t timer_count;

volatile uint8_t Send_Flg;
volatile uint8_t timer_flg;
uint8_t rd_next_cnt;
uint8_t wr_data_cnt;
uint8_t wr_data_max;
uint8_t send_err = 0;

volatile uint32_t packet_sent = 1;
volatile uint32_t packet_receive = 1;

/* Extern variables ----------------------------------------------------------*/
extern __IO uint8_t Receive_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];
extern __IO uint32_t Receive_length;

void *memcpy2(void *dest, void *src, uint16_t len) {
	uint8_t *dest1 = dest;
	uint8_t *src1 = (uint8_t *) src;
	while (len--)
		*dest1++ = *src1++;
	return dest;
}

/* Code ----------------------------------------------------------------------*/

/*******************************************************************************
 * Function Name : Timer_Init.
 * Descriptioan  : Инициализация таймера с периодом 100 us.
 * Input		 : None.
 * Output		 : None.
 * Return		 : None.
 *******************************************************************************/
void Timer_Init(uint16_t period_us) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the TIM2 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = period_us - 1;// 1 MHz down to 10 KHz (100 us)
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 72 MHz Clock down to 1 MHz (adjust per your clock)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* TIM IT enable */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
//	timer_count = 0;
}
/*******************************************************************************
 * Function Name : GetNewRegData.
 * Descriptioan	 : Наполнение буферов передачи данных.
 * Input		 : None.
 * Output		 : None.
 * Return		 : None.
 *******************************************************************************/
void GetNewRegData(void) {
	reg_rd_t *raddr;
	blk_cio_t *p = &Send_Buffer1;
	if (rd_next_cnt >= cfg_ini.rd_count)
		rd_next_cnt = 0;
	raddr = &cfg_ini.rd[rd_next_cnt];
	rd_next_cnt++;
	if (SMBusReadWord(raddr->dev_addr, &p->data.ui[wr_data_cnt],
				raddr->reg_addr)) {
		all_send_count++;			
		wr_data_cnt++;
		if (wr_data_cnt >= wr_data_max) {
			wr_data_cnt = 0;
			rd_next_cnt = 0;
			if(Send_Flg == 0) {
				memcpy2(&Send_BufferO.data, &Send_Buffer1.data, sizeof(Send_BufferO.data));
				Send_Flg = 1;
			}
			else {
				++not_send_count;
				if (not_send_count > 10) { 
					TIM_Cmd(TIM2, DISABLE);
					send_err = 1;
				}
			}
		}
	}
	else {
		TIM_Cmd(TIM2, DISABLE);		
		send_err = 2;
	}
}

/*******************************************************************************
 * Function Name : InitExtDevice.
 * Descriptioan  : Инициализация таймера опроса и устройва на SMBus
 * Input		 : None.
 * Output		 : None.
 * Return		 : None.
 *******************************************************************************/
uint8_t InitExtDevice(void) {
	int i;
	uint32_t tmp;
//	SMBus_init(400000);
//	SMBusSendHS(0x08);
	Send_Flg = 0;
	rd_next_cnt = 0;
	wr_data_cnt = 0;
	if(cfg_ini.clk_khz < 100 || cfg_ini.clk_khz > 5000) 
		cfg_ini.clk_khz = SMBus_Speed/1000;
	SMBusInit(cfg_ini.clk_khz * 1000);
	if (cfg_ini.rd_count > MAX_READ_REGS) cfg_ini.rd_count = 0;
	tmp = 73000/cfg_ini.clk_khz; // CLK I2C = 1 MHz -> read reg 60 us 
	if (cfg_ini.rd_count == 0 || cfg_ini.time < tmp) {
		cfg_ini.rd_count = 0;
		wr_data_max = 0; // MAX_READ_REGS*2;
		TIM_Cmd(TIM2, DISABLE);
		if (cfg_ini.time < tmp) {
//			not_send_count = 0xfffffffe;
			return 0;
		}
	} else {
		Timer_Init(cfg_ini.time);
		wr_data_max = (((VIRTUAL_COM_PORT_DATA_SIZE - sizeof(blk_head_t) - 1)/sizeof(int16_t))/cfg_ini.rd_count) * cfg_ini.rd_count;
	}
	Send_BufferO.head.size = Send_Buffer1.head.size = wr_data_max * 2;
	not_send_count = 0;
	all_send_count = 0;
	if (cfg_ini.init_count > MAX_INIT_REGS) cfg_ini.init_count = 0;
	else for(i = 0; i < cfg_ini.init_count ; i++)
		if (!SMBusWriteWord(cfg_ini.init[i].dev_addr, cfg_ini.init[i].data,	cfg_ini.init[i].reg_addr))
			return 0;
    return 1;
}
/*******************************************************************************
 * Function Name	 : main.
 * Descriptioan	 : Main routine.
 * Input			 : None.
 * Output		 : None.
 * Return		 : None.
 *******************************************************************************/
int main(void) {
	uint32_t reg;
	Set_System(); // in hw_config.c
// USB Init
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();
// Buf Send data Init
	Send_Buffer1.head.id = 0x5555;
	Send_Buffer1.head.cmd = 0x07;
	Send_BufferO.head.id = 0x5555;
	Send_BufferO.head.cmd = 0x07;
	Delay_ms(75); // Wait stabilization power voltage
// CFG Init
	if (!ReadIniBlk(&cfg_ini, sizeof(cfg_ini))) {
		cfg_ini.rd_count = 0;
		cfg_ini.init_count = 0;
		cfg_ini.clk_khz = 400;
		cfg_ini.time = 400;
	}
	InitExtDevice();
	while (1) {
		if (bDeviceState == CONFIGURED) {
			CDC_Receive_DATA();
			/*Check to see if we have data yet */
			if (Receive_length >= sizeof(blk_head_t)) {
				if (packet_sent == 1) {
				    blk_cio_t * pbufi = (blk_cio_t *) Receive_Buffer;
					if (pbufi->head.id == 0x5555 
					&& Receive_length >= pbufi->head.size + sizeof(blk_head_t)) {
						switch (pbufi->head.cmd) {
						case 0x00: // Get Ver
							pbufi->data.ud[0] = 0x0110005; // DevID = 0x11, Ver 0.0.0.3 = 0x00002
							pbufi->head.size = 4;
							Receive_length = 4 + sizeof(blk_head_t);
							break;
						case 0x01: // Get/Set CFG/ini & Start measure
							if (pbufi->head.size != 0) {
								timer_flg = 0;
								memcpy2(&cfg_ini, &pbufi->data,
									(pbufi->head.size > sizeof(cfg_ini))? sizeof(cfg_ini) : pbufi->head.size);
								if (!InitExtDevice()) {
									timer_flg = 1;
									pbufi->head.cmd |= 0x80; // Error cmd
									pbufi->head.size = 0;
									Receive_length = 0 + sizeof(blk_head_t);
									break;
								}
								timer_flg = 1;
							} 
							memcpy2(&pbufi->data, &cfg_ini, sizeof(cfg_ini));
							pbufi->head.size = sizeof(cfg_ini);
							Receive_length = sizeof(cfg_ini) + sizeof(blk_head_t);
							break;
						case 0x02: // Store CFG/ini in Flash
							WriteIniBlk(&cfg_ini, sizeof(cfg_ini));
							pbufi->head.size = 0;
							Receive_length = sizeof(blk_head_t);
							break;
						case 0x03: // Status
							pbufi->data.ud[0] = all_send_count;
							pbufi->data.ud[1] = not_send_count;
							pbufi->head.size = 8;
							Receive_length = 8 + sizeof(blk_head_t);
							break;
						//-------
						case 0x10: // Get reg
							timer_flg = 0;
							if (SMBusReadWord(pbufi->data.reg.dev_addr,
								&pbufi->data.reg.data, pbufi->data.reg.reg_addr)) {
								timer_flg = 1;
								pbufi->head.size = sizeof(reg_wr_t);
								Receive_length = sizeof(reg_wr_t) + sizeof(blk_head_t);
							} else {
								timer_flg = 1;
								pbufi->head.cmd |= 0x80; // Error cmd
								pbufi->head.size = 0;
								Receive_length = 0 + sizeof(blk_head_t);
							};
							break;
						case 0x11: // Set reg
							timer_flg = 0;
							if (SMBusWriteWord(pbufi->data.reg.dev_addr,
								pbufi->data.reg.data, pbufi->data.reg.reg_addr)) {
								timer_flg = 1;
								pbufi->head.size = sizeof(reg_wr_t);
								Receive_length = sizeof(reg_wr_t) + sizeof(blk_head_t);
							} else {
								timer_flg = 1;
								pbufi->head.cmd |= 0x80; // Error cmd
								pbufi->head.size = 0;
								Receive_length = 0 + sizeof(blk_head_t);
							};
							break;
						case 0x12: // Get SMBUS dev Alert addr
							timer_flg = 0;
							if (SMBusReadAlert(&pbufi->data.reg.dev_addr)) {
								timer_flg = 1;
								pbufi->head.size = 1;
								Receive_length = 1 + sizeof(blk_head_t);
							} else {
								timer_flg = 1;
								pbufi->head.cmd |= 0x80; // Error cmd
								pbufi->head.size = 0;
								Receive_length = 0 + sizeof(blk_head_t);
							}
							break;
						default:
							pbufi->head.cmd |= 0x80; // Error cmd
							pbufi->head.size = 0;
							Receive_length = 0 + sizeof(blk_head_t);
							break;
						};
					}  else if (Receive_Buffer[0] == 0x55) {
						if(Receive_Buffer[0] == 0x7f) {
							// Get version functions GPIO
							Receive_Buffer[2] = 0x00; // version hi
							Receive_Buffer[3] = 0x02; // version lo
							reg = GPIOA->IDR;
							Receive_Buffer[4] = reg >> 8;	// PA8..15
							Receive_Buffer[5] = reg;		// PA0..7
							reg = GPIOB->IDR;   
							Receive_Buffer[6] = reg >> 8;	// PB8..15
							Receive_Buffer[7] = reg;		// PB0..7
							reg = GPIOC->IDR;
							Receive_Buffer[8] = reg >> 8;  // PC8..15 (STM32F103C8 only PC13..15)
							Receive_Buffer[9] = reg;		// PC0..7
							Receive_length = 10;
						} 
						else if (Receive_length == 6) {
							// Set/Res GPIO
							reg = (Receive_Buffer[2]<<24) | (Receive_Buffer[3]<<16) | (Receive_Buffer[4]<<8) | Receive_Buffer[5];
							switch(Receive_Buffer[1]) {
							case 0x10: 
								GPIOA->BSRR = reg;
								reg = GPIOA->ODR;
								break;
							case 0x11: 
								GPIOB->BSRR = reg;
								reg = GPIOB->ODR;
								break;
							case 0x12: 
								GPIOC->BSRR = reg;
								reg = GPIOC->ODR;
								break;
							
							case 0x20: 
								GPIOA->CRL &= reg;
								reg = GPIOA->CRL;
								break;
							case 0x21: 
								GPIOA->CRH &= reg;
								reg = GPIOA->CRH;
								break;
							case 0x22: 
								GPIOB->CRL &= reg;
								reg = GPIOB->CRL;
								break;
							case 0x23: 
								GPIOB->CRH &= reg;
								reg = GPIOB->CRH;
								break;
							case 0x24: 
								GPIOC->CRL &= reg;
								reg = GPIOC->CRL;
								break;
							case 0x25: 
								GPIOC->CRH &= reg;
								reg = GPIOC->CRH;
								break;
	
							case 0x30: 
								GPIOA->CRL |= reg;
								reg = GPIOA->CRL;
								break;
							case 0x31: 
								GPIOA->CRH |= reg;
								reg = GPIOA->CRH;
								break;
							case 0x32: 
								GPIOB->CRL |= reg;
								reg = GPIOB->CRL;
								break;
							case 0x33: 
								GPIOB->CRH |= reg;
								reg = GPIOB->CRH;
								break;
							case 0x34: 
								GPIOC->CRL |= reg;
								reg = GPIOC->CRL;
								break;
							case 0x35: 
								GPIOC->CRH |= reg;
								reg = GPIOC->CRH;
								break;
							default:
								Receive_Buffer[1] |= 0x80;
							}
							Receive_Buffer[2] = reg >> 24;
							Receive_Buffer[3] = reg >> 16;
							Receive_Buffer[4] = reg >> 8;
							Receive_Buffer[5] = reg;
						}
				    }
					CDC_Send_DATA((uint8_t *) Receive_Buffer, Receive_length);
				};
				Receive_length = 0;
			}
			else if (packet_sent == 1){
				if (Send_Flg) {
					CDC_Send_DATA((uint8_t *)&Send_BufferO, Send_BufferO.head.size + sizeof(blk_head_t));
					Send_Flg = 0;
				} else if (send_err) {
					Send_BufferO.head.cmd = 0x87;
					Send_BufferO.head.size = 9;
					Send_BufferO.data.ud[0]	= all_send_count;
					Send_BufferO.data.ud[1] = not_send_count;
					Send_BufferO.data.uc[8] = send_err;
					CDC_Send_DATA((uint8_t *)&Send_BufferO, 9 + sizeof(blk_head_t));
					Send_BufferO.head.cmd = 0x07;
					send_err = 0;
				};
			};
		}; // if (bDeviceState == CONFIGURED)
	}; // while(1)
}

#ifdef USE_FULL_ASSERT
/*******************************************************************************
 * Function Name	 : assert_failed
 * Description	 : Reports the name of the source file and the source line number
 *				   where the assert_param error has occurred.
 * Input			 : - file: pointer to the source file name
 *				   - line: assert_param error line source number
 * Output		 : None
 * Return		 : None
 *******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1) {}
}
#endif

/*******************************************************************************
 * Function Name	 : TIM2_IRQHandler
 * Description	 : This function handles TIM2 global interrupt request.
 * Input			 : None
 * Output		 : None
 * Return		 : None
 *******************************************************************************/
void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if(timer_flg)
			GetNewRegData();
	}
}

/*****************************END OF FILE****/
