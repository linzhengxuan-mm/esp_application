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
#include "custom_log.h"

static esp_log_level_t custom_log_level=ESP_LOG_NONE;

esp_log_level_t custom_log_get_level(void)
{
	return custom_log_level;
}
void custom_log_set_level(esp_log_level_t level)
{
	custom_log_level=level;
	// Calling this function restores all Info-level logging at runtime (as "Log Maximum Verbosity" set to "Info")
	//esp_log_level_set("*", ESP_LOG_INFO);
}
void initialize_custom_log(void)
{
	custom_log_level=ESP_LOG_NONE;
}
