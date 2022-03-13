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
#ifndef __OTA_MAIN_H__
#define __OTA_MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "pub_datatype.h"
#include "pub_enum.h"
#include "ota_timer.h"
#include "ota_mem.h"
#include "ota_fs.h"
#include "ota_log.h"
#include "ota_system.h"
#include "ota_modem.h"
#include "sscanf.h"
#include "vsnprintf.h"
#include "ota_constant.h"
#include "ota_msg.h"
#include "ota_timer_handler.h"

#define ota_sscanf          _ota_sscanf
#define ota_sprintf         _ota_sprintf
#define ota_wsprintf        _ota_sprintf

void ota_main_init(void);
ota_err_t ota_main_thread_init(void);

#ifdef __cplusplus
}
#endif
#endif
