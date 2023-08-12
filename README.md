# rtt-bsp-hpm6750evk2

RT-Thread Studio BSP for HPM6750EVK Board

## Supported examples

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

## Environment Setup

### Toolchain setup

- After installing the BSP package, users need to set the `RTT_RISCV_TOOLCHAIN` environment variable before creating any examples/projects
    - Assuming that the toolchain is installed in `C:\DevTools\RT-ThreadStudio\repo\Extract\ToolChain_Support_Packages\RISC-V\RISC-V-GCC-RV32\2022-04-12\bin`, User can set the `RTT_RISCV_TOOLCHAIN` environment variable referring below figure:

![Set RTT_RISCV_TOOLCHAIN environment variable](documents/images/set_rtt_riscv_toolchain_env.png)