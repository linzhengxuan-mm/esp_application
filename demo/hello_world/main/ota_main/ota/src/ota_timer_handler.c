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

#include "ota_main.h"

static const char *TAG = "ota_timer_handler";

void ota_timer_handler_callback(INT32U tid)
{
    if( tid != TID_GT_MAIN_TIMER)
    {
        ota_timer_clear(tid);
    }

    switch(tid)
    {
		case TID_GT_MAIN_TIMER:
			break;
		default:
			break;
	}
}
void ota_timer_handler_init(void)
{
	inv_error_t ret = -1;
	
	ret = ota_timer_init(TID_GT_TIMER_RELOAD_BASE,
						(TID_GT_TIMER_RELOAD_MAX-TID_GT_TIMER_RELOAD_BASE),
						TID_GT_TIMER_ONCE_BASE,
						(TID_GT_TIMER_ONCE_MAX-TID_GT_TIMER_ONCE_BASE),
						ota_timer_handler_callback);
	if(ret<0)
	{
		ESP_LOGE(TAG,"ota_timer_handler_init fail");
	}
	else
	{
		ota_timer_start(TID_GT_MAIN_TIMER, OTA_TIMER_DELAY_MAIN);
	}
}
