/* Console example — NVS commands

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_err.h"
#include "esp_event.h"

ESP_EVENT_DEFINE_BASE(COMMON_EVENT);

static esp_event_loop_handle_t common_event_loop_handle=NULL;

esp_err_t common_event_register(int32_t event_id, esp_event_handler_t event_handler)
{
    if (!common_event_loop_handle) 
	{
        return ESP_FAIL;
    }
	
    return esp_event_handler_register_with(common_event_loop_handle, COMMON_EVENT, event_id, event_handler, NULL);
}

esp_err_t common_event_unregister(int32_t event_id, esp_event_handler_t event_handler)
{
    if (!common_event_loop_handle) 
	{
        return ESP_FAIL;
    }
	
    return esp_event_handler_unregister_with(common_event_loop_handle, COMMON_EVENT, event_id, event_handler);
}

esp_err_t common_event_post(int32_t event_id,void* event_data, size_t event_data_size, TickType_t ticks_to_wait)
{
    if (!common_event_loop_handle) 
	{
        return ESP_FAIL;
    }
    return esp_event_post_to(common_event_loop_handle, COMMON_EVENT, event_id, event_data, event_data_size, ticks_to_wait);
}

#if CONFIG_ESP_EVENT_POST_FROM_ISR
esp_err_t common_event_isr_post(int32_t event_id,void* event_data, size_t event_data_size, BaseType_t* task_unblocked)
{
    if (!common_event_loop_handle) 
	{
        return ESP_FAIL;
    }
    return esp_event_isr_post_to(common_event_loop_handle, COMMON_EVENT, event_id, event_data, event_data_size, task_unblocked);
}
#endif


esp_err_t create_common_event_loop(void)
{
	const esp_event_loop_args_t common_event_loop_args = {
		.queue_size = 32,
		.task_name = "common_event_loop",
		.task_priority = uxTaskPriorityGet(NULL),
		.task_stack_size = 2048,
		.task_core_id = tskNO_AFFINITY
		};
	return esp_event_loop_create(&common_event_loop_args, &common_event_loop_handle);
}
