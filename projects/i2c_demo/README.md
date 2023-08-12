# 简介

本例程演示基于RT-Thread I2C驱动框架对板载EEPROM器件进行读写。

## 支持的命令

```console
RT-Thread shell commands:
eeprom_i2c       - "eeprom_i2c 0x50/0x51"
reset            - reset the board
clear            - clear the terminal screen
version          - show RT-Thread version information
list             - list objects
help             - RT-Thread shell help.
ps               - List threads in the system.
free             - Show the memory usage in the system.
pin              - pin [option]
date             - get date and time or set (local timezone) [year month day hour min sec]
```

## 运行现象
使用`eeprom_i2c 0x50`命令， '0x50'是EEPROM的器件地址
```console
msh >eeprom_i2c 0x50
eeprom device address: 0x50
eeprom write and read succes
write 8 bytes from address 0x00:
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07
read 8 bytes from address 0x00:
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07
```
