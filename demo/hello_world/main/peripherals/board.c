// Copyright 2017-2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_pm.h"
#include "esp_sleep.h"
#include "esp_event.h"
#include "esp_log.h"

#include "driver/rtc_io.h"
#include "driver/rtc_cntl.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "soc/rtc.h"
#if CONFIG_IDF_TARGET_ESP32
#include "esp32/clk.h"
#elif CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/clk.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/clk.h"
#elif CONFIG_IDF_TARGET_ESP32C3
#include "esp32c3/clk.h"
#elif CONFIG_IDF_TARGET_ESP32H2
#include "esp32h2/clk.h"
#endif


#include "app_events.h"
#include "task_def.h"
#include "board.h"


static const char *TAG = "BORAD";

#define GPIO_INPUT_IO_USB_DETECT     		5
#define USB_DETECT_WAKEUP_LEVEL_DEFAULT   	0

#if CONFIG_IDF_TARGET_ESP32
typedef esp_pm_config_esp32_t board_pm_config_t;
#elif CONFIG_IDF_TARGET_ESP32S2
typedef esp_pm_config_esp32s2_t board_pm_config_t;
#elif CONFIG_IDF_TARGET_ESP32S3
typedef esp_pm_config_esp32s3_t board_pm_config_t;
#elif CONFIG_IDF_TARGET_ESP32C3
typedef esp_pm_config_esp32c3_t board_pm_config_t;
#elif CONFIG_IDF_TARGET_ESP32H2
typedef esp_pm_config_esp32h2_t board_pm_config_t;
#endif


#ifdef CONFIG_PM_ENABLE
static esp_pm_lock_handle_t s_project_board_sleep_lock=NULL;
#endif
static bool s_board_usb_detect_flag =false;

static void board_gpio_init(void)
{
	
}
static void board_detect_task(void* arg)
{
    gpio_config_t config = {
            .pin_bit_mask = BIT64(GPIO_INPUT_IO_USB_DETECT),
            .mode = GPIO_MODE_INPUT
    };
    ESP_ERROR_CHECK(gpio_config(&config));
    gpio_wakeup_enable(GPIO_INPUT_IO_USB_DETECT,
            USB_DETECT_WAKEUP_LEVEL_DEFAULT == 0 ? GPIO_INTR_LOW_LEVEL : GPIO_INTR_HIGH_LEVEL);
	if (gpio_get_level(GPIO_INPUT_IO_USB_DETECT) == USB_DETECT_WAKEUP_LEVEL_DEFAULT) 
	{
		app_events_send(APP_EVENT_USB_PLUGIN,NULL,0);
		board_lock_sleep();
		s_board_usb_detect_flag = true;
	}
	else
	{
		s_board_usb_detect_flag = false;
	}
	
	while(1)
	{
        esp_sleep_enable_gpio_wakeup();
        /* Wait until GPIO goes high */
        if (gpio_get_level(GPIO_INPUT_IO_USB_DETECT) == USB_DETECT_WAKEUP_LEVEL_DEFAULT) 
		{
			if (!s_board_usb_detect_flag)
			{
				#if CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG
				ESP_LOGI(TAG,"usb detect need restart!!!");
				break;
				#else
				app_events_send(APP_EVENT_USB_PLUGIN,NULL,0);
				board_lock_sleep();
				s_board_usb_detect_flag = true;
				#endif
			}
        }
		else
		{
			if (s_board_usb_detect_flag)
			{
				app_events_isr_send(APP_EVENT_USB_PLUGOUT,NULL,0);
				#if CONFIG_ESP_CONSOLE_UART
				uart_wait_tx_idle_polling(CONFIG_ESP_CONSOLE_UART_NUM);
				#endif
				board_unlock_sleep();
				s_board_usb_detect_flag = false;
			}
		}
		vTaskDelay(pdMS_TO_TICKS(1000));
	}

	#if CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG
    vTaskDelete(NULL);
	esp_restart();
	#endif
}
static void board_light_sleep_enable(void)
{	
#ifdef CONFIG_PM_ENABLE
	int cur_freq_mhz = esp_clk_cpu_freq() / MHZ;
	int xtal_freq = (int) rtc_clk_xtal_freq_get();

	board_pm_config_t pm_config = {
	    .max_freq_mhz = cur_freq_mhz,
	    .min_freq_mhz = xtal_freq,
		#ifdef CONFIG_FREERTOS_USE_TICKLESS_IDLE
	    .light_sleep_enable = true
	    #endif
	};
	ESP_LOGI(TAG,"%s,cur_freq_mhz:%d,xtal_freq:%d",__func__,cur_freq_mhz,xtal_freq);
	ESP_ERROR_CHECK(esp_pm_configure(&pm_config) );
#endif
}
static void board_light_sleep_disable(void)
{
#ifdef CONFIG_PM_ENABLE
	int cur_freq_mhz = esp_clk_cpu_freq() / MHZ;

	board_pm_config_t pm_config = {
	    .max_freq_mhz = cur_freq_mhz,
	    .min_freq_mhz = cur_freq_mhz,
	};
	ESP_LOGI(TAG,"%s,cur_freq_mhz:%d",__func__,cur_freq_mhz);
	ESP_ERROR_CHECK(esp_pm_configure(&pm_config));
#endif
}
esp_err_t board_init(void)
{
	esp_err_t err = ESP_OK;
	
#ifdef CONFIG_PM_ENABLE
	board_light_sleep_enable();

	if (s_project_board_sleep_lock == NULL) 
	{
		err = esp_pm_lock_create(ESP_PM_NO_LIGHT_SLEEP, 0, "project_board",&s_project_board_sleep_lock);
		if (err != ESP_OK) 
		{
			return err;
		}
	}
#endif

	board_gpio_init();

    xTaskCreate(board_detect_task, "detect_task", 20480, NULL, 10, NULL);
    return err;
}

void board_lock_sleep(void)
{
#ifdef CONFIG_PM_ENABLE
	ESP_LOGI(TAG,"%s",__func__);
    if(s_project_board_sleep_lock)
    {
    	esp_pm_lock_acquire(s_project_board_sleep_lock);
    }
	else
	{
		ESP_LOGE(TAG,"%s,no sleep lock handle",__func__);
	}
#endif
}

void board_unlock_sleep(void)
{
#ifdef CONFIG_PM_ENABLE
	ESP_LOGI(TAG,"%s",__func__);
    if(s_project_board_sleep_lock)
    {
    	esp_pm_lock_release(s_project_board_sleep_lock);
    }
	else
	{
		ESP_LOGE(TAG,"%s,no sleep lock handle",__func__);
	}
#endif
}
void board_enter_deep_sleep(void)
{
	ESP_LOGI(TAG,"%s",__func__);

	board_light_sleep_disable();
	esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);

	#if SOC_PM_SUPPORT_EXT_WAKEUP
	ESP_ERROR_CHECK(rtc_gpio_init(GPIO_INPUT_IO_USB_DETECT));
	ESP_ERROR_CHECK(gpio_pullup_en(GPIO_INPUT_IO_USB_DETECT));
	ESP_ERROR_CHECK(gpio_pulldown_dis(GPIO_INPUT_IO_USB_DETECT));
	ESP_ERROR_CHECK(esp_sleep_enable_ext0_wakeup(GPIO_INPUT_IO_USB_DETECT, USB_DETECT_WAKEUP_LEVEL_DEFAULT));
	#endif
	esp_deep_sleep_start();
}
