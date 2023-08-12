/*
 * Copyright (c) 2022 hpmicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "rtt_board.h"

#include "drivers/audio.h"
#include "board.h"
#include "hpm_i2s_drv.h"
#include "drv_i2s.h"
#include "wav_header.h"
#include "hpm_wm8960.h"
#include "hpm_clock_drv.h"
#include <dfs_fs.h>
#include <dfs_posix.h>

#define BUFF_SIZE 2048
uint8_t data_buff[BUFF_SIZE];

#define CODEC_I2S_DEV_NAME        "i2s0"
#define CODEC_I2S_CLK_NAME        clock_i2s0
#define CODEC_I2C_DEV_NAME        "i2c0"
#define CODEC_I2S_DATA_LINE       I2S_DATA_LINE_2
#define CODEC_I2S_CHANNEL         i2s_stereo
#define CODEC_I2S_SAMPLERATE      16000
#define CODEC_I2S_SAMPLEBITS      16
#define RECORD_TIME_MS            10000

static void wavheader_init(wav_header_t *header, uint32_t sample_rate, uint8_t channels, uint8_t sample_bits, uint32_t datasize)
{
    memcpy(header->riff_chunk.id, "RIFF", 4);
    header->riff_chunk.datasize = datasize + 44 - 8;
    memcpy(header->riff_chunk.type, "WAVE", 4);
    memcpy(header->fmt_chunk.id, "fmt ", 4);
    header->fmt_chunk.datasize = 16;
    header->fmt_chunk.compression_code = 1;
    header->fmt_chunk.channels = channels;
    header->fmt_chunk.sample_rate = sample_rate;
    header->fmt_chunk.bit_per_sample = sample_bits;
    header->fmt_chunk.avg_bytes_per_sec = header->fmt_chunk.sample_rate * header->fmt_chunk.channels * header->fmt_chunk.bit_per_sample / 8;
    header->fmt_chunk.block_align = header->fmt_chunk.bit_per_sample * header->fmt_chunk.channels / 8;
    memcpy(header->data_chunk.id, "data", 4);
    header->data_chunk.datasize = datasize;
}

static int codec_recordwav(int argc, char *argv[])
{
    int fd = -1;
    wav_header_t header;
    uint8_t channel_num = 1;
    uint32_t data_size = 0;
    uint32_t target_data_size = 0;

    rt_device_t i2s_dev;
    struct rt_audio_caps i2s_caps = {0};
    struct rt_i2c_bus_device *i2c_bus;

    wm8960_config_t wm8960_config;
    wm8960_control_t wm8960_control;

    if (argc != 2)
    {
        rt_kprintf("Usage:\n");
        rt_kprintf("codec_recordwav /test.wav\n");
        return -RT_ERROR;
    }

    fd = open(argv[1], O_WRONLY | O_CREAT);
    if (fd < 0)
    {
        rt_kprintf("open file failed!\n");
        return -RT_ERROR;
    }

    write(fd, &header, sizeof(wav_header_t));

    //获取 i2c_bus device, 控制codec
    i2c_bus = rt_i2c_bus_device_find(CODEC_I2C_DEV_NAME);
    if (!i2c_bus)
    {
        rt_kprintf("find %s failed!\n", CODEC_I2C_DEV_NAME);
        return -RT_ERROR;
    }

    //获取i2s device, 用于codec传输音频数据
    i2s_dev = rt_device_find(CODEC_I2S_DEV_NAME);
    if (!i2s_dev)
    {
        rt_kprintf("find %s failed!\n", CODEC_I2S_DEV_NAME);
    }

    if (RT_EOK != rt_device_open(i2s_dev, RT_DEVICE_OFLAG_RDONLY))
    {
        rt_kprintf("open %s failed!\n", CODEC_I2S_DEV_NAME);
    }

    //配置CODEC使用的DATA_LINE
    i2s_caps.main_type               = AUDIO_TYPE_INPUT;
    i2s_caps.sub_type                = AUDIO_PARM_I2S_DATA_LINE;
    i2s_caps.udata.value             = CODEC_I2S_DATA_LINE;
    rt_device_control(i2s_dev, AUDIO_CTL_CONFIGURE, &i2s_caps);

    //配置I2S samplerate, samplebits, channels
    i2s_caps.main_type               = AUDIO_TYPE_INPUT;
    i2s_caps.sub_type                = AUDIO_DSP_PARAM;
    i2s_caps.udata.config.samplerate = CODEC_I2S_SAMPLERATE;
    if ((i2s_caps.udata.config.samplerate % 44100) == 0) {
        /* clock_aud1 has been configured for 44100*n sample rate*/
        clock_set_i2s_source(CODEC_I2S_CLK_NAME, clk_i2s_src_aud1);
    } else {
        clock_set_i2s_source(CODEC_I2S_CLK_NAME, clk_i2s_src_aud0);
    }
    i2s_caps.udata.config.samplebits = CODEC_I2S_SAMPLEBITS;
    i2s_caps.udata.config.channels   = CODEC_I2S_CHANNEL;
    rt_device_control(i2s_dev, AUDIO_CTL_CONFIGURE, &i2s_caps);

    wm8960_config.route = wm8960_route_record;
    wm8960_config.left_input = wm8960_input_closed;
    wm8960_config.right_input = wm8960_input_differential_mic_input2;
    wm8960_config.play_source = wm8960_play_source_dac;
    wm8960_config.bus = wm8960_bus_left_justified;
    wm8960_config.format.mclk_hz = clock_get_frequency(CODEC_I2S_CLK_NAME);
    wm8960_config.format.sample_rate = CODEC_I2S_SAMPLERATE;
    wm8960_config.format.bit_width = CODEC_I2S_SAMPLEBITS;

    wm8960_control.i2c_bus = i2c_bus;
    wm8960_control.slave_address = WM8960_I2C_ADDR;
    if (wm8960_init(&wm8960_control, &wm8960_config) != status_success) {
        printf("Init Audio Codec failed\n");
    }
    

    if (CODEC_I2S_CHANNEL != i2s_stereo) {
        channel_num = 1;
    } else {
        channel_num = 2;
    }

    //输出wave文件属性
    rt_kprintf("record %ds audio data to wav file:\n", RECORD_TIME_MS/1000);
    rt_kprintf("samplerate: %d\n", CODEC_I2S_SAMPLERATE);
    rt_kprintf("samplebits: %d\n", CODEC_I2S_SAMPLEBITS);
    rt_kprintf("channel number: %d\n", channel_num);

    target_data_size = CODEC_I2S_SAMPLERATE * (CODEC_I2S_SAMPLEBITS / 8U) * channel_num * RECORD_TIME_MS / 1000;

    while (1)
    {
        int length;

        /* 从audio设置读取音频数据  */
        length = rt_device_read(i2s_dev, 0, data_buff, BUFF_SIZE);

        if (length)
        {
            write(fd, data_buff, length);
            data_size += length;
        }

        if (data_size >= target_data_size)
            break;
    }

    /* 重新写入 wav 文件的头 */
    wavheader_init(&header, CODEC_I2S_SAMPLERATE, channel_num, CODEC_I2S_SAMPLEBITS, data_size);
    lseek(fd, 0, SEEK_SET);
    write(fd, &header, sizeof(wav_header_t));
    close(fd);

    //关闭设备
    rt_device_close(i2s_dev);

    __exit:

        if (fd >= 0)
            close(fd);

        return 0;
}
MSH_CMD_EXPORT(codec_recordwav, codec record sound to wav file);

static int codec_playwav(int argc, char *argv[])
{
    int fd = -1;
    uint32_t time;
    wav_header_t *info = NULL;

    rt_device_t i2s_dev;
    struct rt_audio_caps i2s_caps = {0};
    struct rt_i2c_bus_device *i2c_bus;

    wm8960_config_t wm8960_config;
    wm8960_control_t wm8960_control;

    if (argc != 2)
    {
        rt_kprintf("Usage:\n");
        rt_kprintf("codec_playwav /test.wav\n");
        return -RT_ERROR;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        rt_kprintf("open file failed!\n");
        goto __exit;
    }

    info = (wav_header_t *) rt_malloc(sizeof * info);
    if (info == RT_NULL)
        goto __exit;
    if (read(fd, &(info->riff_chunk), sizeof(riff_chunk_t)) <= 0)
        goto __exit;
    if (read(fd, &(info->fmt_chunk),  sizeof(fmt_chunk_t)) <= 0)
        goto __exit;
    if (read(fd, &(info->data_chunk), sizeof(data_chunk_t)) <= 0)
        goto __exit;

    //输出wave文件属性
    time = info->data_chunk.datasize / (info->fmt_chunk.sample_rate * (info->fmt_chunk.bit_per_sample / 8U) * info->fmt_chunk.channels);
    rt_kprintf("wav information:\n");
    rt_kprintf("time: %ds\n", time);
    rt_kprintf("samplerate: %d\n", info->fmt_chunk.sample_rate);
    rt_kprintf("samplebits: %d\n", info->fmt_chunk.bit_per_sample);
    rt_kprintf("channel: %d\n", info->fmt_chunk.channels);

    //获取 i2c_bus device, 控制codec
    i2c_bus = rt_i2c_bus_device_find(CODEC_I2C_DEV_NAME);
    if (!i2c_bus)
    {
        rt_kprintf("find %s failed!\n", CODEC_I2C_DEV_NAME);
        return -RT_ERROR;
    }

    //获取i2s device, 传输音频数据
    i2s_dev = rt_device_find(CODEC_I2S_DEV_NAME);
    if (!i2s_dev)
    {
        rt_kprintf("find %s failed!\n", CODEC_I2S_DEV_NAME);
    }

    if (RT_EOK != rt_device_open(i2s_dev, RT_DEVICE_OFLAG_WRONLY))
    {
        rt_kprintf("open %s failed!\n", CODEC_I2S_DEV_NAME);
    }

    //配置CODEC使用的I2S
    i2s_caps.main_type               = AUDIO_TYPE_OUTPUT;
    i2s_caps.sub_type                = AUDIO_PARM_I2S_DATA_LINE;
    i2s_caps.udata.value             = CODEC_I2S_DATA_LINE;
    rt_device_control(i2s_dev, AUDIO_CTL_CONFIGURE, &i2s_caps);

    i2s_caps.main_type               = AUDIO_TYPE_OUTPUT;
    i2s_caps.sub_type                = AUDIO_DSP_PARAM;
    i2s_caps.udata.config.samplerate = info->fmt_chunk.sample_rate;
    i2s_caps.udata.config.samplebits = info->fmt_chunk.bit_per_sample;
    if (info->fmt_chunk.channels == 1) {
        //单声道选用左声道播放
        i2s_caps.udata.config.channels = i2s_mono_left;
    } else {
        i2s_caps.udata.config.channels   = i2s_stereo;
    }
    if ((info->fmt_chunk.sample_rate % 44100) == 0) {
        /* clock_aud1 has been configured for 44100*n sample rate*/
        clock_set_i2s_source(CODEC_I2S_CLK_NAME, clk_i2s_src_aud1);
    } else {
        clock_set_i2s_source(CODEC_I2S_CLK_NAME, clk_i2s_src_aud0);
    }
    rt_device_control(i2s_dev, AUDIO_CTL_CONFIGURE, &i2s_caps);

    wm8960_config.route = wm8960_route_playback;
    wm8960_config.left_input = wm8960_input_closed;
    wm8960_config.right_input = wm8960_input_differential_mic_input2;
    wm8960_config.play_source = wm8960_play_source_dac;
    wm8960_config.bus = wm8960_bus_left_justified;
    wm8960_config.format.mclk_hz = clock_get_frequency(CODEC_I2S_CLK_NAME);
    wm8960_config.format.sample_rate = info->fmt_chunk.sample_rate;
    wm8960_config.format.bit_width = info->fmt_chunk.bit_per_sample;

    wm8960_control.i2c_bus = i2c_bus;
    wm8960_control.slave_address = WM8960_I2C_ADDR;
    if (wm8960_init(&wm8960_control, &wm8960_config) != status_success) {
        rt_kprintf("Init Audio Codec failed\n");
    }

    while (1)
    {
        int length;

        /* read audio data in wav file */
        length = read(fd, data_buff, BUFF_SIZE);

        if (length <= 0)
            break;

        rt_device_write(i2s_dev, 0, data_buff, length);
    }

    rt_device_close(i2s_dev);

    __exit:

        if (fd >= 0)
            close(fd);

        if (info)
            rt_free(info);

        return 0;
}
MSH_CMD_EXPORT(codec_playwav, codec play wav file);

int main(void)
{
    rt_thread_mdelay(2000);

    //挂载文件系统
    if (dfs_mount("sd", "/", "elm", 0, NULL) == 0)
    {
        rt_kprintf("sd0 mounted to /\n");
    }
    else
    {
        rt_kprintf("sd0 mount to / failed\n");
    }
    return 0;
}

