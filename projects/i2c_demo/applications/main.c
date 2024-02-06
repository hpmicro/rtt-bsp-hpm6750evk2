/*
 * Copyright (c) 2022 hpmicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "rtt_board.h"
#include "board.h"

#define BOARD_APP_I2C_NAME    BOARD_EEPROM_I2C_NAME
#define EEPROM_ADDR_STR_LEN   4
rt_uint8_t device_addr;


rt_err_t eeprom_i2c_write(struct rt_i2c_bus_device *i2c_bus, rt_uint8_t addr, rt_uint8_t *data, rt_uint32_t len)
{
   rt_uint8_t buf[len+1];
   rt_size_t result;

    buf[0] = addr;
    for (rt_uint32_t i = 0; i < len; i++)
    {
        buf[i+1] = *(data++);
    }

    result = rt_i2c_master_send(i2c_bus, device_addr, RT_I2C_WR, buf, len+1);

    if (result != len+1)
    {
        return RT_ERROR;
    }
    return RT_EOK;
}

rt_err_t eeprom_i2c_read(struct rt_i2c_bus_device *i2c_bus, rt_uint8_t addr, rt_uint8_t *buf, rt_uint32_t len)
{
    rt_size_t result;

    result = rt_i2c_master_send(i2c_bus, device_addr, RT_I2C_WR, &addr, 1);
    if (result != 1) {
        return RT_ERROR;
    }

    result = rt_i2c_master_recv(i2c_bus, device_addr, RT_I2C_RD, buf, len);
    if (result != len) {
        return RT_ERROR;
    }

    return RT_EOK;
}

static int eeprom_i2c(int argc, char *argv[])
{
    char i2c_name[RT_NAME_MAX];
    char eeprom_addr_str[EEPROM_ADDR_STR_LEN + 1] = {0};
    struct rt_i2c_bus_device *i2c_dev;
    rt_uint8_t eeprom_reg_addr = 0x0;
    rt_uint8_t data_count = 8;
    rt_uint8_t wr_data[data_count], rd_data[data_count];
    rt_err_t status;

    rt_strncpy(eeprom_addr_str, argv[1], EEPROM_ADDR_STR_LEN);
    if (argc == 2)
    {
        rt_strncpy(i2c_name, BOARD_APP_I2C_NAME, RT_NAME_MAX);
    }
    else if (argc == 3)
    {
        rt_strncpy(i2c_name, argv[2], RT_NAME_MAX);
    } else {
        rt_kprintf("Please use correct cmd format: eeprom_i2c 0x50\n");
        return -1;
    }

    i2c_dev = rt_i2c_bus_device_find(i2c_name);
    if (!i2c_dev)
    {
        rt_kprintf("find %s failed!\n", i2c_name);
        return -1;
    }

    device_addr = (rt_uint8_t)strtol(eeprom_addr_str, NULL, 16);
    rt_kprintf("eeprom device address: 0x%02x\n", device_addr);

    /* prepare write data */
    for(rt_uint32_t i = 0; i < data_count; i++)
    {
        wr_data[i] = i % 0xFF;
    }

    status = eeprom_i2c_write(i2c_dev, eeprom_reg_addr, wr_data, data_count);
    if (status != RT_EOK)
    {
        rt_kprintf("eeprom write failed\n");
        return -1;
    }
    rt_thread_mdelay(10);
    status = eeprom_i2c_read(i2c_dev, eeprom_reg_addr, rd_data, data_count);
    if (status != RT_EOK)
    {
        rt_kprintf("eeprom read failed\n");
        return -1;
    }

    rt_kprintf("eeprom write and read succes\n");
    rt_kprintf("write %d bytes from address 0x%02x:\n", data_count, eeprom_reg_addr);
    for(rt_uint32_t i = 0; i < data_count; i++)
    {
        rt_kprintf("0x%02x ", wr_data[i]);
    }
    rt_kprintf("\n");
    rt_kprintf("read %d bytes from address 0x%02x:\n", data_count, eeprom_reg_addr);
    for(rt_uint32_t i = 0; i < data_count; i++)
    {
        rt_kprintf("0x%02x ", rd_data[i]);
    }
    rt_kprintf("\n");
}
MSH_CMD_EXPORT(eeprom_i2c, "eeprom_i2c 0x50");

int main(void)
{
    return 0;
}
