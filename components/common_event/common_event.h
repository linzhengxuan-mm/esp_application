/* Console example — declarations of command registration functions.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#ifndef __COMMON_EVENT_H__
#define __COMMON_EVENT_H__

#include "sdkconfig.h"
#include "esp_event.h"
#include "esp_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

ESP_EVENT_DECLARE_BASE(COMMON_EVENT);

esp_err_t create_common_event_loop(void);
esp_err_t common_event_register(int32_t event_id, esp_event_handler_t event_handler);
esp_err_t common_event_unregister(int32_t event_id, esp_event_handler_t event_handler);
esp_err_t common_event_post(int32_t event_id,void* event_data, size_t event_data_size, TickType_t ticks_to_wait);
#if CONFIG_ESP_EVENT_POST_FROM_ISR
esp_err_t common_event_isr_post(int32_t event_id,void* event_data, size_t event_data_size);
#endif

#ifdef __cplusplus
}
#endif

#endif
