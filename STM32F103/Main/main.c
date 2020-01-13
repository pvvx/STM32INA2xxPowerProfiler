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

#define DEV_I2C 0x55
#define DEV_MCU 0xAA

// DEV_I2C cmd:
#define CMD_DI2C_VER  0x00 // Get Ver
#define CMD_DI2C_CFG  0x01 // Get/Set CFG/ini & Start measure
#define CMD_DI2C_SCF  0x02 // Store CFG/ini in Flash
#define CMD_DI2C_STA  0x03 // Status
#define CMD_DI2C_BLK  0x07 // blk out regs data
#define CMD_DI2C_GRG  0x10 // Get reg
#define CMD_DI2C_SRG  0x11 // Set reg
#define CMD_DI2C_ALR  0x12 // Get SMBUS dev Alert addr (test)

typedef __packed struct {
	uint8_t dev_addr; // адрес на шине i2c
	uint8_t reg_addr; // номер регистра чтения
} reg_rd_t;

typedef __packed struct {
	uint8_t dev_addr; // адрес на шине i2c
	uint8_t reg_addr; // номер регистра чтения
	uint16_t data; // значение для записи в регистр
} reg_wr_t;

#define MAX_INIT_REGS 4
#define MAX_READ_REGS 4

// Структура конфигурации опроса и инициализации устройства
// Выходной пакет непрерывного опроса формируется по данному описанию
typedef __packed struct {
	uint8_t rd_count; 	// кол-во регистров для разового чтения
	uint8_t init_count; // кол-во регистров для инициализации
	uint8_t multiplier; // множитель периода опроса, time << multiplier
	uint8_t pktcnt;  	// кол-во передаваемых значений из регистров в одном пакете передачи 
	uint16_t time; 		// период опроса регистров чтения в us 
	uint16_t clk_khz; 	// частота i2c шины
	reg_wr_t init[MAX_INIT_REGS];
	reg_rd_t rd[MAX_READ_REGS];
} dev_cfg_t; 

#define DEV_I2C 0x55
#define DEV_MCU 0xAA

// DEV_I2C cmd:
#define CMD_DI2C_VER  0x00 // Get Ver
#define CMD_DI2C_CFG  0x01 // Get/Set CFG/ini & Start measure
#define CMD_DI2C_SCF  0x02 // Store CFG/ini in Flash
#define CMD_DI2C_STA  0x03 // Status
#define CMD_DI2C_BLK  0x07 // blk out regs data
#define CMD_DI2C_GRG  0x10 // Get reg
#define CMD_DI2C_SRG  0x11 // Set reg
#define CMD_DI2C_ALR  0x12 // Get SMBUS dev Alert addr (test)
//
#define CMD_ERR_FLG   0x80 // send error cmd mask
//
typedef  __packed struct{
	uint8_t size; // размер пакета
	uint8_t cmd;  // номер команды / тип пакета
} blk_head_t;
	
typedef  __packed struct _blk_cio_t{
	blk_head_t head;
	__packed union {
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
#define Timer_Stop()		TIM_Cmd(TIM2, DISABLE);
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
void Timer_Init(uint16_t period_us, uint16_t multiplier) {
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
	TIM_TimeBaseStructure.TIM_Prescaler = (72 << multiplier) - 1; // 72 MHz Clock down to 1 MHz (adjust per your clock)
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
	if (SMBusReadWord(raddr->dev_addr, (uint16_t *)&p->data.ui[wr_data_cnt],
				raddr->reg_addr)) {
		all_send_count++;			
		wr_data_cnt++;
		if (wr_data_cnt >= wr_data_max) {
			wr_data_cnt = 0;
			rd_next_cnt = 0;
			if(Send_Flg == 0) {
				memcpy2(&Send_BufferO, &Send_Buffer1, sizeof(Send_BufferO));
				Send_Flg = 1;
			}
			else {
				++not_send_count;
				if (not_send_count > 10) { 
					Timer_Stop();
					send_err = 1;
				}
			}
		}
	}
	else {
		Timer_Stop();
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
		Timer_Stop();
		if (cfg_ini.time < tmp) {
//			not_send_count = 0xfffffffe;
			return 0;
		}
	} else {
		Timer_Init(cfg_ini.time, cfg_ini.multiplier);
		wr_data_max = (((VIRTUAL_COM_PORT_DATA_SIZE - sizeof(blk_head_t) - 1)/sizeof(int16_t))/cfg_ini.rd_count) * cfg_ini.rd_count;
		if(cfg_ini.pktcnt != 0 && cfg_ini.pktcnt <= wr_data_max)
			wr_data_max = cfg_ini.pktcnt;
	}
	// Buf Send data Init
	
	Send_Buffer1.head.size = wr_data_max * 2;
	Send_Buffer1.head.cmd = CMD_DI2C_BLK;
	
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
	Set_System(); // in hw_config.c
// USB Init
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();
	Delay_ms(75); // Wait stabilization power voltage
// CFG Init
	if (!ReadIniBlk(&cfg_ini, sizeof(cfg_ini))) {
		cfg_ini.rd_count = 0;
		cfg_ini.init_count = 0;
		cfg_ini.time = 400;
		cfg_ini.multiplier = 0;
		cfg_ini.clk_khz = 400;
		cfg_ini.pktcnt = 30;
	}
	InitExtDevice();
	while (1) {
		if (bDeviceState == CONFIGURED) {
			CDC_Receive_DATA();
			/*Check to see if we have data yet */
			if (Receive_length >= sizeof(blk_head_t)) {
				if (packet_sent == 1) {
				    blk_cio_t * pbufi = (blk_cio_t *) Receive_Buffer;
					if (Receive_length >= pbufi->head.size + sizeof(blk_head_t)) {
						switch (pbufi->head.cmd) {
						case CMD_DI2C_VER: // Get Ver
							pbufi->data.ui[0] = 0x0016; // DevID = 0x0016
							pbufi->data.ui[1] = 0x0006; // Ver 0.0.0.6 = 0x0006
							Receive_length = 4 + sizeof(blk_head_t);
							break;
						case CMD_DI2C_CFG: // Get/Set CFG/ini & Start measure
							if (pbufi->head.size) {
								timer_flg = 0;
								memcpy2(&cfg_ini, &pbufi->data,
									(pbufi->head.size > sizeof(cfg_ini))? sizeof(cfg_ini) : pbufi->head.size);
								if (!InitExtDevice()) {
									timer_flg = 1;
									pbufi->head.cmd |= CMD_ERR_FLG; // Error cmd
									Receive_length = 0 + sizeof(blk_head_t);
									break;
								}
								timer_flg = 1;
							} 
							memcpy2(&pbufi->data, &cfg_ini, sizeof(cfg_ini));
							Receive_length = sizeof(cfg_ini) + sizeof(blk_head_t);
							break;
						case CMD_DI2C_SCF: // Store CFG/ini in Flash
							WriteIniBlk(&cfg_ini, sizeof(cfg_ini));
							Receive_length = sizeof(blk_head_t);
							break;
						case CMD_DI2C_STA: // Status
							pbufi->data.ud[0] = all_send_count;
							pbufi->data.ud[1] = not_send_count;
							Receive_length = sizeof(pbufi->data.ud[0]) + sizeof(pbufi->data.ud[1]) + sizeof(blk_head_t);
							break;
						//-------
						case CMD_DI2C_GRG: // Get reg
							timer_flg = 0;
							if (SMBusReadWord(pbufi->data.reg.dev_addr,
								(uint16_t *)&pbufi->data.reg.data, pbufi->data.reg.reg_addr)) {
								timer_flg = 1;
								Receive_length = sizeof(reg_wr_t) + sizeof(blk_head_t);
							} else {
								timer_flg = 1;
								pbufi->head.cmd |= CMD_ERR_FLG; // Error cmd
								Receive_length = 0 + sizeof(blk_head_t);
							};
							break;
						case CMD_DI2C_SRG: // Set reg
							timer_flg = 0;
							if (SMBusWriteWord(pbufi->data.reg.dev_addr,
								pbufi->data.reg.data, pbufi->data.reg.reg_addr)) {
								timer_flg = 1;
								Receive_length = sizeof(reg_wr_t) + sizeof(blk_head_t);
							} else {
								timer_flg = 1;
								pbufi->head.cmd |= CMD_ERR_FLG; // Error cmd
								Receive_length = 0 + sizeof(blk_head_t);
							};
							break;
						case CMD_DI2C_ALR: // Get SMBUS dev Alert addr
							timer_flg = 0;
							if (SMBusReadAlert(&pbufi->data.reg.dev_addr)) {
								timer_flg = 1;
								Receive_length = 1 + sizeof(blk_head_t);
							} else {
								timer_flg = 1;
								pbufi->head.cmd |= CMD_ERR_FLG; // Error cmd
								Receive_length = 0 + sizeof(blk_head_t);
							}
							break;
						default:
							pbufi->head.cmd |= CMD_ERR_FLG; // Error cmd
							Receive_length = 0 + sizeof(blk_head_t);
							break;
						};
						pbufi->head.size = Receive_length - sizeof(blk_head_t);
						CDC_Send_DATA((uint8_t *) Receive_Buffer, Receive_length);
					}
					Receive_length = 0;
				};
			}
			else if (packet_sent == 1){
				if (Send_Flg) {
					CDC_Send_DATA((uint8_t *)&Send_BufferO, Send_BufferO.head.size + sizeof(blk_head_t));
					Send_Flg = 0;
				} else if (send_err) {
					Send_BufferO.head.cmd = CMD_DI2C_BLK | CMD_ERR_FLG;
					Send_BufferO.head.size = 9;
					Send_BufferO.data.ud[0]	= all_send_count;
					Send_BufferO.data.ud[1] = not_send_count;
					Send_BufferO.data.uc[8] = send_err;
					CDC_Send_DATA((uint8_t *)&Send_BufferO, 9 + sizeof(blk_head_t));
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
