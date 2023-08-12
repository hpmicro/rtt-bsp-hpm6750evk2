# 更新

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
