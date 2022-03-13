/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "ota_main.h"

static int ota_main_modem_received_ind(void *buffer, unsigned int len)
{
	if((!buffer)||(len==0))
	{
		return -1;
	}
	OTA_LOGI("len:%d",len);
	
	OTA_LOG_BUFFER_CHAR(buffer,len);
	return 0;
}
static void ota_main_modem_event_ind(ota_modem_event_type event)
{
	switch(event)
	{
		case OTA_MODEM_EVENT_POWER_UP:
			OTA_LOGI("modem power up");
			OTA_MSG_SEND_TO_MAIN(MID_OTA_MODEM_POWER_UP,NULL,0);
			break;
		case OTA_MODEM_EVENT_POWER_DOWN:
			OTA_LOGI("modem power down");
			break;
		case OTA_MODEM_EVENT_COM_ERROR:
			OTA_LOGE("modem communicate error!");
			break;
		default:
			OTA_LOGW("unknown modem event!");
			break;
	}
}
void ota_main_init(void)
{
	ota_log_init();
	ota_timer_handler_init();
	
	ota_modem_init();
	ota_modem_register_callback(ota_main_modem_received_ind,ota_main_modem_event_ind);

	ota_msg_init();
	ota_main_thread_init();
}
