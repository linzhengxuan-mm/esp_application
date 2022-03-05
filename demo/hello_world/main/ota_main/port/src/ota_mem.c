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
** ����: host_malloc
** ����: �����ڴ�, ������host_mfree���ͷ�.
** ����: s ��ʾ��Ҫ������ڴ��ֽڳ���.
**       v ��ʾ�����������ڴ������ʼ��Ϊ��ֵ.
** ����: �������뵽���ڴ��ַ.
** ����: wangqingxiang
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
** ����: host_mfree
** ����: �ͷ��ڴ�, �Ƿ���host_malloc���������ڴ�.
** ����: p   ��ʾ��Ҫ�ͷŵ��ڴ��ַ.
** ����: ��
** ����: wangqingxiang
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
