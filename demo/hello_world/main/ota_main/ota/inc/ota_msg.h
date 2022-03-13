// Copyright 2018 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef __OTA_MSG_H__
#define __OTA_MSG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "pub_datatype.h"
#include "ota_system.h"

#define OTA_INVALID_MSG_HANDLE 		OTA_SYS_INVALID_HANDLE
typedef OtaQueueHandle ota_msg_handle;

typedef struct 
{
	INT16U msg_id;
	INT16U param_len;
	void *param;
} ota_ilm_msg_t;

typedef enum
{
	MID_OTA_NONE=0,
	MID_OTA_MODEM_POWER_UP,

	MID_OTA_MAX_NUM,
}ota_message_id;


typedef enum
{
	MODE_OTA_MAIN=0,

	MODE_OTA_MAX_NUM,
}ota_mode_id;

#define OTA_MSG_SEND_TO_MAIN(id,param,len) ota_msg_post_to(MODE_OTA_MAIN,id,param,len)

ota_err_t ota_msg_create_handle(ota_mode_id mode,INT16U msg_num);
ota_err_t ota_msg_delete_handle(ota_mode_id mode);
ota_err_t ota_msg_receive(ota_mode_id mode,ota_ilm_msg_t*msg);
ota_err_t ota_msg_post_to(ota_mode_id mode,ota_message_id msg_id, void *param, INT32U parma_len);
ota_err_t ota_msg_init(void);
#ifdef __cplusplus
}
#endif

#endif
