# STM32INA2xxPowerProfiler #
(!) The further development of the project was transferred to [UBIA](https://github.com/pvvx/UBIA)

STM32/TLSR8266 INA219/INA226 Power Profiler
![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/STM32INA2xxPowerProfiler.gif)

![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/STM32INA219..gif)

![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/STM32INA226.gif)

![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/JDY-10-INA219.gif)

![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/JDY-10-INA226.gif)

JDY-10 pin PD5:<br>
='0' (“not connected or gnd") - BLE<br>
='1' (+3.3v) - USB<br>
# WebBluetooth: Connect
![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/WebBluetooth1.gif)
# WebBluetooth: Work
![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/WebBluetooth2.gif)
# WebBluetooth: Settings in test.html
![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/UserSet.gif)
# AndroidScreenshot
![SCH](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/Docs/AndroidScreenshot.gif)

# BLE Power:
Полной оптимизации LowPower на BLE версии пока не производилось. Текущие при TX +8 дБ (вместе с INA2xx) по шине 3.3В:
* В режиме ожидания соединения:
91 uA + 57.8 uA (INA219 sleep) = до 0.150 мА (3.3В)
* В режиме соединения и передачи данных по BLE:<br>
При 51..2200 sps -> Power mode: 8.9..14.5 mA (0.7 mA INA219)<br>
При 0.125..50 sps -> Low power mode: 0.7..3.5 mA (INA219: 0.7 mA, Sleep TLSR8266:13.2 uA, TX +8 дБ: пик 32 mA)

# USB Power:
 USB версия (вместе с INA2xx):
* В режиме ожидания USB подключения (USB шина отключена): 9.16 мА (3.3В)
* В режиме ожидания открытия COM порта (USB шина подключена и активна): 9.9 мА (3.3В)
* В режиме соединения и передачи данных в USB при 14 sps..12 ksps: от 10.1 до 10.9 мА (3.3В)<br>
Ток при работе с USB имеет зависимость от нагрузки - кабеля и ответного USB.

# Deep-Sleep in BLE Mode (PD5 not connected):

PB0 - Deep-Sleep Timer 30 sec<br>
3 uA (TLSR8266)<br>
WakeUP: PB0 = “not connected or 1”

PA5 - Deep-Sleep Pin WakeUP<br>
1.4 uA (TLSR8266)<br>
WakeUP: PA5 = “1”


[Готовые прошивка и программа](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/bin/STM32INA219BIN.zip)

[Форум](https://esp8266.ru/forum/threads/power-profiler.4643)
