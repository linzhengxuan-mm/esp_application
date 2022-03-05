/* Console example — declarations of command registration functions.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#ifndef __TASK_DEF_H__
#define __TASK_DEF_H__

#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_system.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (* task_common_cb_fun_t) (void *param,uint16_t param_len);

typedef struct {
	uint16_t msg_id;
    void *param;
    uint16_t param_len;
	task_common_cb_fun_t cb_fun;
} task_common_msg_t;


#define COMMON_TASK_QUEUE_CREATE(q,q_s,t_f,t_n,t_s,t_p,t_c,tag,e)                                 \
    do                                                                                \
    {                                                                                 \
		q = xQueueCreate(q_s,sizeof(task_common_msg_t));                                            \
		if (!q)                                                                       \
		{                                                                             \
			ESP_LOGE(tag, "task queue create fail");                                  \
			return e;                                                                 \
		}                                                                              \
		BaseType_t ret = xTaskCreatePinnedToCore(t_f, t_n, t_s, NULL, t_p, NULL, t_c); \
		if (ret != pdTRUE)                                                             \
 		{                                                                              \
			ESP_LOGE(tag, "task create fail");                                  \
			return e;                                                                  \
		}                                                                              \
    } while (0)


#define COMMON_TASK_CREATE(t_f,t_n,t_s,t_p,t_c,tag,e)                                 \
    do                                                                                \
    {                                                                                 \
		BaseType_t ret = xTaskCreatePinnedToCore(t_f, t_n, t_s, NULL, t_p, NULL, t_c); \
		if (ret != pdTRUE)                                                             \
 		{                                                                              \
			ESP_LOGE(tag, "task create fail");                                  		\
			return e;                                                                  \
		}                                                                              \
    } while (0)

#define COMMON_TASK_RUN(q,fun)                              \
	while(1) 												\
	{                                                       \
		task_common_msg_t msg;                              \
		memset(&msg,0x00,sizeof(task_common_msg_t));        \
	    if (xQueueReceive(q, &msg, portMAX_DELAY) == pdTRUE) \
		{                                                    \
			fun(&msg);                                       \
	    }                                                    \
	}

#define DEFINE_COMMON_TASK_SEND_MSG_FUN(name, queue,tag) \
	static esp_err_t name(uint16_t msg_id, void *param, uint32_t parma_len,task_common_cb_fun_t fun_cb) \
	{ \
	    task_common_msg_t msg; \
	    BaseType_t ret,xHigherPriorityTaskWoken = pdFALSE; \
	    if (!queue) \
	    {\
			ESP_LOGE(tag, "send msg fail,queue is null");\
			return ESP_FAIL;\
		} \
		msg.msg_id = msg_id;  \
		msg.cb_fun = fun_cb;  \
		if ((param)&&(parma_len>0)) \
		{  \
			msg.param = calloc(1, parma_len);  \
			if(msg.param)  \
			{  \
				memcpy(msg.param,param,parma_len);  \
				msg.param_len = parma_len;  \
			}  \
			else \
			{ \
				ESP_LOGE(tag, "send msg fail,param calloc fail");\
				return ESP_FAIL;  \
			} \
		} \
		else \
		{  \
			msg.param = NULL;  \
			msg.param_len = 0;   \
		} \
		if( xPortInIsrContext() == pdTRUE ) \
		{ \
			ret = xQueueSendFromISR(queue, &msg, &xHigherPriorityTaskWoken); \
			if ( xHigherPriorityTaskWoken ) \
			{ \
				portYIELD_FROM_ISR(); \
			}\
		} \
		else \
		{ \
			ret = xQueueSend(queue, &msg, pdMS_TO_TICKS(200)); \
		} \
		if (ret != pdTRUE) \
		{ \
			ESP_LOGE(tag, "send msg fail");\
			if (param)	\
			{ \
				free(param);  \
			}  \
			return ESP_FAIL; \
		}\
	    return ESP_OK; \
	}


#define APP_TASK_PRIORITY_BASE		10
#define APP_TASK_DEFALUT_COREID		0

#define APP_MAIN_TASK_NAME			"app_main_task"
#define APP_MAIN_TASK_STACK_SIZE	4096
#define APP_MAIN_TASK_PRIORITY		APP_TASK_PRIORITY_BASE
#define APP_MAIN_TASK_COREID		APP_TASK_DEFALUT_COREID

#define MODEM_BOARD_TASK_NAME		"modem_board_task"
#define MODEM_BOARD_TASK_STACK_SIZE	4096
#define MODEM_BOARD_TASK_PRIORITY	APP_TASK_PRIORITY_BASE
#define MODEM_BOARD_TASK_COREID		APP_TASK_DEFALUT_COREID

#ifdef __cplusplus
}
#endif

#endif

