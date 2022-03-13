/* Console example — declarations of command registration functions.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#ifndef __APP_EVENTS_H__
#define __APP_EVENTS_H__

#include "common_event.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief app events enum
 */
typedef enum {
    APP_EVENT_ANY = ESP_EVENT_ANY_ID,      /*!< app any event */
	APP_EVENT_USB_PLUGIN,
	APP_EVENT_USB_PLUGOUT,
	APP_EVENT_SIM_READY,
	APP_EVENT_SIM_NO_READY,
	APP_EVENT_NW_REG,
	APP_EVENT_NW_NO_REG,
} app_event_t;


void app_events_init(void);
esp_err_t app_events_send(int32_t event_id,void* param, size_t param_size);
esp_err_t app_events_isr_send(int32_t event_id,void* param, size_t param_size);

#ifdef __cplusplus
}
#endif

#endif

