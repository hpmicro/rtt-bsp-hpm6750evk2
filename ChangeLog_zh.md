# 更新
## v1.11.0

- 整合了hpm_sdk v1.11.0
- 升级RT-Thread 到 v5.2.2
- 新增 RT-Thread 标准动态中断注册机制，支持在静态绑定与动态管理模式间灵活切换
- 新增通用启动文件（generic startup）支持，同时兼容原有启动方式

- 更新：
  - CherryUSB协议栈从软件包支持改为RT-Thread的组件驱动支持
  - 增强UART V2驱动
  - 增强MCAN驱动
  - 增强CAN驱动
  - 增强ENET驱动
  - 增强ENET PHY驱动

- 修复：

- 新增：
  - 新增软件I2C驱动
  - 新增软件SPI驱动
  - 新增uart_interrupt_demo
  - UART V2驱动新增PUART
  - TMR驱动新增PTMR


## v1.10.0

- 整合了 hpm_sdk v1.10.0

- 更新：
  - 升级CherryUSB协议栈到1.5.0
  - RTC驱动支持闹钟警报设置和获取
  - RTC驱动支持 `timeval` 的设置和获取

- 修复：
  - 修复RTC驱动只支持32bit timestamp问题，可根据time_t长度支持32bit和64bit timestamp
  - 修复中断向量模式在某些条件下工作异常的问题
  - 修复shell 执行`reset`命令后，看门狗复位失效的问题

- 新增
  - 支持zcc编译器
  - 支持SEGGER Embedded Studio IDE，版本为8.24

## v1.9.0

- 整合了 hpm_sdk v1.9.0

- 更新：
  - 升级CherryUSB协议栈到1.4.3
  - `sdio`驱动支持可配置的IRQ优先级
  - `uart/uart_v2`驱动支持可配置的IRQ优先级
  - `spi`驱动支持可配置的IRQ优先级
  - `i2c`驱动支持可配置的IRQ优先级
  - `lcd`驱动支持可配置的IRQ优先级
  - `gpio`驱动支持可配置的IRQ优先级
  - `can`驱动支持可配置的IRQ优先级

- 修复
  - `ethernet_ptp_slave_demo`修复主设备和从设备之间的相同MAC地址问题

- 新增:
  - `sdio` 驱动增加了中断驱动模式，默认切换到ADMA3模式，增加了对非对齐buffer的安全处理
  - `spi`驱动增加DSPI/QSPI模式

## v1.6.0

- 整合了hpm_sdk v1.6.0
- 修复:
  - UART7 和 UART8 TX DMA 相关的宏定义错误问题
  - uart_v2 中的内存泄露问题
- 更新：
  - 优化CANFD帧的BRS位控制
  - 升级CherryUSB协议栈到1.3.1
  - 为USB例程增加了全速模式相关的描述符
  - 芯片的数据手册
  - 优化了SDIO驱动，增加了SDIO的`BUS_WIDTH`等配置选项，增加eMMC HS400支持，增加SD UHS-I 速度模式支持
  - 在`sdcard_demo`示例中增加了性能测试命令
- 新增:
  - SPI 的CS引脚控制选项
  - 适配了RT-Thread Cache 驱动
- 新增示例：
  - airoc_wifi_demo （英飞凌 AIROC 系列WiFi）

## v1.5.0

- 整合了hpm_sdk v1.5.0
- 更新:
  - USB相关sample协议栈切换为cherryUSB
  - ETH PHY 抽象层多实例支持和优化
- 修复:
  - CAN sample开启硬件过滤器读取数据错误的问题
- 新增:
  - 增加中断向量模式和可抢占模式
  - systemView组件
  - rtt lcd driver驱动

## v1.4.1

- 修复：
  - I2C开启DMA后传输出错的问题
  - 默认模板工程创建出错的问题

## v1.4.0

- 整合了hpm_sdk v1.4.0
- 升级RT-Thread 到 v5.0.2
- 更新
  - 增强了 SPI 驱动
  - 增强了 I2C 驱动
  - 增加了 SDIO驱动对多SDXC实例的支持
  - 增加了对 `rt_pin_get` API的支持
  - 统一了ethernet phy驱动
  - 增强了i2s驱动
  - RTC驱动中默认使能电池域掉电保持
- 修复：
  - 修复了`pwm`驱动返回值类型不匹配的问题
- 新增示例
  - web_camera_demo
  - webnet_demo

## v1.3.0

- 整合了hpm_sdk v1.3.0
  - 注：
    - SDK根目录下的docs,middleware,samples, scripts等目录被移除
- 更新
  - 增加了SVD文件的支持
  - CAN: 增加了硬件过滤器支持
  - 默认禁用了PWM输出反向
- 修复：
  - RT-Thread Studio生成的elf文件无法被`Ozone`正确的识别
  - 在线包`i2c tools`不工作
  - 以太网在网络风暴产生后工作不正常
  - 音频声道控制在产生上溢/下溢后产生异常
  - 当开启`ADC12`后编译失败
  - GPIO在配置为开漏极并上拉模式后无法正确的读出引脚的电平

## v1.2.0

- 整合了hpm_sdk v1.2.0
  - 注：
    - SDK根目录下的docs,middleware,samples, scripts等目录被移除
- 更新:
  - 迁移到rt-thread v5.0.1
  - 优化了lvgl demo的性能
  - 增加了sample README
  - 增加了 `flash_release` 和 `ram_release` 工程
  - 增加了 UART RXFIFO timeout 中断支持
- 修复：
  - sd card 性能慢
  - uart 在低波特率下丢数据

## v1.1.0

- 更新：
  - 整合了 HPM_SDK v1.1.0 release
    - 注:
      - HPM_SDK根目录下 *docs*, *samples*, *middleware* 和 *cmake* 目录已移除
- 新增示例：
  - audio_i2s_demo
  - audio_pdm_dao_demo
  - blink_led
  - can_example
  - ethernet_demo
  - ethernet_ptp_master_demo
  - ethernet_ptp_slave_demo
  - flashdb_demo
  - i2c_demo
  - lv_demo_widgets
  - sdcard_demo
  - uart_dma_demo
  - usb_device_generic_hid
  - usb_host_msc_udisk
