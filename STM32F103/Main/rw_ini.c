/*******************************************************************************
// EEPROM` в FLASH для сохранения настроек (pvvx Ver 1.0)
// Циклически использует область одной страницы во Flash
// для сохранения блока конфигурации
*******************************************************************************/

#include "rw_ini.h"

// Находит адрес для последнего записанного блока настроек.
// Условие - у блока присуствует начальный и конецный маркер записи.
// Если = 0 - нет записей.

uint32_t GetLastAdrIniBlk(int blk_size)
{
  uint32_t adr = FLASH_STORE_STR_ADR, ret = 0;
  uint16_t w, len = blk_size + (blk_size&0x1) + 4;

    while (adr < FLASH_STORE_END_ADR + 2 - len) // цикл до конца области записей
    {
      w = *(uint16_t*)adr; // считать маркер старта записи
      if(w == 0x55AA)  // есть маркер старта записи?
      { // есть маркер старта записи
        w = *(uint16_t*)(adr + len - 2); // считать маркер конца записи
        if(w == 0xAA55) ret = adr; // есть маркер конца записи? да.
      }
      else return ret; // нет маркера старта записи
      adr += len; // шаг на следующий адрес блока
    };
    return ret;
}


// Находит последний пустой адрес для записи блока настроек.
// Если = 0 - нет пустых блоков

uint32_t GetLastAdrClrBlk(int blk_size)
{
  uint32_t adr = FLASH_STORE_STR_ADR;
  uint16_t w, len = blk_size + (blk_size&0x1) + 4;

    while (adr < FLASH_STORE_END_ADR + 2 - len) // цикл до конца области записей
    {
      w = *(uint16_t*)adr;  // считать маркер старта записи
      if(w == 0xFFFF) return adr; // есть маркер старта записи? нет.
      adr += len;
    };
    return 0; //
}

//* Запись блока настроек во Flash

void WriteIniBlk(void * blk, uint16_t blk_size)
{
  uint32_t adr;
  uint16_t i = 0,w;
  uint8_t *src = (uint8_t *)blk;

   adr = GetLastAdrClrBlk(blk_size); // найти последний пустой адрес для записи блока настроек
   FLASH_Unlock();
   if (adr == 0)        // пустое место для записи блока не найдено
   {
     adr = FLASH_STORE_STR_ADR; // ипользуем начальный адрес страницы
     FLASH_ErasePage(FLASH_STORE_STR_ADR); // очишаем страницу flash
   };
   FLASH_ProgramHalfWord(adr, 0x55AA); // записываем маркер старта записи блока
   // Сохраняем сам блок 
   adr += 2;
   while (i<blk_size)
   {
     w = *src++;
     w |= *src++ << 8;
     FLASH_ProgramHalfWord(adr, w);
     adr += 2;
     i += 2;
   };
   FLASH_ProgramHalfWord(adr, 0xAA55); // записываем маркер конца записи блока
}

//* Чтение блока из Flash

int ReadIniBlk(void * blk, uint16_t blk_size)
{
  uint32_t adr;
  uint8_t *dest = (uint8_t *)blk;
  uint8_t *src;

   adr = GetLastAdrIniBlk(blk_size); // найти адрес последнего записанного блока настроек
   if(adr == 0) return 0; // нет записей
   // скопировать блок из Flash в запрос
   src = (uint8_t *)(adr+2);
   while (blk_size--) *dest++ = *src++;
   return 1; // блок считан
}

