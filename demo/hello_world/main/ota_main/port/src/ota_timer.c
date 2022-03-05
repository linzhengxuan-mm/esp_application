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
#include "esp_err.h"
#include "esp_timer.h"
#include "ota_mem.h"
#include "ota_timer.h"

static const char *TAG = "ota_timer";

#define OTA_TIMER_INVALID_HANDLE NULL

typedef esp_timer_handle_t ota_timer_handle_t;

typedef struct
{
    ota_timer_handle_t handle;
    _BOOL state;
} ota_timer_item_info_t;

typedef struct
{
	INT32U once_timer_base;
	INT16U once_timer_count;
	INT32U reload_timer_base;
	INT16U reload_timer_count;
	ota_timer_expiry_cb_t expiry_cb;
	ota_timer_item_info_t*timer_table;
}ota_timer_info_cntx_t;


static ota_timer_info_cntx_t ota_timer_info_cntx;


static _BOOL ota_timer_is_reload_timer(INT32U tid)
{
	if ((tid>=ota_timer_info_cntx.reload_timer_base)&&(tid<(ota_timer_info_cntx.reload_timer_base+ota_timer_info_cntx.reload_timer_count)))
	{
		return _TRUE;
	}
	else
	{
		return _FALSE;
	}
}
static ota_timer_item_info_t* ota_timer_find_item_info(INT32U tid)
{
	INT32U index =0;
	ota_timer_item_info_t*timer_item=NULL;
	
	if ((tid<ota_timer_info_cntx.reload_timer_base)
	||(tid>(ota_timer_info_cntx.once_timer_base+ota_timer_info_cntx.once_timer_count)))
	{
		return NULL;
	}

	if ((tid>=ota_timer_info_cntx.reload_timer_base)&&(tid<(ota_timer_info_cntx.reload_timer_base+ota_timer_info_cntx.reload_timer_count)))
	{
		index = tid-ota_timer_info_cntx.reload_timer_base;
		timer_item = ota_timer_info_cntx.timer_table+index;
	}
	else 
	{
		if ((tid>=ota_timer_info_cntx.once_timer_base)&&(tid<(ota_timer_info_cntx.once_timer_base+ota_timer_info_cntx.once_timer_count)))
		{
			index = tid-ota_timer_info_cntx.once_timer_base;
			timer_item = ota_timer_info_cntx.timer_table+ota_timer_info_cntx.reload_timer_count+index;
		}
	}

	return timer_item;
}

static INT32S ota_timer_find_id(ota_timer_handle_t handle)
{
	ota_timer_item_info_t*timer_item=NULL;
	INT32S tid = -1;
	INT16U i = 0;
	INT16U timer_total_num = ota_timer_info_cntx.reload_timer_count+ota_timer_info_cntx.once_timer_count;

	if ((timer_total_num>0)&&(ota_timer_info_cntx.timer_table!=NULL))
	{
		for(i=0;i<timer_total_num;i++)
	    {
	    	timer_item = ota_timer_info_cntx.timer_table+i;
			if (timer_item)
			{
		    	if (timer_item->handle == handle)
		    	{
					if (i<ota_timer_info_cntx.reload_timer_count)
					{
						tid = ota_timer_info_cntx.reload_timer_base+i;
					}
					else
					{
						tid = ota_timer_info_cntx.once_timer_base+(i-ota_timer_info_cntx.reload_timer_count);
					}
				}
			}
	    }
	}

	return tid;
}
static void ota_timer_default_cb(void *arg)
{
    ota_timer_handle_t*handle=(ota_timer_handle_t*)arg;

	if (!handle)
	{
		ESP_LOGE(TAG,"ota_timer_default_cb invalid arg");
		return;
	}

	INT32S tid=ota_timer_find_id(*handle);
	if (tid>0)
	{
		if (ota_timer_info_cntx.expiry_cb)
		{
			ota_timer_info_cntx.expiry_cb((INT32U)tid);
		}
		else
		{
			ESP_LOGW(TAG,"ota_timer_default_cb,no expiry_cb");
		}
	}
	else
	{
		ESP_LOGE(TAG,"ota_timer_default_cb can not find timer id");
	}
}
inv_error_t ota_timer_start(INT32U tid, INT32U dly)
{
	esp_err_t ret = ESP_OK;
	ota_timer_item_info_t*timer_item = ota_timer_find_item_info(tid);

    if((!timer_item)||(timer_item->handle==OTA_TIMER_INVALID_HANDLE))
    {
		ESP_LOGE(TAG,"ota_timer_start,unavailable timer id [%d]",tid);
        return -1;
    }

    esp_timer_stop(timer_item->handle);
	if(ota_timer_is_reload_timer(tid))
	{
		ret = esp_timer_start_periodic(timer_item->handle,dly*1000);//us
	}
	else
	{
		ret = esp_timer_start_once(timer_item->handle,dly*1000);//us
	}

    ESP_LOGI(TAG,"[+TIMER:%d] status[%d] delay = %d", tid, ret, dly);
	if (ret==ESP_OK)
	{
    	timer_item->state = _TRUE;
		return 0;
	}
	else
	{
		ESP_LOGE(TAG,"timer start fail, timer id [%d]", tid);
		return -1;
	}
}

inv_error_t ota_timer_stop(INT32U tid)
{
	esp_err_t ret = ESP_OK;
	ota_timer_item_info_t*timer_item = ota_timer_find_item_info(tid);

    if((!timer_item)||(timer_item->handle==OTA_TIMER_INVALID_HANDLE))
    {
		ESP_LOGE(TAG,"ota_timer_stop,unavailable timer id [%d]", tid);
        return -1;
    }
    ret = esp_timer_stop(timer_item->handle);
	timer_item->state = _FALSE;
    ESP_LOGI(TAG,"[-TIMER] status[%d]", tid);
	return 0;
}
inv_error_t ota_timer_clear(INT32U tid)
{
	ota_timer_item_info_t*timer_item = ota_timer_find_item_info(tid);

    if((!timer_item)||(timer_item->handle==OTA_TIMER_INVALID_HANDLE))
    {
		ESP_LOGE(TAG,"ota_timer_clear,unavailable timer id [%d]", tid);
        return -1;
    }
	timer_item->state = _FALSE;
	return 0;
}

_BOOL ota_timer_running(INT32U tid)
{
	ota_timer_item_info_t*timer_item = ota_timer_find_item_info(tid);

    if((!timer_item)||(timer_item->handle==OTA_TIMER_INVALID_HANDLE))
    {
		ESP_LOGE(TAG,"ota_timer_running,unavailable timer id [%d]", tid);
        return _FALSE;
    }
    return timer_item->state;
}

_BOOL ota_timer_exist(INT16U tid)
{
	ota_timer_item_info_t*timer_item = ota_timer_find_item_info(tid);
	if (timer_item)
	{
		return _TRUE;
	}
	else
	{
		return _FALSE;
	}
}
inv_error_t ota_timer_init(INT32U reload_timer_base,
					 	INT16U reload_timer_count,
						INT32U once_timer_base,
					 	INT16U once_timer_count,
					 	ota_timer_expiry_cb_t expiry_cb)
{
	INT16U i = 0;
	esp_timer_create_args_t timer_args;
	ota_timer_item_info_t*timer_item=NULL;
	INT16U timer_total_num = reload_timer_count+once_timer_count;
	
	memset(&ota_timer_info_cntx,0x00,sizeof(ota_timer_info_cntx_t));
	if ((timer_total_num== 0)||(once_timer_base<(reload_timer_base+reload_timer_count)))
	{
		ESP_LOGE(TAG,"ota timer total num zero");
		return -1;
	}

	ESP_LOGI(TAG,"timer_total_num:%d,reload_timer_count:%d,once_timer_count:%d",timer_total_num,reload_timer_count,once_timer_count);
	ota_timer_info_cntx.reload_timer_base= reload_timer_base;
	ota_timer_info_cntx.reload_timer_count= reload_timer_count;
	ota_timer_info_cntx.once_timer_base = once_timer_base;
	ota_timer_info_cntx.once_timer_count= once_timer_count;
	ota_timer_info_cntx.expiry_cb = expiry_cb;

	ota_timer_info_cntx.timer_table=(ota_timer_item_info_t*)ota_malloc(sizeof(ota_timer_item_info_t)*timer_total_num,0);
	if(!ota_timer_info_cntx.timer_table)
	{
		ESP_LOGE(TAG,"ota timer table mem alloc fail");
		return -1;
	}

	for(i=0;i<timer_total_num;i++)
	{
		timer_item = ota_timer_info_cntx.timer_table+i;
		if(timer_item)
		{
			memset(&timer_args,0x00,sizeof(esp_timer_create_args_t));
			timer_args.callback = ota_timer_default_cb;
			timer_args.arg = &(timer_item->handle);
			timer_args.name = "ota timer";

			ESP_ERROR_CHECK(esp_timer_create(&timer_args, &(timer_item->handle)));
		}
	}

	return 0;
}
void ota_timer_deinit(void)
{
	ota_timer_item_info_t*timer_item=NULL;
	INT16U i = 0;
	INT16U timer_total_num = ota_timer_info_cntx.reload_timer_count+ota_timer_info_cntx.once_timer_count;

	if ((timer_total_num>0)&&(ota_timer_info_cntx.timer_table!=NULL))
	{
		for(i=0;i<timer_total_num;i++)
	    {
			timer_item = ota_timer_info_cntx.timer_table+i;
			if(timer_item)
			{
		    	esp_timer_stop(timer_item->handle);
				esp_timer_delete(timer_item->handle);
			}
	    }
		ota_mfree(ota_timer_info_cntx.timer_table);
	}

	memset(&ota_timer_info_cntx,0x00,sizeof(ota_timer_info_cntx_t));
}
