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
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_sleep.h"
#include "driver/uart.h"
#include "cmd_system.h"
#include "cmd_wifi.h"
#include "cmd_nvs.h"
#include "custom_console.h"

#define PROMPT_STR CONFIG_IDF_TARGET

void custom_console_init(void)
{
	esp_console_repl_t *repl = NULL;
	esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();

	repl_config.prompt = PROMPT_STR "> ";
	repl_config.max_cmdline_length = 256;

	/* Register commands */
	register_system();
	register_wifi();
	register_nvs();
	
	// install console REPL environment
#if CONFIG_ESP_CONSOLE_UART
	esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &repl));
	#if CONFIG_PM_ENABLE
	ESP_ERROR_CHECK(uart_set_wakeup_threshold(uart_config.channel, 3));
	ESP_ERROR_CHECK(esp_sleep_enable_uart_wakeup(uart_config.channel));
	#endif
#elif CONFIG_ESP_CONSOLE_USB_CDC
	esp_console_dev_usb_cdc_config_t cdc_config = ESP_CONSOLE_DEV_CDC_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_console_new_repl_usb_cdc(&cdc_config, &repl_config, &repl));
#elif CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG
	esp_console_dev_usb_serial_jtag_config_t usbjtag_config = ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_console_new_repl_usb_serial_jtag(&usbjtag_config, &repl_config, &repl));
#endif
	ESP_ERROR_CHECK(esp_console_start_repl(repl));
}

void custom_console_register_cmd(const esp_console_cmd_t *cmd)
{
	ESP_ERROR_CHECK(esp_console_cmd_register(cmd));
}
esp_err_t custom_console_cmd_reply(const char*cmd_str)
{
	if (cmd_str)
	{
		printf("%s\r\n",cmd_str);
	}
	return ESP_OK;
}
