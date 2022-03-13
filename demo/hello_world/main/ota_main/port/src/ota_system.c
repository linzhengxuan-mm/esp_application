/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "unity.h"
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "sdkconfig.h"
#include "esp_system.h"
#include "ota_log.h"
#include "ota_system.h"
#include "board.h"

#define OTA_SYS_TIME_BASE_YEAR  1900

/////////////////////////////////system interface////////////////////////////////////////////////////////
void ota_sys_restart(void)
{
	esp_restart();
}
void ota_sys_delay(int ms)
{
	vTaskDelay(pdMS_TO_TICKS(ms));
}


/////////////////////////////////system sleep////////////////////////////////////////////////////////
void ota_sys_enter_deep_sleep(void)
{
	board_enter_deep_sleep();
}
void ota_sys_lock_sleep(void)
{
	board_lock_sleep();
}
void ota_sys_unlock_sleep(void)
{
	board_unlock_sleep();
}


/////////////////////////////////system time////////////////////////////////////////////////////////
int ota_sys_time_stamp(void)
{
	struct timeval tv={0};

	gettimeofday(&tv, NULL);
	
	return tv.tv_sec;
}
int ota_sys_get_time(ota_sys_time*cur_time)
{
	struct timeval tv={0};
	struct tm timeinfo={0};
    static _lock_t bufferLock = 0;

	
	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &timeinfo);

	_lock_acquire(&bufferLock);
	cur_time->m_sec 	= timeinfo.tm_sec;
	cur_time->m_min 	= timeinfo.tm_min;
	cur_time->m_hour 	= timeinfo.tm_hour;
	cur_time->m_mday 	= timeinfo.tm_mday;
	cur_time->m_mon 	= timeinfo.tm_mon;
	cur_time->m_year 	= timeinfo.tm_year+OTA_SYS_TIME_BASE_YEAR;
	cur_time->m_wday 	= timeinfo.tm_wday;
	_lock_release(&bufferLock);

	return 0;
}
int ota_sys_set_time(ota_sys_time*cur_time)
{
    struct tm tm = { 0 };
    tm.tm_year = cur_time->m_year- OTA_SYS_TIME_BASE_YEAR;
    //tm.tm_wday = cur_time->m_wday;
    tm.tm_mon = cur_time->m_mon;
    tm.tm_mday = cur_time->m_mday;
    tm.tm_hour = cur_time->m_hour;
    tm.tm_min = cur_time->m_min;
    tm.tm_sec = cur_time->m_sec;
	
    time_t t = mktime(&tm);
    struct timeval now = { .tv_sec = t };
    settimeofday(&now, NULL);

	return 0;
}


/////////////////////////////////thread////////////////////////////////////////////////////////
int ota_sys_thread_create(OtaTreadFunction thread_entry,
							const char * const pcName,
							const unsigned int StackSize,
							void * const pvParameters,
							unsigned int Priority,
							OtaTreadHandle*pthread_handle)
{
	BaseType_t ret = pdFAIL;
	
    ret = xTaskCreatePinnedToCore((TaskFunction_t)thread_entry,
			                            pcName,
			                            StackSize,
			                            pvParameters,
			                            Priority,
			                            (TaskHandle_t*)pthread_handle,
			                            xPortGetCoreID());

	return ((ret!=pdPASS)? -1:0);
}
void ota_sys_thread_delete(OtaTreadHandle thread_handle)
{
	vTaskDelete((TaskHandle_t)thread_handle);
}


/////////////////////////////////queue////////////////////////////////////////////////////////
OtaQueueHandle ota_sys_queue_create(unsigned int queue_len, unsigned int item_size)
{
    return xQueueCreate(queue_len, item_size);
}
int ota_sys_queue_delete(OtaQueueHandle queue)
{
	vQueueDelete(queue);
	return 0;
}
int ota_sys_queue_send(OtaQueueHandle queue, void *item, unsigned int block_time_ms)
{
	BaseType_t ret = pdFALSE;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	
	if (xPortInIsrContext()) 
	{
		ret = xQueueSendFromISR(queue, item, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken==pdPASS)
		{
			portYIELD_FROM_ISR();
		}
	}
	else
	{
		if (block_time_ms == OTA_SYS_TIME_BLOCKING)
		{
			ret = xQueueSend(queue, item, portMAX_DELAY);
		} 
		else 
		{
			ret = xQueueSend(queue, item, pdMS_TO_TICKS(block_time_ms));
		}
	}
	
	return ((ret!=pdPASS)?-1:0);
}
int ota_sys_queue_send_to_back(OtaQueueHandle queue, void *item, unsigned int block_time_ms)
{
    BaseType_t ret = xQueueGenericSend(queue, item, pdMS_TO_TICKS(block_time_ms), queueSEND_TO_BACK);
	
	return ((ret!=pdPASS)?-1:0);
}
int ota_sys_queue_send_to_front(OtaQueueHandle queue, void *item, unsigned int block_time_ms)
{
    BaseType_t ret = xQueueGenericSend(queue, item, pdMS_TO_TICKS(block_time_ms), queueSEND_TO_FRONT);
	
	return ((ret!=pdPASS)?-1:0);
}
int ota_sys_queue_recv(OtaQueueHandle queue, void *item, unsigned int block_time_ms)
{
	BaseType_t ret = pdFALSE;

	if (block_time_ms == OTA_SYS_TIME_BLOCKING)
	{
		ret = xQueueReceive(queue, item, portMAX_DELAY);
	}
	else 
	{
		ret = xQueueReceive(queue, item, pdMS_TO_TICKS(block_time_ms));
	}
	
	return ((ret!=pdPASS)?-1:0);
}

/////////////////////////////////semphr////////////////////////////////////////////////////////
OtaSemaphoreHandle ota_sys_semphr_create(unsigned int max, unsigned int init)
{
    return xSemaphoreCreateCounting(max, init);
}
void ota_sys_semphr_delete(OtaSemaphoreHandle semphr)
{
    vSemaphoreDelete(semphr);
}
int ota_sys_semphr_take(OtaSemaphoreHandle semphr, unsigned int block_time_ms)
{
	BaseType_t ret = pdFALSE;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;


	if (xPortInIsrContext()) 
	{
		ret = xSemaphoreTakeFromISR(semphr, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken) 
		{
			portYIELD_FROM_ISR();
		}
	}
	else
	{
	    if (block_time_ms == OTA_SYS_TIME_BLOCKING) 
		{
	        ret = xSemaphoreTake(semphr, portMAX_DELAY);
	    } 
		else 
		{
	        ret = xSemaphoreTake(semphr, pdMS_TO_TICKS(block_time_ms));
	    }
	}

	return ((ret!=pdPASS)?-1:0);
}
int ota_sys_semphr_give(OtaSemaphoreHandle semphr)
{
	BaseType_t ret = pdFALSE;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (xPortInIsrContext()) 
	{
		ret = xSemaphoreGiveFromISR(semphr, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken) 
		{
			portYIELD_FROM_ISR();
		}
	}
	else
	{
    	ret = xSemaphoreGive(semphr);
	}

	return ((ret!=pdPASS)?-1:0);
}

/////////////////////////////////mutex////////////////////////////////////////////////////////
OtaMutexHandle ota_sys_recursive_mutex_create(void)
{
    return xSemaphoreCreateRecursiveMutex();
}
OtaMutexHandle ota_sys_mutex_create(void)
{
    return xSemaphoreCreateMutex();
}
void ota_sys_mutex_delete(OtaMutexHandle mutex)
{
    vSemaphoreDelete(mutex);
}
int IRAM_ATTR ota_sys_mutex_lock(OtaMutexHandle mutex)
{
    BaseType_t ret = xSemaphoreTakeRecursive(mutex, portMAX_DELAY);

	return ((ret!=pdPASS)?-1:0);
}
int IRAM_ATTR ota_sys_mutex_unlock(OtaMutexHandle mutex)
{
    BaseType_t ret = xSemaphoreGiveRecursive(mutex);

	return ((ret!=pdPASS)?-1:0);
}

/////////////////////////////////event group////////////////////////////////////////////////////////
OtaEventGroupHandle ota_sys_event_group_create(void)
{
	return xEventGroupCreate();	
}
unsigned int ota_sys_event_group_wait_bits(OtaEventGroupHandle EventGroup, unsigned int bits_to_wait_for, int clear_on_exit, int wait_for_all_bits, unsigned int block_time_ms)
{
	if (block_time_ms == OTA_SYS_TIME_BLOCKING)
	{
		return (unsigned int)xEventGroupWaitBits(EventGroup, bits_to_wait_for, clear_on_exit, wait_for_all_bits, portMAX_DELAY);
	}
	else
	{
		return (unsigned int)xEventGroupWaitBits(EventGroup, bits_to_wait_for, clear_on_exit, wait_for_all_bits, pdMS_TO_TICKS(block_time_ms));
	}
}
unsigned int ota_sys_event_group_clear_bits(OtaEventGroupHandle EventGroup,unsigned int bits_to_clear)
{
	return (unsigned int)xEventGroupClearBits(EventGroup,bits_to_clear);
}
unsigned int ota_sys_event_group_set_bits(OtaEventGroupHandle EventGroup,unsigned int bits_to_set)
{
	return (unsigned int)xEventGroupSetBits(EventGroup,bits_to_set);
}
void ota_sys_event_group_delete(OtaEventGroupHandle EventGroup)
{
	vEventGroupDelete(EventGroup);
}
