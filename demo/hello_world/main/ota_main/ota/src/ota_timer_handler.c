/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "pub_datatype.h"
#include "ota_log.h"
#include "ota_timer.h"
#include "ota_timer_handler.h"

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
ota_err_t ota_timer_handler_init(void)
{
	inv_error_t ret = -1;
	
	ret = ota_timer_init(TID_GT_TIMER_RELOAD_BASE,
						(TID_GT_TIMER_RELOAD_MAX-TID_GT_TIMER_RELOAD_BASE),
						TID_GT_TIMER_ONCE_BASE,
						(TID_GT_TIMER_ONCE_MAX-TID_GT_TIMER_ONCE_BASE),
						ota_timer_handler_callback);
	if(ret<0)
	{
		OTA_LOGE("init fail");
		return OTA_FAIL;
	}
	else
	{
		return OTA_OK;
	}
}
