/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "ota_mem.h"

void ota_mem_init(void)
{

}
void ota_mem_deinit(void)
{

}
/*******************************************************************************
** 函数: host_malloc
** 功能: 申请内存, 必须由host_mfree来释放.
** 参数: s 表示需要申请的内存字节长度.
**       v 表示将申请来的内存区间初始化为该值.
** 返回: 返回申请到的内存地址.
** 作者: wangqingxiang
*******/
void *ota_mem_alloc(INT32U s, INT8U v)
{
    void *memptr = NULL;
    if (s)
    {
        memptr = calloc(1, s);
        if (memptr)
        {
            if(v != '?')
            {
                memset(memptr, v, s);
            }
        }
    }

    return memptr;
}


/*******************************************************************************
** 函数: host_mfree
** 功能: 释放内存, 是否由host_malloc申请来的内存.
** 参数: p   表示需要释放的内存地址.
** 返回: 无
** 作者: wangqingxiang
*******/
void ota_mem_free(void *p)
{
    free(p);
}

void *ota_vfs_alloc(INT32U s)
{
    void *memptr = NULL;
    if (s)
    {
        memptr = calloc(1, s);
        if(memptr)
        {
            memset(memptr, 0 , s);
        }
    }
    return memptr;
}
void ota_vfs_free(void *p)
{
    free(p);
}
