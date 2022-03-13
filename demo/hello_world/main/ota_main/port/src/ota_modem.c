/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "driver/uart.h"
#include "driver/gpio.h"

#include "ota_modem.h"

#define OTA_MODEM_UART_RX_FULL_THRESHOLD (64)

static const char *MODEM_TAG = "ota_modem";
#define MODEM_CHECK(a, str, goto_tag, ...)                                              \
    do                                                                                  \
    {                                                                                   \
        if (!(a))                                                                       \
        {                                                                               \
            ESP_LOGE(MODEM_TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            goto goto_tag;                                                              \
        }                                                                               \
    } while (0)

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

#define MODEM_POWER_GPIO_ACTIVE_LEVEL     	0
#define MODEM_POWER_GPIO_ACTIVE_MS          3000
#define MODEM_POWER_GPIO_INACTIVE_MS        8000
#define MODEM_RESET_GPIO_ACTIVE_LEVEL     	0
#define MODEM_RESET_GPIO_ACTIVE_MS          500
#define MODEM_RESET_GPIO_INACTIVE_MS        5000


#define MODEM_CHECK_POWER_UP_DELAY_MS       3000
#define MODEM_CHECK_POWER_DOWN_DELAY_MS     3000
#define MODEM_CHECK_STABLE_DELAY_MS       	7000

#define OTA_MODEM_TASK_NAME					"ota_modem"
#define OTA_MODEM_TASK_STACK_SIZE			4096
#define OTA_MODEM_TASK_PRIORITY				10
#define OTA_MODEM_QUEUE_NUM					32


typedef struct 
{
	uint16_t msg_id;
	void *param;
	uint16_t param_len;
} ota_modem_msg_t;

typedef enum
{
	OTA_MODEM_MSG_POWER_UP=0,
	OTA_MODEM_MSG_POWER_DOWN,
	OTA_MODEM_MSG_POWER_RESET,
	
	OTA_MODEM_MSG_WAIT_POWER_UP,
	OTA_MODEM_MSG_WAIT_POWER_DOWN,
	OTA_MODEM_MSG_UPDATE_POWER_STATUS,
	
	OTA_MODEM_MSG_SEND_DATA,
	
	OTA_MODEM_MSG_MAX_NUM,
}ota_modem_message_id;

typedef enum
{
	OTA_MODEM_TIMER_EVENT_WAIT_POWERUP=0,
	OTA_MODEM_TIMER_EVENT_WAIT_STABLE,
	OTA_MODEM_TIMER_EVENT_WAIT_POWERDOWN
}ota_modem_timer_event_t;

typedef enum {
    MODEM_FLOW_CONTROL_NONE = 0,
    MODEM_FLOW_CONTROL_SW,
    MODEM_FLOW_CONTROL_HW
} ota_modem_flow_ctrl_t;

typedef struct 
{
    uart_port_t port_num;           /*!< UART port number */
    uart_word_length_t data_bits;   /*!< Data bits of UART */
    uart_stop_bits_t stop_bits;     /*!< Stop bits of UART */
    uart_parity_t parity;           /*!< Parity type */
    ota_modem_flow_ctrl_t flow_control; /*!< Flow control type */
    unsigned int baud_rate;             /*!< Communication baud rate */
    int tx_io_num;                  /*!< TXD Pin Number */
    int rx_io_num;                  /*!< RXD Pin Number */
    int rts_io_num;                 /*!< RTS Pin Number */
    int cts_io_num;                 /*!< CTS Pin Number */
    int rx_buffer_size;             /*!< UART RX Buffer Size */
    int tx_buffer_size;             /*!< UART TX Buffer Size */
    int event_queue_size;           /*!< UART Event Queue Size */
    uint32_t event_task_stack_size; /*!< UART Event Task Stack size */
    int event_task_priority;        /*!< UART Event Task Priority */
    int received_buffer_size;       /*!< Compatible option for the internal buffer size */
} ota_modem_config_t;

typedef struct 
{
	uart_port_t uart_port;                  /*!< UART port */
	uint8_t *buffer;                        /*!< Internal buffer to store response lines/data from DCE */
	QueueHandle_t event_queue;              /*!< UART event queue handle */
	TaskHandle_t uart_event_task_hdl;       /*!< UART event task handle */
	SemaphoreHandle_t process_sem;          /*!< Semaphore used for indicating processing status */
	ota_modem_received_ind received_cb;     /*!< ptr to data reception */
	ota_modem_event_ind event_cb;
	ota_modem_line_handle line_handle;
	int buffer_size;                        /*!< internal buffer size */
	int consumed;                           /*!< index to the consumed buffer pointer */	
} ota_modem_dte_t;


typedef struct 
{
	ota_modem_dte_t*dte;
	esp_timer_handle_t timer_handle;
	xQueueHandle queue;

	ota_modem_received_ind received_cb;     /*!< ptr to data reception */
	ota_modem_event_ind event_cb;
	ota_modem_line_handle line_handle;
	
	bool is_power_up;
	ota_modem_timer_event_t timer_event;
} ota_modem_cntx_t;


#define OTA_MODEM_DEFAULT_CONFIG()          \
    {                                           \
        .port_num = UART_NUM_1,                 \
        .data_bits = UART_DATA_8_BITS,          \
        .stop_bits = UART_STOP_BITS_1,          \
        .parity = UART_PARITY_DISABLE,          \
        .baud_rate = 115200,                    \
        .flow_control = MODEM_FLOW_CONTROL_NONE,\
        .tx_io_num = 25,                        \
        .rx_io_num = 26,                        \
        .rts_io_num = 27,                       \
        .cts_io_num = 23,                       \
        .rx_buffer_size = 1024,                 \
        .tx_buffer_size = 512,                  \
        .event_queue_size = 30,                 \
        .event_task_stack_size = 2048,          \
        .event_task_priority = 5,               \
        .received_buffer_size = 512                  \
    }

static ota_modem_cntx_t s_ota_modem_cntx;

static inline bool is_only_cr(const char *str, uint32_t len)
{
    for (int i=0; i<len; ++i) {
        if (str[i] != '\r') {
            return false;
        }
    }
    return true;
}
static esp_err_t ota_modem_send_msg(uint16_t msg_id, void *param, uint32_t parma_len)
{ 
	ota_modem_msg_t msg;
	BaseType_t ret,xHigherPriorityTaskWoken = pdFALSE;
	
	if (!s_ota_modem_cntx.queue) 
	{
		ESP_LOGE(MODEM_TAG, "%s,send msg fail,queue is null",__func__);
		return ESP_FAIL;
	} 
	msg.msg_id = msg_id;  
	if ((param)&&(parma_len>0))
	{
		msg.param = malloc(parma_len+1);
		if(msg.param)
		{
			memset(msg.param,0x00,parma_len+1);
			memcpy(msg.param,param,parma_len);
			msg.param_len = parma_len;
		}
		else
		{
			ESP_LOGE(MODEM_TAG, "%s,send msg fail,param malloc fail",__func__);
			return ESP_FAIL;
		}
	}
	else
	{
		msg.param = NULL;
		msg.param_len = 0;
	}
	
	if( xPortInIsrContext() == pdTRUE )
	{
		ret = xQueueSendFromISR(s_ota_modem_cntx.queue, &msg, &xHigherPriorityTaskWoken);
		if ( xHigherPriorityTaskWoken )
		{
			portYIELD_FROM_ISR();
		}
	}
	else
	{
		ret = xQueueSend(s_ota_modem_cntx.queue, &msg, 0);
	}
	
	if (ret != pdTRUE)
	{
		ESP_LOGE(MODEM_TAG, "%s,send msg fail",__func__);
		if (msg.param)
		{
			free(msg.param);
		}
		return ESP_FAIL;
	}
	return ESP_OK;
}

static void ota_modem_timer_cb(void *arg)
{
	uint16_t*timer_event = (uint16_t*)arg;

	if(timer_event)
	{	
		ESP_LOGI(MODEM_TAG, "%s,event:%d",__func__,*timer_event);
		switch(*timer_event)
		{
			case OTA_MODEM_TIMER_EVENT_WAIT_POWERUP:
				ota_modem_send_msg(OTA_MODEM_MSG_WAIT_POWER_UP,NULL,0);
				break;
			case OTA_MODEM_TIMER_EVENT_WAIT_STABLE:
				ota_modem_send_msg(OTA_MODEM_MSG_UPDATE_POWER_STATUS,NULL,0);
				break;
			case OTA_MODEM_TIMER_EVENT_WAIT_POWERDOWN:
				ota_modem_send_msg(OTA_MODEM_MSG_WAIT_POWER_DOWN,NULL,0);
				break;
			default:
				break;
		}
	}
	else
	{
		ESP_LOGE(MODEM_TAG, "%s,invalid arg",__func__);
	}
}
static esp_err_t ota_modem_create_timer(esp_timer_handle_t* handle,void*arg)
{
    const esp_timer_create_args_t timer_args = {
            .callback = &ota_modem_timer_cb,
			.arg = arg,
            .name = "modem board timer"
    };
    return esp_timer_create(&timer_args, handle);
}
static void ota_modem_start_timer(esp_timer_handle_t handle,uint64_t timeout_ms,bool is_period)
{
	if(handle)
	{
		if(is_period)
		{
			esp_timer_start_periodic(handle,timeout_ms*1000);
		}
		else
		{
			esp_timer_start_once(handle,timeout_ms*1000);
		}
	}
	else
	{
		ESP_LOGE(MODEM_TAG, "start timer fail,invalid handle");
	}
}
static void ota_modem_stop_timer(esp_timer_handle_t handle)
{
	if(handle)
	{
		esp_timer_stop(handle);
	}
	else
	{
		ESP_LOGE(MODEM_TAG, "stop timer fail,invalid handle");
	}
}
static void ota_modem_delete_timer(esp_timer_handle_t*handle)
{
	esp_err_t ret = ESP_OK;
	
	if(*handle)
	{
		ret = esp_timer_delete(*handle);
		if (ret == ESP_OK)
		{
			*handle = NULL;
		}
	}
	else
	{
		ESP_LOGE(MODEM_TAG, "delete timer fail,invalid handle");
	}
}
static void ota_modem_drv_pull_gpio(gpio_num_t gpio_num, uint32_t level)
{
    gpio_config_t io_config = {
            .pin_bit_mask = BIT64(gpio_num),
            .mode = GPIO_MODE_OUTPUT
    };
    gpio_config(&io_config);
    gpio_set_level(gpio_num, level);
}
static int ota_modem_drv_read_gpio(gpio_num_t gpio_num)
{
    gpio_config_t io_config = {
            .pin_bit_mask = BIT64(gpio_num),
            .mode = GPIO_MODE_OUTPUT
    };
    gpio_config(&io_config);
    return gpio_get_level(gpio_num);
}
static void ota_modem_drv_power_reset(void)
{
	if (MODEM_RESET_GPIO)
	{
		ota_modem_drv_pull_gpio(MODEM_RESET_GPIO,MODEM_RESET_GPIO_ACTIVE_LEVEL);
		vTaskDelay(pdMS_TO_TICKS(MODEM_RESET_GPIO_ACTIVE_MS));
		ota_modem_drv_pull_gpio(MODEM_RESET_GPIO,!MODEM_RESET_GPIO_ACTIVE_LEVEL);
	}
}
static void ota_modem_drv_power_up(void)
{
	if (MODEM_POWER_GPIO)
	{
		ota_modem_drv_pull_gpio(MODEM_POWER_GPIO,MODEM_POWER_GPIO_ACTIVE_LEVEL);
		vTaskDelay(pdMS_TO_TICKS(MODEM_POWER_GPIO_ACTIVE_MS));
		ota_modem_drv_pull_gpio(MODEM_POWER_GPIO,!MODEM_RESET_GPIO_ACTIVE_LEVEL);
	}
}
static void ota_modem_drv_power_down(void)
{
	if(MODEM_POWER_GPIO)
	{
		ota_modem_drv_pull_gpio(MODEM_POWER_GPIO,MODEM_POWER_GPIO_ACTIVE_LEVEL);
		vTaskDelay(pdMS_TO_TICKS(MODEM_POWER_GPIO_INACTIVE_MS));
		ota_modem_drv_pull_gpio(MODEM_POWER_GPIO,!MODEM_RESET_GPIO_ACTIVE_LEVEL);
	}
}
static bool ota_modem_drv_read_power_status(void)
{
	//to get modem power status and set status :is_power_up
	//ota_modem_drv_read_gpio();
	return true;
}
static int ota_modem_handle_line(ota_modem_dte_t *modem_dte, char * line, unsigned int len, char separator)
{
    /* Tokenize the data to call handlers separately for each *line* */
    char *str_ptr = NULL;
    char *p = strtok_r(line, "\n", &str_ptr);
	
    while (p) 
	{
        int plen = strlen(p);
        if (plen > 1 && !is_only_cr(p, plen)) 
		{
            ESP_LOGD(MODEM_TAG, "Handling line: >>%s\n<<", p);
            if (modem_dte->line_handle == NULL) 
			{
                /* Received an asynchronous line, but no handler waiting this this */
                ESP_LOGD(MODEM_TAG, "No handler for line: %s", p);
                return 0; /* Not an error, just propagate the line to user handler */
            }
			
            if (modem_dte->line_handle(p) != 0) 
			{
                ESP_LOGE(MODEM_TAG, "handle line failed");
            }
        }
        p = strtok_r(NULL, "\n", &str_ptr);
    }
    return 0;
}

static void ota_modem_handle_uart_data(ota_modem_dte_t *modem_dte)
{
	unsigned int length = 0;
	
	uart_get_buffered_data_len(modem_dte->uart_port, &length);
	ESP_LOGV(MODEM_TAG, "uart_get_buffered_data_len()=%d", length);

    if (modem_dte->line_handle != NULL && length) 
	{
        // Read the data and process it using `line_handle` logic
        length = MIN(modem_dte->buffer_size - 1, length);
        length = uart_read_bytes(modem_dte->uart_port, modem_dte->buffer + modem_dte->consumed, length - modem_dte->consumed, portMAX_DELAY);
        const char separator = '\n';
        if (memchr(modem_dte->buffer + modem_dte->consumed, separator, length)) 
		{
            modem_dte->buffer[length] = '\0';
            ESP_LOG_BUFFER_HEXDUMP("esp-modem: pattern-detection", modem_dte->buffer, length, ESP_LOG_VERBOSE);
            /* Send new line to handle if handler registered */
            if (ota_modem_handle_line(modem_dte, (char*)modem_dte->buffer, length, separator) == 0) 
			{
                modem_dte->consumed = 0;
                return;
            }
            modem_dte->consumed += length;
        }
        return;
    }
	
	length = MIN(modem_dte->buffer_size, length);
	length = uart_read_bytes(modem_dte->uart_port, modem_dte->buffer, length, portMAX_DELAY);
	/* pass the input data to configured callback */
	if ((length>0)&&(modem_dte->received_cb!=NULL))
	{
		modem_dte->received_cb(modem_dte->buffer, length);
	}
}
static void ota_modem_uart_events_task(void *param)
{
	ota_modem_dte_t *modem_dte = (ota_modem_dte_t *)param;
	uart_event_t event;
	
	while (1) 
	{
		if (!modem_dte)
		{
			break;
		}
		
		/* Process UART events */
		if (xQueueReceive(modem_dte->event_queue, &event, pdMS_TO_TICKS(100))) 
		{
			switch (event.type) 
			{
				case UART_DATA:
					ota_modem_handle_uart_data(modem_dte);
					break;
				case UART_FIFO_OVF:
					ESP_LOGW(MODEM_TAG, "%s,HW FIFO Overflow",__func__);
					uart_flush_input(modem_dte->uart_port);
					xQueueReset(modem_dte->event_queue);
					break;
				case UART_BUFFER_FULL:
					ESP_LOGW(MODEM_TAG, "%s,Ring Buffer Full",__func__);
					uart_flush_input(modem_dte->uart_port);
					xQueueReset(modem_dte->event_queue);
					break;
				case UART_BREAK:
					ESP_LOGW(MODEM_TAG, "%s,Rx Break",__func__);
					if(modem_dte->event_cb!=NULL)
					{
						modem_dte->event_cb(OTA_MODEM_EVENT_COM_ERROR);
					}
					break;
				case UART_PARITY_ERR:
					ESP_LOGE(MODEM_TAG, "%s,Parity Error",__func__);
					if(modem_dte->event_cb!=NULL)
					{
						modem_dte->event_cb(OTA_MODEM_EVENT_COM_ERROR);
					}
					break;
				case UART_FRAME_ERR:
					ESP_LOGE(MODEM_TAG, "%s,Frame Error",__func__);
					if(modem_dte->event_cb!=NULL)
					{
						modem_dte->event_cb(OTA_MODEM_EVENT_COM_ERROR);
					}
					break;
				default:
					ESP_LOGW(MODEM_TAG, "%s,unknown uart event type: %d", __func__,event.type);
					break;
			}
		}
	}
}

static ota_modem_dte_t* ota_modem_dte_init(void)
{
	esp_err_t res;

	ota_modem_config_t config = OTA_MODEM_DEFAULT_CONFIG();
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
	config.received_buffer_size = MODEM_UART_RX_BUFFER_SIZE / 2;


	/* malloc memory for modem_dte object */
	ota_modem_dte_t *modem_dte = calloc(1, sizeof(ota_modem_dte_t));
	MODEM_CHECK(modem_dte, "calloc modem_dte failed", err_mem);
	/* init callback */
	modem_dte->received_cb = NULL;
	modem_dte->line_handle = NULL;
	/* malloc memory to storing lines from modem dce */
	modem_dte->buffer_size = config.received_buffer_size;
	modem_dte->buffer = calloc(1, config.received_buffer_size);
	MODEM_CHECK(modem_dte->buffer, "calloc line memory failed", err_line_mem);
	/* Set attributes */
	modem_dte->uart_port = config.port_num;
	
	/* Config UART */
	uart_config_t uart_config = {
		.baud_rate = config.baud_rate,
		.data_bits = config.data_bits,
		.parity = config.parity,
		.stop_bits = config.stop_bits,
		#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
		.source_clk = UART_SCLK_REF_TICK,
		#else
		.source_clk = UART_SCLK_XTAL,
		#endif
		.flow_ctrl = (config.flow_control == MODEM_FLOW_CONTROL_HW) ? UART_HW_FLOWCTRL_CTS_RTS : UART_HW_FLOWCTRL_DISABLE
	};
	MODEM_CHECK(uart_param_config(modem_dte->uart_port, &uart_config) == ESP_OK, "config uart parameter failed", err_uart_config);
	if (config.flow_control == MODEM_FLOW_CONTROL_HW) {
		res = uart_set_pin(modem_dte->uart_port, config.tx_io_num, config.rx_io_num,
						   config.rts_io_num, config.cts_io_num);
	} else {
		res = uart_set_pin(modem_dte->uart_port, config.tx_io_num, config.rx_io_num,
						   UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	}
	MODEM_CHECK(res == ESP_OK, "config uart gpio failed", err_uart_config);
	/* Set flow control threshold */
	if (config.flow_control == MODEM_FLOW_CONTROL_HW) {
		res = uart_set_hw_flow_ctrl(modem_dte->uart_port, UART_HW_FLOWCTRL_CTS_RTS, UART_FIFO_LEN - 8);
	} else if (config.flow_control == MODEM_FLOW_CONTROL_SW) {
		res = uart_set_sw_flow_ctrl(modem_dte->uart_port, true, 8, UART_FIFO_LEN - 8);
	}
	MODEM_CHECK(res == ESP_OK, "config uart flow control failed", err_uart_config);
	/* Install UART driver and get event queue used inside driver */
	res = uart_driver_install(modem_dte->uart_port, config.rx_buffer_size, config.tx_buffer_size,
							  config.event_queue_size, &(modem_dte->event_queue), 0);
	MODEM_CHECK(res == ESP_OK, "install uart driver failed", err_uart_config);
	res = uart_set_rx_timeout(modem_dte->uart_port, 1);
	MODEM_CHECK(res == ESP_OK, "set rx timeout failed", err_uart_config);

	res = uart_set_rx_full_threshold(config.port_num, OTA_MODEM_UART_RX_FULL_THRESHOLD);
	MODEM_CHECK(res == ESP_OK, "config rx full threshold failed", err_uart_config);

	/* Create semaphore */
	modem_dte->process_sem = xSemaphoreCreateBinary();
	MODEM_CHECK(modem_dte->process_sem, "create process semaphore failed", err_sem);

	/* Create UART Event task */
	BaseType_t ret = xTaskCreatePinnedToCore(ota_modem_uart_events_task, 			//Task Entry
								 "modem_uart",				//Task Name
								 config.event_task_stack_size, 		  //Task Stack Size(Bytes)
								 modem_dte,							//Task Parameter
								 config.event_task_priority,			  //Task Priority
								 & (modem_dte->uart_event_task_hdl),	//Task Handler
								 xPortGetCoreID());
	MODEM_CHECK(ret == pdTRUE, "create uart event task failed", err_tsk_create);
	return modem_dte;
	/* Error handling */
err_tsk_create:
	vSemaphoreDelete(modem_dte->process_sem);
err_sem:
	uart_driver_delete(modem_dte->uart_port);
err_uart_config:
	free(modem_dte->buffer);
err_line_mem:
	free(modem_dte);
err_mem:
	return NULL;
}
static int ota_modem_dte_deinit(ota_modem_dte_t *modem_dte)
{
	if (!modem_dte)
	{
		ESP_LOGE(MODEM_TAG, "%s,empty dte ptr!",__func__);
		return -1;
	}
	/* Delete UART event task */
	if (modem_dte->uart_event_task_hdl)
	{
		vTaskDelete(modem_dte->uart_event_task_hdl);
	}
	/* Delete queue */
	if (modem_dte->event_queue)
	{
		vQueueDelete(modem_dte->event_queue);
	}
	/* Delete semaphores */
	if (modem_dte->process_sem)
	{
		vSemaphoreDelete(modem_dte->process_sem);
	}
	/* Uninstall UART Driver */
	uart_driver_delete(modem_dte->uart_port);
	/* Free memory */
	free(modem_dte->buffer);
	free(modem_dte);
	
    return 0;
}
static void ota_modem_msg_handle_power_up(void)
{
	if (s_ota_modem_cntx.dte==NULL)
	{
		s_ota_modem_cntx.dte = ota_modem_dte_init();
		assert(s_ota_modem_cntx.dte!=NULL);
		s_ota_modem_cntx.dte->received_cb = s_ota_modem_cntx.received_cb;
		s_ota_modem_cntx.dte->event_cb= s_ota_modem_cntx.event_cb;
		s_ota_modem_cntx.dte->line_handle = s_ota_modem_cntx.line_handle;
	}
	ota_modem_drv_power_up();
	if (ota_modem_drv_read_power_status())
	{
		s_ota_modem_cntx.timer_event = OTA_MODEM_TIMER_EVENT_WAIT_STABLE;
		ota_modem_start_timer(s_ota_modem_cntx.timer_handle,MODEM_CHECK_STABLE_DELAY_MS,false);
	}
	else
	{
		s_ota_modem_cntx.timer_event = OTA_MODEM_TIMER_EVENT_WAIT_POWERUP;
		ota_modem_start_timer(s_ota_modem_cntx.timer_handle,MODEM_CHECK_POWER_UP_DELAY_MS,false);
	}
}
static void ota_modem_msg_handle_power_down(void)
{
	ota_modem_drv_power_down();
	if (!ota_modem_drv_read_power_status())
	{
		ota_modem_send_msg(OTA_MODEM_MSG_UPDATE_POWER_STATUS,NULL,0);
	}
	else
	{
		s_ota_modem_cntx.timer_event = OTA_MODEM_TIMER_EVENT_WAIT_POWERDOWN;
		ota_modem_start_timer(s_ota_modem_cntx.timer_handle,MODEM_CHECK_POWER_DOWN_DELAY_MS,false);
	}
}
static void ota_modem_msg_handle_power_reset(void)
{
	ota_modem_drv_power_reset();
	if (ota_modem_drv_read_power_status())
	{
		s_ota_modem_cntx.timer_event = OTA_MODEM_TIMER_EVENT_WAIT_STABLE;
		ota_modem_start_timer(s_ota_modem_cntx.timer_handle,MODEM_CHECK_STABLE_DELAY_MS,false);
	}
	else
	{
		s_ota_modem_cntx.timer_event = OTA_MODEM_TIMER_EVENT_WAIT_POWERUP;
		ota_modem_start_timer(s_ota_modem_cntx.timer_handle,MODEM_CHECK_POWER_UP_DELAY_MS,false);
	}
}
static void ota_modem_msg_handle_wait_power_up(void)
{
	if (ota_modem_drv_read_power_status())
	{
		s_ota_modem_cntx.timer_event = OTA_MODEM_TIMER_EVENT_WAIT_STABLE;
		ota_modem_start_timer(s_ota_modem_cntx.timer_handle,MODEM_CHECK_STABLE_DELAY_MS,false);
	}
	else
	{
		//retry
		ota_modem_drv_power_up();
		s_ota_modem_cntx.timer_event = OTA_MODEM_TIMER_EVENT_WAIT_POWERUP;
		ota_modem_start_timer(s_ota_modem_cntx.timer_handle,MODEM_CHECK_POWER_UP_DELAY_MS,false);
	}
}
static void ota_modem_msg_handle_wait_power_down(void)
{
	if (!ota_modem_drv_read_power_status())
	{
		ota_modem_send_msg(OTA_MODEM_MSG_UPDATE_POWER_STATUS,NULL,0);
	}
	else
	{
		//retry
		ota_modem_drv_power_down();
		s_ota_modem_cntx.timer_event = OTA_MODEM_TIMER_EVENT_WAIT_POWERDOWN;
		ota_modem_start_timer(s_ota_modem_cntx.timer_handle,MODEM_CHECK_POWER_DOWN_DELAY_MS,false);
	}
}
static void ota_modem_msg_handle_update_power_status(void)
{
	if (s_ota_modem_cntx.is_power_up)
	{
		s_ota_modem_cntx.is_power_up = false;
		if (s_ota_modem_cntx.dte!=NULL)
		{
			ota_modem_dte_deinit(s_ota_modem_cntx.dte);
			s_ota_modem_cntx.dte = NULL;
		}
		ESP_LOGI(MODEM_TAG, "ota modem power down complete!");
		if (s_ota_modem_cntx.event_cb!=NULL)
		{
			s_ota_modem_cntx.event_cb(OTA_MODEM_EVENT_POWER_DOWN);
		}
	}
	else
	{
		s_ota_modem_cntx.is_power_up = true;
		ESP_LOGI(MODEM_TAG, "ota modem power up complete!");
		if (s_ota_modem_cntx.event_cb!=NULL)
		{
			s_ota_modem_cntx.event_cb(OTA_MODEM_EVENT_POWER_UP);
		}
	}
}
static void ota_modem_msg_handle_send_data(char *data, unsigned int length)
{
	ota_modem_dte_t*dte=s_ota_modem_cntx.dte;
	int ret =-1;

	if ((data!=NULL)&&(length>0))
	{
		ESP_LOGI(MODEM_TAG, "%s,data:%s,len:%d",__func__,data,length);
		if ((s_ota_modem_cntx.is_power_up)&&(dte!=NULL))
		{
			ret = uart_write_bytes(dte->uart_port, data, length);
			ESP_LOGI(MODEM_TAG, "%s,sent ret:%d",__func__,ret);
			if ((ret<0)&&(dte->received_cb!=NULL))
			{
				dte->received_cb(OTA_MODEM_RESULT_CODE_BUSY,strlen(OTA_MODEM_RESULT_CODE_BUSY));
			}
		}
		else
		{
			if (s_ota_modem_cntx.received_cb!=NULL)
			{
				s_ota_modem_cntx.received_cb(OTA_MODEM_RESULT_CODE_NO_CARRIER,strlen(OTA_MODEM_RESULT_CODE_NO_CARRIER));
			}
		}
	}
}
static void ota_modem_task(void *param)
{
	ota_modem_msg_t msg;
	
	while(1)
	{
		memset(&msg,0x00,sizeof(ota_modem_msg_t));        
	    if (xQueueReceive(s_ota_modem_cntx.queue, &msg, portMAX_DELAY) == pdTRUE) 
		{
			switch(msg.msg_id)
			{
				case OTA_MODEM_MSG_POWER_UP:
					ota_modem_msg_handle_power_up();
					break;
				case OTA_MODEM_MSG_POWER_DOWN:
					ota_modem_msg_handle_power_down();
					break;
				case OTA_MODEM_MSG_POWER_RESET:
					ota_modem_msg_handle_power_reset();
					break;
				case OTA_MODEM_MSG_WAIT_POWER_UP:
					ota_modem_msg_handle_wait_power_up();
					break;
				case OTA_MODEM_MSG_WAIT_POWER_DOWN:
					ota_modem_msg_handle_wait_power_down();
					break;
				case OTA_MODEM_MSG_UPDATE_POWER_STATUS:
					ota_modem_msg_handle_update_power_status();
					break;
				case OTA_MODEM_MSG_SEND_DATA:
					ota_modem_msg_handle_send_data(msg.param,msg.param_len);
					break;
				default:
					break;
			}

			if(msg.param!=NULL)
			{
				free(msg.param);
			}
	    }
	}
}
int ota_modem_init(void)
{
	memset(&s_ota_modem_cntx,0x00,sizeof(ota_modem_cntx_t));
	
	if (ota_modem_create_timer(&(s_ota_modem_cntx.timer_handle),(void*)&(s_ota_modem_cntx.timer_event))!=ESP_OK)
	{
		goto err_timer;
	}

	s_ota_modem_cntx.queue= xQueueCreate(OTA_MODEM_QUEUE_NUM,sizeof(ota_modem_msg_t));
	if (!s_ota_modem_cntx.queue)
	{																			  
		goto err_queue;
	}
	
	BaseType_t ret = xTaskCreatePinnedToCore(ota_modem_task, 			//Task Entry
								 OTA_MODEM_TASK_NAME,				//Task Name
								 OTA_MODEM_TASK_STACK_SIZE, 		  //Task Stack Size(Bytes)
								 NULL,							//Task Parameter
								 OTA_MODEM_TASK_PRIORITY,			  //Task Priority
								 NULL,	//Task Handler
								 xPortGetCoreID());
	MODEM_CHECK(ret == pdTRUE, "create ota modem task failed", err_tsk_create);

	s_ota_modem_cntx.dte = NULL;
	s_ota_modem_cntx.is_power_up = false;
	ota_modem_power_up();
	return 0;
err_tsk_create:
    vQueueDelete(s_ota_modem_cntx.queue);
	s_ota_modem_cntx.queue = NULL;
err_queue:
	ota_modem_delete_timer(s_ota_modem_cntx.timer_handle);
	s_ota_modem_cntx.timer_handle = NULL;
err_timer:
	return -1;
}
int ota_modem_register_callback(ota_modem_received_ind received_cb,ota_modem_event_ind event_cb)
{
	s_ota_modem_cntx.received_cb = received_cb;
	s_ota_modem_cntx.event_cb = event_cb;
	return 0;
}
int ota_modem_send_data(char *data, unsigned int length)
{
	if ((s_ota_modem_cntx.is_power_up)&&(s_ota_modem_cntx.dte!=NULL))
	{
		return ota_modem_send_msg(OTA_MODEM_MSG_SEND_DATA,data,length);
	}
	else
	{
		ESP_LOGI(MODEM_TAG, "%s,send data fail!",__func__);
    	return -1;
	}
}
int ota_modem_power_up(void)
{
	return ota_modem_send_msg(OTA_MODEM_MSG_POWER_UP,NULL,0);
}
int ota_modem_power_down(void)
{
	return ota_modem_send_msg(OTA_MODEM_MSG_POWER_DOWN,NULL,0);
}
int ota_modem_power_reset(void)
{
	return ota_modem_send_msg(OTA_MODEM_MSG_POWER_RESET,NULL,0);
}
