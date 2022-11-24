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

#define PKG_USING_SGM823
#ifdef PKG_USING_SGM823

#define DBG_TAG "sgm823"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

static rt_err_t sgm823_feed(rt_watchdog_t *wdt)
{
    rt_base_t pin = 0;

    pin = (rt_base_t)wdt->parent.user_data;
    if (PIN_LOW == rt_pin_read(pin))
    {
        rt_pin_write(pin, PIN_HIGH);
    }
    else
    {
        rt_pin_write(pin, PIN_LOW);
    }
    return RT_EOK;
}

static rt_err_t sgm823_init(rt_watchdog_t *wdt)
{
    rt_base_t pin = 0;

    pin = (rt_base_t)wdt->parent.user_data;
    rt_pin_mode(pin, PIN_MODE_OUTPUT);
    rt_pin_write(pin, PIN_LOW); /* 初始化引脚后即启动sgm823看门狗，且不可停止 */
    return RT_EOK;
}

static rt_err_t sgm823_control(rt_watchdog_t *wdt, int cmd, void *args)
{
    rt_uint8_t  *p = RT_NULL;

    switch (cmd)
    {
    case RT_DEVICE_CTRL_WDT_GET_TIMEOUT:
        p = (rt_uint8_t*)args;
        *p = 16;    /* sgm823为 1.6s溢出时间 */
        break;
    case RT_DEVICE_CTRL_WDT_KEEPALIVE:
        sgm823_feed(wdt);
        break;
    default:
        return RT_ERROR;
    }
    return RT_EOK;
}

static struct rt_watchdog_ops sgm823_ops =
    {
        sgm823_init,
        sgm823_control
    };

static rt_watchdog_t sgm823_iwg =
    {
        .ops = &sgm823_ops
    };

int rt_hw_sgm823_init(const char*name, rt_base_t pin)
{
    return rt_hw_watchdog_register(&sgm823_iwg, name, RT_DEVICE_FLAG_DEACTIVATE, (void*)pin);
}

#endif
