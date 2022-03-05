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
#ifndef __OTA_TIMER_H__
#define __OTA_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "pub_datatype.h"

typedef void (* ota_timer_expiry_cb_t) (INT32U tid);

inv_error_t ota_timer_start(INT32U tid, INT32U dly);
inv_error_t ota_timer_stop(INT32U tid);
inv_error_t ota_timer_clear(INT32U tid);
_BOOL ota_timer_running(INT32U tid);
_BOOL ota_timer_exist(INT16U tid);
inv_error_t ota_timer_init(INT32U reload_timer_base,
					 	INT16U reload_timer_count,
						INT32U once_timer_base,
					 	INT16U once_timer_count,
					 	ota_timer_expiry_cb_t expiry_cb);
void ota_timer_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
