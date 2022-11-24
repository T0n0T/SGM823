/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-24     liwentai       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "sgm823.h"

#define WDT_DEVICE_NAME    "wdt"    /* 看门狗设备名称 */
#define WDT_PIN             0       /* 喂狗引脚, PA0 */

static rt_device_t wdg_dev;         /* 看门狗设备句柄 */

static void idle_hook(void)
{
    /* 在空闲线程的回调函数里喂狗 */
    rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
}

static int wdt_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    char device_name[RT_NAME_MAX];

    if (argc == 2)
    {
        rt_strncpy(device_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(device_name, WDT_DEVICE_NAME, RT_NAME_MAX);
    }
    wdg_dev = rt_device_find(device_name);
    if (!wdg_dev)
    {
        rt_kprintf("find %s failed!\n", device_name);
        return RT_ERROR;
    }

    ret = rt_device_init(wdg_dev);
    if (ret != RT_EOK)
    {
        rt_kprintf("%s init failed!\n", device_name);
        return RT_ERROR;
    }

    /* 设置空闲线程回调函数 */
    rt_thread_idle_sethook(idle_hook);

    rt_kprintf("start watchdog device\n");

    return ret;
}
MSH_CMD_EXPORT(wdt_sample, wdt sample);

static int rt_hw_sgm823_port(void)
{
    rt_hw_sgm823_init(WDT_DEVICE_NAME, WDT_PIN);

    return RT_EOK;
}
/* 注册看门狗设备 */
INIT_COMPONENT_EXPORT(rt_hw_sgm823_port);
