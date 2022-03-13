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
#include "esp_sleep.h"
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
static void printf_setup_info(void)
{
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
	printf("reset reason: %d\n", esp_reset_reason());

	esp_sleep_wakeup_cause_t wakeup_cause = esp_sleep_get_wakeup_cause();
	switch (wakeup_cause) 
	{
		case ESP_SLEEP_WAKEUP_EXT0: 
		{
			printf("Wake up from ext0\n");
			break;
		}
		case ESP_SLEEP_WAKEUP_EXT1: 
		{
			#if SOC_PM_SUPPORT_EXT_WAKEUP
			uint64_t wakeup_pin_mask = esp_sleep_get_ext1_wakeup_status();
			if (wakeup_pin_mask != 0) 
			{
				int pin = __builtin_ffsll(wakeup_pin_mask) - 1;
				printf("Wake up from ext1 GPIO %d\n", pin);
			} 
			else 
			{
				printf("Wake up from ext1 GPIO\n");
			}
			#endif
			break;
		}
		case ESP_SLEEP_WAKEUP_TIMER: 
		{
			printf("Wake up from timer\n");
			break;
		}
		case ESP_SLEEP_WAKEUP_TOUCHPAD:
		{
			printf("Wake up from touchpad\n");
			break;
		}
		case ESP_SLEEP_WAKEUP_ULP:
		{
			printf("Wake up from ulp\n");
			break;
		}
		case ESP_SLEEP_WAKEUP_UNDEFINED:
		default:
			printf("Not a deep sleep reset,wake up cause:%d\n",wakeup_cause);
	}
}
void app_main(void)
{
	printf_setup_info();
	
	nvs_init();	
	fat_init();
	app_events_init();
	
	board_init();
	
	custom_console_init();
	app_console_cmd_register();
	
	network_init();
	ota_main_init();
}
