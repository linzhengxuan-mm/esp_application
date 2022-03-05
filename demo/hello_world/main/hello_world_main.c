/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"

#include "custom_console.h"
#include "app_console_cmd.h"
#include "board.h"
#include "fat.h"
#include "network.h"
#include "app_events.h"
#include "task_def.h"

static const char *TAG = "APP_FAT";

extern void ota_main_init(void);

static void nvs_init(void)
{
	esp_err_t err = nvs_flash_init();
	
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) 
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	ESP_ERROR_CHECK(err);
}
static void log_init(void)
{
	//read from nv to decide log level
	esp_log_level_set("*", ESP_LOG_DEBUG);
}
void app_main(void)
{
	nvs_init();
	log_init();
	fat_init();
	board_init();

	ESP_LOGD(TAG,"Hello world");
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    ESP_LOGD(TAG,"This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    ESP_LOGD(TAG,"silicon revision %d, ", chip_info.revision);
    ESP_LOGD(TAG,"%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    ESP_LOGD(TAG,"Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());
	
	custom_console_init();
	app_console_cmd_register();
	
	/* Initialize and register events */
	app_events_init();
	network_init();
	ota_main_init();
}
