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
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "app_events.h"

static const char *TAG = "app_events";

static void app_events_on_common_event(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "app common event: %d", event_id);
	switch(event_id)
	{
		case APP_EVENT_SIM_READY:
			ESP_LOGI(TAG, "sim ready!");
			break;
		case APP_EVENT_SIM_NO_READY:
			ESP_LOGI(TAG, "sim no ready!");
			break;
		case APP_EVENT_NW_REG:
			ESP_LOGI(TAG, "network reg ok!");
			break;
		case APP_EVENT_NW_NO_REG:
			ESP_LOGI(TAG, "network reg fail!");
			break;
		default:
			break;
	}
}
esp_err_t app_events_send(int32_t event_id,void* param, size_t param_size)
{
	return common_event_post(event_id,param,param_size,portMAX_DELAY);
}
void app_events_init(void)
{
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	ESP_ERROR_CHECK(create_common_event_loop());
	ESP_ERROR_CHECK(common_event_register(APP_EVENT_ANY,&app_events_on_common_event));
}
