# Change Log

## v1.2.0
- Integrated hpm_sdk v1.2.0
  - Note:
    - the docs, middleware, scripts, samples folder in SDK root directory were removed
- Updated:
    - Upgraded rt-thread to v5.0.1
    - Optimized the performance of LVGL demo
    - Added README.md for all samples
    - Added UART RXFIFO timeout interrupt support
    - Added `flash_release` build and `ram_release` build
  - Fixed:
    - SD card performance is slow
    - Uart lost data when working in low baudrate

## v1.1.0

- Updated:
  - Integrated HPM_SDK v1.1.0 release
    - NOTE:
      - The directory *docs*, *samples*, *middleware* and *cmake* under hpm_sdk directory has been removed
- New samples：
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
