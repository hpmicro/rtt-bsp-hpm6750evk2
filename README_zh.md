# rtt-bsp-hpm6750evk2

HPM6750EVK 开板的 RT-Thread Studio 板级支持包

## 支持的示例

***

- blink_led
- lv_demo_widgets
- uart_dma_demo
- timer_demo
- sdcard_demo
- usb_device_generic_hid
- flashdb_demo
- can_example
- ethernet_demo
- ethernet_ptp_master_demo
- ethernet_ptp_slave_demo
- i2c_demo
- usb_host_msc_udisk
- audio_i2s_demo
- audio_pdm_dao_demo


## 环境设置

## 工具链环境设置
- 安装好BSP包后, 用户在创建示例/工程前需要 设置 `RTT_RISCV_TOOLCHAIN` 环境变量。
    - 假定RISC-V工具链安装于目录： `C:\DevTools\RT-ThreadStudio\repo\Extract\ToolChain_Support_Packages\RISC-V\RISC-V-GCC-RV32\2022-04-12\bin`, 用户可参考下图，配置`RTT_RISCV_TOOLCHAIN`环境变量 
    ![设置 RTT_RISCV_TOOLCHAIN 环境变量](documents/images/set_rtt_riscv_toolchain_env.png)
