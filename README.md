# STM32INA2xxPowerProfiler
STM32/TLSR8266 INA219/INA226 Power Profiler

![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/STM32INA2xxPowerProfiler.gif)

![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/STM32INA219..gif)

![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/STM32INA226.gif)

![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/JDY-10-INA219.gif)

![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/JDY-10-INA226.gif)

JDY-10 pin PC5:<br>
='0' (gnd) - BLE<br>
='1' (+3.3v) - USB<br>
# WebBluetooth: Connect
![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/WebBluetooth1.gif)
# WebBluetooth: Work
![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/WebBluetooth2.gif)
# WebBluetooth: Settings in test.html
![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/UserSet.gif)
# AndroidScreenshot
![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/AndroidScreenshot.gif)

Полной оптимизации LowPower на BLE версии пока не производилось. Текущие при TX +8 дБ (вместе с INA2xx):
* В режиме ожидания соединения: до 0.200 мА (3.3В)
* В режиме соединения и передачи данных по BLE: от 9 до 11 мА (3.3В) (зависит от скорости опроса INA2xx)

USB версия (вместе с INA2xx):
* В режиме ожидания USB подключения (USB шина отключена): 14.3 мА (3.3В)
* В режиме ожидания открытия COM порта (USB шина подключена и активна): 14.4 мА (3.3В)
* В режиме соединения и передачи данных в USB: от 14 до 15.6 мА (3.3В) (зависит от скорости опроса INA2xx)
Ток при работе с USB имеет зависимость от нагрузки - кабеля и ответного USB.

[Готовые прошивка и программа](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/bin/STM32INA219BIN.zip)

[Форум](https://esp8266.ru/forum/threads/power-profiler.4643)
