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
#ifndef __OTA_SYSTEM_H__
#define __OTA_SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"


#define OTA_SYS_TIME_BLOCKING  			0xffffffff
#define OTA_SYS_THREAD_BASE_PRIORITIES 	10

#define OTA_SYS_INVALID_HANDLE  		NULL

#define OTA_SYS_ASSERT(_e)  assert(_e)

typedef struct  
{
	int m_sec; /* Seconds. [0-60] (1 leap second) */
	int m_min; /* Minutes. [0-59] */
	int m_hour; /* Hours. [0-23] */
	int m_mday; /* Day. [1-31] */
	int m_mon; /* Month. [0-11] */
	int m_year; /* Year*/
	int m_wday; /* Day of week. [0-6] */
}ota_sys_time;

typedef TaskFunction_t OtaTreadFunction;

typedef TaskHandle_t OtaTreadHandle;
typedef QueueHandle_t OtaQueueHandle;
typedef SemaphoreHandle_t OtaSemaphoreHandle;
typedef SemaphoreHandle_t OtaMutexHandle;
typedef EventGroupHandle_t OtaEventGroupHandle;

void ota_sys_restart(void);
void ota_sys_delay(int ms);


void ota_sys_enter_deep_sleep(void);
void ota_sys_lock_sleep(void);
void ota_sys_unlock_sleep(void);

int ota_sys_time_stamp(void);
int ota_sys_get_time(ota_sys_time*cur_time);
int ota_sys_set_time(ota_sys_time*cur_time);

int ota_sys_thread_create(OtaTreadFunction thread_entry,
							const char * const pcName,
							const unsigned int StackSize,
							void * const pvParameters,
							unsigned int Priority,
							OtaTreadHandle*pthread_handle);
void ota_sys_thread_delete(OtaTreadHandle thread_handle);

OtaQueueHandle ota_sys_queue_create(unsigned int queue_len, unsigned int item_size);
int ota_sys_queue_delete(OtaQueueHandle queue);
int ota_sys_queue_send(OtaQueueHandle queue, void *item, unsigned int block_time_ms);
int ota_sys_queue_send_to_back(OtaQueueHandle queue, void *item, unsigned int block_time_ms);
int ota_sys_queue_send_to_front(OtaQueueHandle queue, void *item, unsigned int block_time_ms);
int ota_sys_queue_recv(OtaQueueHandle queue, void *item, unsigned int block_time_ms);

OtaSemaphoreHandle ota_sys_semphr_create(unsigned int max, unsigned int init);
void ota_sys_semphr_delete(OtaSemaphoreHandle semphr);
int ota_sys_semphr_take(OtaSemaphoreHandle semphr, unsigned int block_time_ms);
int ota_sys_semphr_give(OtaSemaphoreHandle semphr);

OtaMutexHandle ota_sys_recursive_mutex_create(void);
OtaMutexHandle ota_sys_mutex_create(void);
void ota_sys_mutex_delete(OtaMutexHandle mutex);
int IRAM_ATTR ota_sys_mutex_lock(OtaMutexHandle mutex);
int IRAM_ATTR ota_sys_mutex_unlock(OtaMutexHandle mutex);

OtaEventGroupHandle ota_sys_event_group_create(void);
unsigned int ota_sys_event_group_wait_bits(OtaEventGroupHandle EventGroup, unsigned int bits_to_wait_for, int clear_on_exit, int wait_for_all_bits, unsigned int block_time_ms);
unsigned int ota_sys_event_group_clear_bits(OtaEventGroupHandle EventGroup,unsigned int bits_to_clear);
unsigned int ota_sys_event_group_set_bits(OtaEventGroupHandle EventGroup,unsigned int bits_to_set);
void ota_sys_event_group_delete(OtaEventGroupHandle EventGroup);

#ifdef __cplusplus
}
#endif
#endif

