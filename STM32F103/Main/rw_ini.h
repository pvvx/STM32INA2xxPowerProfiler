//
// EEPROM` в FLASH для сохранения настроек (pvvx Ver 1.0)
// Циклически использует область в 1024 байт (страницу) во Flash MCU
// для сохранения блока конфигурации
//
#ifndef __RW_FLASH_INI_H
#define __RW_FLASH_INI_H

#include "stm32f10x.h"
#include "stm32f10x_flash.h"

// Medium-density devices are STM32F101xx, STM32F102xx and STM32F103xx microcontrollers
// where the Flash memory density ranges between 64 and 128 Kbytes.
#define FLASH_SIZE 65536ul // размер Flash
#define FLASH_PAGE_SIZE 1024ul // размер страницы Flash для FLASH_ErasePage()

#define FLASH_STORE_STR_ADR FLASH_BASE + FLASH_SIZE - FLASH_PAGE_SIZE // стартовый адрес блока сохранения настроек в Flash (64k - страница)
#define FLASH_STORE_END_ADR FLASH_BASE + FLASH_SIZE - 1 // конечный адрес  блока сохранения настроек в Flash (конецный адрес Flash)

void WriteIniBlk(void * blk, uint16_t blk_size); // запись блока настроек во Flash
int ReadIniBlk(void * blk, uint16_t blk_size); // чтение блока настроек из Flash

#endif //__RW_FLASH_INI_H

