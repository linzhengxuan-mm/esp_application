/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "pub_datatype.h"
#include "pub_enum.h"
#include "ota_mem.h"
#include "ota_log.h"
#include "ota_msg.h"

#define OTA_MAIN_THREAD_QUEUE_ITEM_SIZE   	sizeof(ota_ilm_msg_t)
static ota_msg_handle s_ota_msg_handle_list[MODE_OTA_MAX_NUM];

static ota_msg_handle otg_msg_get_handle(ota_mode_id mode)
{
	if (mode>=MODE_OTA_MAX_NUM)
	{
		return OTA_INVALID_MSG_HANDLE;
	}
	else
	{
		return s_ota_msg_handle_list[mode];
	}
}
ota_err_t ota_msg_create_handle(ota_mode_id mode,INT16U msg_num)
{
	if (mode>=MODE_OTA_MAX_NUM)
	{
		return OTA_FAIL;
	}
	
	if (s_ota_msg_handle_list[mode] == OTA_INVALID_MSG_HANDLE)
	{
		s_ota_msg_handle_list[mode] = ota_sys_queue_create(msg_num,OTA_MAIN_THREAD_QUEUE_ITEM_SIZE);
		if (s_ota_msg_handle_list[mode]!=OTA_INVALID_MSG_HANDLE)
		{
			return OTA_OK;
		}
		else
		{
			return OTA_FAIL;
		}
	}
	else
	{
		return OTA_OK;
	}
}
ota_err_t ota_msg_delete_handle(ota_mode_id mode)
{
	ota_err_t ret = OTA_FAIL;
	
	if (mode>=MODE_OTA_MAX_NUM)
	{
		return OTA_FAIL;
	}
	
	if (s_ota_msg_handle_list[mode] != OTA_INVALID_MSG_HANDLE)
	{
		ret = ota_sys_queue_delete(s_ota_msg_handle_list[mode]);
		if (ret==OTA_OK)
		{
			s_ota_msg_handle_list[mode] = OTA_INVALID_MSG_HANDLE;
		}
		return ret;
	}
	else
	{
		return OTA_OK;
	}
}
ota_err_t ota_msg_receive(ota_mode_id mode,ota_ilm_msg_t*msg)
{
	ota_msg_handle msg_handle=OTA_INVALID_MSG_HANDLE;

	msg_handle = otg_msg_get_handle(mode);
	if (msg_handle == OTA_INVALID_MSG_HANDLE) 
	{
		OTA_LOGE("msg handle is null");
		return OTA_FAIL;
	}

	return ota_sys_queue_recv(msg_handle,msg, OTA_SYS_TIME_BLOCKING);
}
ota_err_t ota_msg_init(void)
{
	int i=0;

	for (i=0;i<MODE_OTA_MAX_NUM;i++)
	{
		s_ota_msg_handle_list[i] = OTA_INVALID_MSG_HANDLE;
	}
	return OTA_OK;
}
ota_err_t ota_msg_post_to(ota_mode_id mode,ota_message_id msg_id, void *param, INT32U parma_len)
{ 
	ota_ilm_msg_t msg;
	ota_err_t ret;
	ota_msg_handle msg_handle=OTA_INVALID_MSG_HANDLE;

	msg_handle = otg_msg_get_handle(mode);
	if (msg_handle == OTA_INVALID_MSG_HANDLE) 
	{
		OTA_LOGE("send msg fail,msg handle is null");
		return OTA_FAIL;
	} 
	msg.msg_id = msg_id;  
	if ((param)&&(parma_len>0))
	{
		msg.param = ota_malloc(parma_len+1,0);
		if(msg.param)
		{
			memcpy(msg.param,param,parma_len);
			msg.param_len = parma_len;
		}
		else
		{
			OTA_LOGE("send msg fail,param malloc fail");
			return OTA_FAIL;
		}
	}
	else
	{
		msg.param = NULL;
		msg.param_len = 0;
	}
	
	ret = ota_sys_queue_send(msg_handle,&msg,100);
	if (ret != OTA_OK)
	{
		OTA_LOGE("send msg fail");
		if (msg.param)
		{
			ota_mfree(msg.param);
		}
	}
	
	return ret;
}
