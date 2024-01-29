# Webnet网页服务器演示

## 概述

本示例演示了rtthread的webnet组件

## 硬件设置
* 使用USB Type-C线缆连接PC USB端口和PWR DEBUG端口
* 使用以太网线缆连接PC以太网端口和开发板RGMII或RMII端口


## 完整的测试步骤

### 1. 上电初始化log
```console
 \ | /
- RT -     Thread Operating System
 / | \     5.0.1 build Dec 18 2023 18:03:54
 2006 - 2022 Copyright by RT-Thread team
lwIP-2.1.2 initialized!
[3] I/sal.skt: Socket Abstraction Layer initialize success.
[D/FAL] (fal_flash_init:47) Flash device |                norflash0 | addr: 0x80000000 | len: 0x01000000 | blk_size: 0x00001000 |initialized finish.
[I/FAL] ==================== FAL partition table ====================
[I/FAL] | name | flash_dev |   offset   |    length  |
[I/FAL] -------------------------------------------------------------
[I/FAL] | app  | norflash0 | 0x00000000 | 0x00600000 |
[I/FAL] | fs   | norflash0 | 0x00600000 | 0x00a00000 |
[I/FAL] =============================================================
[I/FAL] RT-Thread Flash Abstraction Layer initialize success.
[I/FAL] The FAL block device (fs) created successfully
Create a block device on the fs partition of flash successful.
Filesystem initialized!
msh />[1084] I/NO_TAG: PHY Status: Link up
[1088] I/NO_TAG: PHY Speed: 100Mbps
[1092] I/NO_TAG: PHY Duplex: full duplex
```

### 例程使用

- 参考rtthread的组件webnet下的[sample 使用说明](packages/webnet-latest/docs/samples.md)





