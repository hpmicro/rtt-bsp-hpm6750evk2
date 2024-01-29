/*
 * Copyright (c) 2021 hpmicro
 *
 * Change Logs:
 * Date         Author          Notes
 * 2021-08-13   Fan YANG        first version
 *
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "rtt_board.h"
#include "dfs_fs.h"
#include "dfs_elm.h"
#ifdef RT_USING_FAL
#include "fal.h"
#endif

#define FS_PARTITION_NAME      "fs"
extern void rt_hw_cpu_reset(void);

void thread_entry(void *arg);
static void dfs_fal_init(void);

int main(void)
{

    app_init_led_pins();
    dfs_fal_init();
    static uint32_t led_thread_arg = 0;
    rt_thread_t led_thread = rt_thread_create("led_th", thread_entry, &led_thread_arg, 1024, 25, 10);
    rt_thread_startup(led_thread);
    return 0;
}


void thread_entry(void *arg)
{
    while(1){
        app_led_write(0, APP_LED_ON);
        rt_thread_mdelay(500);
        app_led_write(0, APP_LED_OFF);
        rt_thread_mdelay(500);
        app_led_write(1, APP_LED_ON);
        rt_thread_mdelay(500);
        app_led_write(1, APP_LED_OFF);
        rt_thread_mdelay(500);
        app_led_write(2, APP_LED_ON);
        rt_thread_mdelay(500);
        app_led_write(2, APP_LED_OFF);
        rt_thread_mdelay(500);
    }
}

static void dfs_fal_init(void)
{
#ifdef RT_USING_FAL
    fal_init();
    struct rt_device *flash_dev = fal_blk_device_create(FS_PARTITION_NAME);
    if (flash_dev == NULL){
        rt_kprintf("Can't create a block device on '%s' partition.\n", FS_PARTITION_NAME);
    } else {
        rt_kprintf("Create a block device on the %s partition of flash successful.\n", FS_PARTITION_NAME);
    }
   if (dfs_mount(FS_PARTITION_NAME, "/", "elm", 0, 0) == 0)
    {
        rt_kprintf("Filesystem initialized!\n");
    }
    else
    {
        rt_device_open(flash_dev, RT_DEVICE_OFLAG_RDWR);
        rt_kprintf("Failed to initialize filesystem!\n");
        rt_kprintf("You should create a filesystem on the block device first!\n");
        elm_init();
        dfs_mkfs("elm", "fs");
        rt_thread_mdelay(1000);
        rt_hw_cpu_reset();
    }
#endif
}
