// Copyright 2019-2021 Espressif Systems (Shanghai) PTE LTD
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

#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_timer.h"

#include "esp_modem.h"
#include "sim800.h"
#include "bg96.h"
#include "bg95.h"
#include "sim7600.h"

#include "led_indicator.h"
#include "modem_board.h"
#include "task_def.h"
#include "app_events.h"

static const char *TAG = "modem_board";

#define MODEM_BOARD_QUEUE_TIMEOUT_MS (100)
#define MODEM_BOARD_QUEUE_LENGTH (32)

#define MODEM_BOARD_CMD_LEN  32  

typedef struct 
{
	modem_dte_t*dte;
	
	xQueueHandle queue;
	esp_timer_handle_t timer;
	bool is_power_up;
	uint16_t timer_event;

	led_indicator_handle_t led_handle;
	led_indicator_blink_type_t led_blink_type;
} modem_board_t;

typedef struct {
	char cmd[MODEM_BOARD_CMD_LEN];
	uint32_t timeout_ms;
	modem_board_cmd_cb_fun_t cb_fun;
} modem_board_cmd_msg_t;

typedef enum
{
	MODEM_MSG_POWER_UP=0,
	MODEM_MSG_POWER_DOWN,
	MODEM_MSG_RESET,
	MODEM_MSG_CHECK_POWER_STATUS,
	
	MODEM_MSG_INIT_DCE,
	MODEM_MSG_ATTACH_TO_NETWORK,

	MODEM_MSG_SEND_CMD,
	
	MODEM_MSG_MAX_NUM,
}modem_board_message_id;


typedef enum
{
	MODEM_TIMER_IDLE_EVENT=0,
	MODEM_TIMER_WAIT_STABLE_EVENT,

	MODEM_TIMER_EVENT_NUM,
}modem_board_timer_event;

#define MODEM_BOARD_CHECK(a, str,...)                                \
    do                                                                                \
    {                                                                                 \
        if (!(a))                                                                     \
        {                                                                             \
            ESP_LOGE(TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        }                                                                             \
    } while (0)

#define MODEM_POWER_GPIO_ACTIVE_LEVEL     	0
#define MODEM_POWER_GPIO_ACTIVE_MS          3000
#define MODEM_POWER_GPIO_INACTIVE_MS        8000
#define MODEM_RESET_GPIO_ACTIVE_LEVEL     	0
#define MODEM_RESET_GPIO_ACTIVE_MS          500
#define MODEM_RESET_GPIO_INACTIVE_MS        5000

#ifdef CONFIG_MODEM_UART_TX_PIN
#define MODEM_UART_TX_GPIO                  CONFIG_MODEM_UART_TX_PIN
#else
#define MODEM_UART_TX_GPIO                  25
#endif

#ifdef CONFIG_MODEM_UART_RX_PIN
#define MODEM_UART_RX_GPIO                  CONFIG_MODEM_UART_RX_PIN
#else
#define MODEM_UART_RX_GPIO                  26
#endif

#ifdef CONFIG_MODEM_UART_RTS_PIN
#define MODEM_UART_RTS_GPIO                 CONFIG_MODEM_UART_RTS_PIN
#else
#define MODEM_UART_RTS_GPIO                 27
#endif

#ifdef CONFIG_MODEM_UART_CTS_PIN
#define MODEM_UART_CTS_GPIO                 CONFIG_MODEM_UART_CTS_PIN
#else
#define MODEM_UART_CTS_GPIO                 23
#endif

#ifdef CONFIG_MODEM_POWER_PIN
#define MODEM_POWER_GPIO                    CONFIG_MODEM_POWER_PIN
#else
#define MODEM_POWER_GPIO                    0
#endif

#ifdef CONFIG_MODEM_RESET_PIN
#define MODEM_RESET_GPIO                    CONFIG_MODEM_RESET_PIN
#else
#define MODEM_RESET_GPIO                    0
#endif

#ifdef CONFIG_MODEM_LED_INDICATOR_PIN
#define MODEM_LED_INDICATOR_GPIO            CONFIG_MODEM_LED_INDICATOR_PIN
#else
#define MODEM_LED_INDICATOR_GPIO            0
#endif

#ifdef CONFIG_MODEM_UART_TX_BUFFER_SIZE
#define MODEM_UART_TX_BUFFER_SIZE            CONFIG_MODEM_UART_TX_BUFFER_SIZE
#else
#define MODEM_UART_TX_BUFFER_SIZE            512
#endif


#ifdef CONFIG_MODEM_UART_RX_BUFFER_SIZE
#define MODEM_UART_RX_BUFFER_SIZE            CONFIG_MODEM_UART_RX_BUFFER_SIZE
#else
#define MODEM_UART_RX_BUFFER_SIZE            1024
#endif


#ifdef CONFIG_MODEM_UART_EVENT_QUEUE_SIZE
#define MODEM_UART_EVENT_QUEUE_SIZE          CONFIG_MODEM_UART_EVENT_QUEUE_SIZE
#else
#define MODEM_UART_EVENT_QUEUE_SIZE          30
#endif

#ifdef CONFIG_MODEM_UART_EVENT_TASK_STACK_SIZE
#define MODEM_UART_EVENT_TASK_STACK_SIZE     CONFIG_MODEM_UART_EVENT_TASK_STACK_SIZE
#else
#define MODEM_UART_EVENT_TASK_STACK_SIZE     4096
#endif

#ifdef CONFIG_MODEM_UART_EVENT_TASK_PRIORITY
#define MODEM_UART_EVENT_TASK_PRIORITY       CONFIG_MODEM_UART_EVENT_TASK_PRIORITY
#else
#define MODEM_UART_EVENT_TASK_PRIORITY       5
#endif

//static const int MODEM_BOARD_CONNECT_BIT = BIT0;
//static const int MODEM_BOARD_DISCONNECT_BIT = BIT1;
static modem_board_t modem_board_cntx;

DEFINE_COMMON_TASK_SEND_MSG_FUN(modem_board_send_message,modem_board_cntx.queue,TAG);

static void modem_board_led_indicator_init(void)
{
    led_indicator_config_t led_config = {
        .off_level = 0,
        .mode = LED_GPIO_MODE,
    };

    if(MODEM_LED_INDICATOR_GPIO) 
    {
		modem_board_cntx.led_handle = led_indicator_create(MODEM_LED_INDICATOR_GPIO, &led_config);
    }
	else
	{
		modem_board_cntx.led_handle = NULL;
	}
}
static void modem_board_led_indicator(led_indicator_blink_type_t blink_type)
{
	if (modem_board_cntx.led_handle)
	{
		led_indicator_stop(modem_board_cntx.led_handle,modem_board_cntx.led_blink_type);
		led_indicator_start(modem_board_cntx.led_handle, blink_type);
		modem_board_cntx.led_blink_type = blink_type;
	}
}
static void modem_board_timer_cb(void *arg)
{
	uint16_t*timer_event = (uint16_t*)arg;

	if(timer_event)
	{	
		ESP_LOGI(TAG, "modem_board_timer_cb,event:%d",*timer_event);
		switch(*timer_event)
		{
			case MODEM_TIMER_IDLE_EVENT:
				break;
			case MODEM_TIMER_WAIT_STABLE_EVENT:
				modem_board_send_message(MODEM_MSG_INIT_DCE,NULL,0x00,NULL);
				break;
			default:
				break;
		}
	}
	else
	{
		ESP_LOGE(TAG, "modem_board_timer_cb,invalid arg");
	}
}
static void modem_board_timer_create(esp_timer_handle_t* handle,void*arg)
{
    const esp_timer_create_args_t timer_args = {
            .callback = &modem_board_timer_cb,
			.arg = arg,
            .name = "modem board timer"
    };
    ESP_ERROR_CHECK(esp_timer_create(&timer_args, handle));
}
static void modem_board_start_timer(esp_timer_handle_t handle,uint64_t timeout_us,bool is_period)
{
	esp_err_t ret = ESP_OK;
	if(handle)
	{
		if(is_period)
		{
			ret = esp_timer_start_periodic(handle,timeout_us);
		}
		else
		{
			ret = esp_timer_start_once(handle,timeout_us);
		}
		MODEM_BOARD_CHECK(ret==ESP_OK,"start timer fail,ret:%d",ret);
	}
	else
	{
		ESP_LOGE(TAG, "start timer fail,invalid handle");
	}
}
static void modem_board_stop_timer(esp_timer_handle_t handle)
{
	esp_err_t ret = ESP_OK;
	if(handle)
	{
		ret = esp_timer_stop(handle);
		MODEM_BOARD_CHECK(ret==ESP_OK,"stop timer fail,ret:%d",ret);
	}
	else
	{
		ESP_LOGE(TAG, "stop timer fail,invalid handle");
	}
}
static void modem_board_timer_delete(esp_timer_handle_t*handle)
{
	esp_err_t ret = ESP_OK;
	if(*handle)
	{
		ret = esp_timer_delete(*handle);
		if (ret == ESP_OK)
		{
			*handle = NULL;
		}
		MODEM_BOARD_CHECK(ret==ESP_OK,"delete timer fail,ret:%d",ret);
	}
	else
	{
		ESP_LOGE(TAG, "delete timer fail,invalid handle");
	}
}
static void modem_board_gpio_pull(gpio_num_t gpio_num, uint32_t level)
{
    gpio_config_t io_config = {
            .pin_bit_mask = BIT64(gpio_num),
            .mode = GPIO_MODE_OUTPUT
    };
    gpio_config(&io_config);
    gpio_set_level(gpio_num, level);
}
static int modem_board_read_gpio(gpio_num_t gpio_num)
{
    gpio_config_t io_config = {
            .pin_bit_mask = BIT64(gpio_num),
            .mode = GPIO_MODE_OUTPUT
    };
    gpio_config(&io_config);
    return gpio_get_level(gpio_num);
}
static void modem_board_check_power_status(void)
{
	modem_board_send_message(MODEM_MSG_CHECK_POWER_STATUS,NULL,0x00,NULL);
}
static void modem_board_wait_stable(void)
{
	if (modem_board_cntx.timer_event != MODEM_TIMER_IDLE_EVENT)
	{
		modem_board_stop_timer(modem_board_cntx.timer);
	}
	modem_board_cntx.timer_event = MODEM_TIMER_WAIT_STABLE_EVENT;
	modem_board_start_timer(modem_board_cntx.timer,5*1000*1000,false);
}
static void modem_board_reset_handle(void)
{
	if (MODEM_RESET_GPIO)
	{
		modem_board_gpio_pull(MODEM_RESET_GPIO,MODEM_RESET_GPIO_ACTIVE_LEVEL);
	    vTaskDelay(pdMS_TO_TICKS(MODEM_RESET_GPIO_ACTIVE_MS));
		modem_board_gpio_pull(MODEM_RESET_GPIO,!MODEM_RESET_GPIO_ACTIVE_LEVEL);
	}
}
static void modem_board_power_up_handle(void)
{
	if (MODEM_POWER_GPIO)
	{
		modem_board_gpio_pull(MODEM_POWER_GPIO,MODEM_POWER_GPIO_ACTIVE_LEVEL);
	    vTaskDelay(pdMS_TO_TICKS(MODEM_POWER_GPIO_ACTIVE_MS));
		modem_board_gpio_pull(MODEM_POWER_GPIO,!MODEM_RESET_GPIO_ACTIVE_LEVEL);
	}
}
static void modem_board_power_down_handle(void)
{
	if(MODEM_POWER_GPIO)
	{
		modem_board_gpio_pull(MODEM_POWER_GPIO,MODEM_POWER_GPIO_ACTIVE_LEVEL);
	    vTaskDelay(pdMS_TO_TICKS(MODEM_POWER_GPIO_INACTIVE_MS));
		modem_board_gpio_pull(MODEM_POWER_GPIO,!MODEM_RESET_GPIO_ACTIVE_LEVEL);
	}
}
static void modem_board_check_power_status_handle(void)
{
	//to get modem power status and set status :is_power_up
	if (modem_board_cntx.is_power_up)
	{
		modem_board_cntx.is_power_up = false;
	}
	else
	{
		modem_board_cntx.is_power_up = true;
	}

	ESP_LOGI(TAG, "modem board power status:%d",modem_board_cntx.is_power_up);
	if (modem_board_cntx.is_power_up)
	{
		modem_board_wait_stable();
	}
}
static modem_dce_t* modem_board_init_dce_handle(modem_dte_t *dte)
{
	modem_dce_t *dce = NULL;

	ESP_LOGI(TAG, "modem_board_init_dce_handle");
	if (!dte)
	{
		ESP_LOGE(TAG, "modem board init dce fail,dte==NULL");
		return NULL;
	}

	/* create dce object */
	#if CONFIG_MODEM_DEVICE_SIM800
	dce = sim800_init(dte);
	#elif CONFIG_MODEM_DEVICE_BG96
	dce = bg96_init(dte);
	#elif CONFIG_MODEM_DEVICE_BG95
	dce = bg95_init(dte);
	#elif CONFIG_MODEM_DEVICE_SIM7600
	dce = sim7600_init(dte);
	#else
	#error "Unsupported DCE"
	#endif

	if(dce != NULL)
	{
		ESP_ERROR_CHECK(dce->set_flow_ctrl(dce, MODEM_FLOW_CONTROL_NONE));
		ESP_ERROR_CHECK(dce->store_profile(dce));
		/* Print Module ID, Operator, IMEI, IMSI */
		ESP_LOGI(TAG, "Module: %s", dce->name);
		ESP_LOGI(TAG, "Operator: %s", dce->oper);
		ESP_LOGI(TAG, "IMEI: %s", dce->imei);
		ESP_LOGI(TAG, "IMSI: %s", dce->imsi);
		/* Get signal quality */
		uint32_t rssi = 0, ber = 0;
		ESP_ERROR_CHECK(dce->get_signal_quality(dce, &rssi, &ber));
		ESP_LOGI(TAG, "rssi: %d, ber: %d", rssi, ber);
		/* Get battery voltage */
		uint32_t voltage = 0, bcs = 0, bcl = 0;
		ESP_ERROR_CHECK(dce->get_battery_status(dce, &bcs, &bcl, &voltage));
		ESP_LOGI(TAG, "Battery voltage: %d mV", voltage);
	}

	return dce;
}
static esp_err_t modem_board_send_cmd_handle(void *param,uint16_t param_len)
{
	esp_err_t ret=ESP_OK;
	modem_board_cmd_msg_t*cmd_msg = (modem_board_cmd_msg_t*)param;
	modem_dce_t*dce=(modem_board_cntx.dte)->dce;

	if((!cmd_msg)||(param_len==0))
	{
		ESP_LOGE(TAG, "invalid cmd msg!");
		return ESP_FAIL;
	}

	if(!dce)
	{
		ESP_LOGE(TAG, "dce is no ready!");
		goto error;
	}
	
	if (dce->send_user_cmd)
	{
		ret=dce->send_user_cmd(dce,cmd_msg->cmd,cmd_msg->timeout_ms,cmd_msg->cb_fun);
	}
	else
	{
		ESP_LOGE(TAG, "dce send user cmd fun is null!");
		ret=ESP_FAIL;
	}
	if(ret==ESP_OK)
	{
		return ret;
	}
error:
	if(cmd_msg->cb_fun)
	{
		cmd_msg->cb_fun("ERROR");
	}
	return ESP_FAIL;
}
static void modem_board_message_dispatch(task_common_msg_t*msg)
{
	if(!msg)
	{
		return;
	}

	switch(msg->msg_id)
	{
		case MODEM_MSG_POWER_UP:
			modem_board_power_up_handle();
			modem_board_check_power_status();
			break;
		case MODEM_MSG_POWER_DOWN:
			modem_board_power_down_handle();
			modem_board_check_power_status();
			break;
		case MODEM_MSG_RESET:
			modem_board_reset_handle();
			break;
		case MODEM_MSG_CHECK_POWER_STATUS:
			modem_board_check_power_status_handle();
			break;
		case MODEM_MSG_INIT_DCE:
			if(modem_board_init_dce_handle(modem_board_cntx.dte)!=NULL)
			{
				app_events_send(APP_EVENT_SIM_READY,NULL,0);
				modem_board_send_message(MODEM_MSG_ATTACH_TO_NETWORK,NULL,0x00,NULL);
			}
			break;
			
		case MODEM_MSG_SEND_CMD:
			modem_board_send_cmd_handle(msg->param,msg->param_len);
		default:
			break;
	}
}
static void modem_board_task(void *param)
{
	/* create dte object */
	esp_modem_dte_config_t config = ESP_MODEM_DTE_DEFAULT_CONFIG();
	/* setup UART specific configuration based on kconfig options */
	config.tx_io_num = MODEM_UART_TX_GPIO;
	config.rx_io_num = MODEM_UART_RX_GPIO;
	config.rts_io_num = MODEM_UART_RTS_GPIO;
	config.cts_io_num = MODEM_UART_CTS_GPIO;
	config.rx_buffer_size = MODEM_UART_RX_BUFFER_SIZE;
	config.tx_buffer_size = MODEM_UART_TX_BUFFER_SIZE;
	config.event_queue_size = MODEM_UART_EVENT_QUEUE_SIZE;
	config.event_task_stack_size = MODEM_UART_EVENT_TASK_STACK_SIZE;
	config.event_task_priority = MODEM_UART_EVENT_TASK_PRIORITY;
	config.dte_buffer_size = MODEM_UART_RX_BUFFER_SIZE / 2;

	modem_board_cntx.dte = esp_modem_dte_init(&config);
	assert(modem_board_cntx.dte!=NULL);
	
	COMMON_TASK_RUN(modem_board_cntx.queue,modem_board_message_dispatch);

	if (modem_board_cntx.dte->dce!=NULL)
	{
    	ESP_ERROR_CHECK((modem_board_cntx.dte->dce)->deinit((modem_board_cntx.dte->dce)));
	}
    ESP_ERROR_CHECK(modem_board_cntx.dte->deinit(modem_board_cntx.dte));
	modem_board_cntx.dte = NULL;
    vTaskDelete(NULL);
}
esp_err_t modem_board_power_up(void)
{
	return modem_board_send_message(MODEM_MSG_POWER_UP,NULL,0x00,NULL);
}
esp_err_t modem_board_power_down(void)
{
	return modem_board_send_message(MODEM_MSG_POWER_DOWN,NULL,0x00,NULL);
}
esp_err_t modem_board_reset(void)
{
	return modem_board_send_message(MODEM_MSG_RESET,NULL,0x00,NULL);
}
esp_err_t modem_board_send_cmd(char*cmd,uint32_t timeout_ms,modem_board_cmd_cb_fun_t cb)
{
	if (modem_board_cntx.dte->dce)
	{
		modem_board_cmd_msg_t cmd_msg;

		memset(&cmd_msg,0x00,sizeof(modem_board_cmd_msg_t));
		strncpy(cmd_msg.cmd,cmd,MODEM_BOARD_CMD_LEN);
		cmd_msg.cb_fun = cb;
		cmd_msg.timeout_ms = timeout_ms;
		return modem_board_send_message(MODEM_MSG_SEND_CMD,&cmd_msg,sizeof(modem_board_cmd_msg_t),NULL);
	}
	else
	{
		return ESP_FAIL;
	}
}
esp_err_t modem_board_init(void)
{
	memset(&modem_board_cntx,0x00,sizeof(modem_board_t));
	modem_board_led_indicator_init();

	modem_board_cntx.timer = NULL;
	modem_board_cntx.timer_event = MODEM_TIMER_IDLE_EVENT;
	modem_board_timer_create(&(modem_board_cntx.timer),(void*)&(modem_board_cntx.timer_event));
	
	COMMON_TASK_QUEUE_CREATE(modem_board_cntx.queue,
				MODEM_BOARD_QUEUE_LENGTH,
				modem_board_task,
				MODEM_BOARD_TASK_NAME,
				MODEM_BOARD_TASK_STACK_SIZE,
				MODEM_BOARD_TASK_PRIORITY,
				MODEM_BOARD_TASK_COREID,
				TAG,
				ESP_FAIL);

	modem_board_cntx.is_power_up = false;
	modem_board_power_up();
	return ESP_OK;
}
