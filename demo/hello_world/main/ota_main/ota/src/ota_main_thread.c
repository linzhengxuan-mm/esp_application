/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "ota_main.h"

#define OTA_MAIN_THREAD_NAME			"ota_main_thread"
#define OTA_MAIN_THREAD_STACK_SIZE   	16*1024
#define OTA_MAIN_THREAD_PRIORITIES   	OTA_SYS_THREAD_BASE_PRIORITIES+1
#define OTA_MAIN_THREAD_QUEUE_NUM   	128

static void ota_main_thread_entry(void*arg)
{
	ota_ilm_msg_t msg;
	
	while(1)
	{
		memset(&msg,0x00,sizeof(ota_ilm_msg_t));			
		if (ota_msg_receive(MODE_OTA_MAIN,&msg)==OTA_OK)
		{
			switch(msg.msg_id)
			{
				case MID_OTA_MODEM_POWER_UP:
					ota_modem_send_data("AT+CPIN?\r\n",strlen("AT+CPIN?\r\n"));
					break;
				default:
					break;
			}

			if (msg.param!=NULL)
			{
				ota_mem_free(msg.param);
			}
		}
		else
		{
			OTA_LOGE("never run into here!");
			break;
		}
	}
	
	ota_msg_delete_handle(MODE_OTA_MAIN);
	ota_sys_thread_delete(NULL);
}

ota_err_t ota_main_thread_init(void)
{
	if (ota_msg_create_handle(MODE_OTA_MAIN,OTA_MAIN_THREAD_QUEUE_NUM)!=OTA_OK)
	{
		OTA_LOGE("ota main thread msg handle create fail");
		OTA_SYS_ASSERT(0);
		return OTA_FAIL;
	}

	if(ota_sys_thread_create(ota_main_thread_entry,
						OTA_MAIN_THREAD_NAME,
						OTA_MAIN_THREAD_STACK_SIZE,
						NULL,
						OTA_MAIN_THREAD_PRIORITIES,
						NULL)!=OTA_OK)
	{
		OTA_LOGE("ota main thread create fail");
		OTA_SYS_ASSERT(0);
		return OTA_FAIL;
	}
	
	return OTA_OK;
}
