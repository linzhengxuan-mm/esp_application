/* To set the control period for DC motor Timer

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include "esp_hw_timer.h"
#include "esp_check.h"

#define ESP_HW_TIMER_INVALID_HANDLE		0xFF
#define ESP_HW_TIMER_DIVIDER         (16)  //  Hardware timer clock divider
#define ESP_HW_TIMER_SCALE           (TIMER_BASE_CLK / ESP_HW_TIMER_DIVIDER)  // convert counter value to seconds

static const char *TAG = "esp_hw_timer";
static uint8_t esp_timer_used[TIMER_GROUP_MAX][TIMER_MAX] = {0};

/**
 * @brief allocate timer handle
 */
static void esp_hw_timer_get_timer_from_handle(uint8_t handle,int*group_num,int*timer_num)
{
	*group_num=((handle&0xF0)>>4)&0x0F;
	*timer_num=handle&0x0F;
}
/**
 * @brief allocate timer handle
 */
static uint8_t esp_hw_timer_allocate_handle(void)
{
	int i,j;
	uint8_t handle=ESP_HW_TIMER_INVALID_HANDLE;

	for(i=0;i<TIMER_GROUP_MAX;i++)
	{
		for(j=0;j<TIMER_MAX;j++)
		{
			if(esp_timer_used[i][j]==0)
			{
				handle=((i<<4)&0xF0)|(j&0x0F);
				break;
			}
		}
	}
	return handle;
}

/**
 * @brief free timer handle
 */
static void esp_hw_timer_free_handle(uint8_t handle)
{
	int group_num,timer_num;
	
	esp_hw_timer_get_timer_from_handle(handle,&group_num,&timer_num);
	if((group_num>=TIMER_GROUP_MAX)||(timer_num>=TIMER_MAX))
	{
		return;
	}
	esp_timer_used[group_num][timer_num]=0x00;
}
/**
 * @brief Initialize the esp hw timer
 *
 * @param auto_reload
 * @param ctrl_period_ms esp hw period
 *  @return
 *      - ESP_OK: esp hw timer initialized successfully
 *      - ESP_FAIL: esp hw timer failed to initialize because of other errors
 */
uint8_t esp_hw_new_timer(bool auto_reload, double timer_interval_msec, timer_isr_handle_t *isr_handle,void *arg)
{
    esp_err_t ret = ESP_FAIL;
	uint8_t handle=ESP_HW_TIMER_INVALID_HANDLE;
	int group_num,timer_num;

	handle=esp_hw_timer_allocate_handle();
	if(handle==ESP_HW_TIMER_INVALID_HANDLE)
	{
		return handle;
	}
	esp_hw_timer_get_timer_from_handle(handle,&group_num,&timer_num);
	if((group_num>=TIMER_GROUP_MAX)||(timer_num>=TIMER_MAX))
	{
		return ESP_HW_TIMER_INVALID_HANDLE;
	}
	
    /* Select and initialize basic parameters of the timer */
    timer_config_t config;
    config.divider     = ESP_HW_TIMER_DIVIDER;
    config.counter_dir = TIMER_COUNT_UP;
    config.counter_en  = TIMER_PAUSE;
    config.alarm_en    = TIMER_ALARM_EN;
    config.intr_type   = TIMER_INTR_LEVEL;
    config.auto_reload = auto_reload;
    ret = timer_init(group_num, timer_num, &config);
    ESP_GOTO_ON_ERROR(ret, err,TAG, "timer init failed\n");
    /* Timer's counter will initially start from value below.
       Also, if auto_reload is set, this value will be automatically reload on alarm */
    timer_set_counter_value(group_num, timer_num, 0x00000000ULL);
    /* Configure the alarm value and the interrupt on alarm. */
    timer_set_alarm_value(group_num, timer_num, timer_interval_msec * ESP_HW_TIMER_SCALE / 1000);
    timer_enable_intr(group_num, timer_num);
    ret = timer_isr_register(group_num, timer_num, (void *)isr_handle, arg, ESP_INTR_FLAG_IRAM, NULL);
	ESP_GOTO_ON_ERROR(ret, err,TAG, "timer isr handle register failed\n");
    return handle;
err:
    timer_deinit(group_num, timer_num);
	esp_hw_timer_free_handle(handle);
    return ESP_HW_TIMER_INVALID_HANDLE;
}

/**
 * @brief Set timer alarm period
 *
 * @param period Timer alarm period
 * @return
 *      - void
 */
esp_err_t esp_hw_timer_set_period(uint8_t handle,double timer_interval_msec)
{
	int group_num,timer_num;
	
	esp_hw_timer_get_timer_from_handle(handle,&group_num,&timer_num);
	if((group_num>=TIMER_GROUP_MAX)||(timer_num>=TIMER_MAX))
	{
		return ESP_FAIL;
	}
    return timer_set_alarm_value(group_num, timer_num, timer_interval_msec * ESP_HW_TIMER_SCALE / 1000);
}

/**
 * @brief Start the timer
 */
esp_err_t esp_hw_timer_start(uint8_t handle)
{
    /* start the timer */
	int group_num,timer_num;
	
	esp_hw_timer_get_timer_from_handle(handle,&group_num,&timer_num);
	if((group_num>=TIMER_GROUP_MAX)||(timer_num>=TIMER_MAX))
	{
		return ESP_FAIL;
	}
    return timer_start(group_num, timer_num);
}


/**
 * @brief Pause the timer and clear the counting value
 */
esp_err_t esp_hw_timer_stop(uint8_t handle)
{
    /* stop the timer */
	int group_num,timer_num;
	
	esp_hw_timer_get_timer_from_handle(handle,&group_num,&timer_num);
	if((group_num>=TIMER_GROUP_MAX)||(timer_num>=TIMER_MAX))
	{
		return ESP_FAIL;
	}
    timer_pause(group_num, timer_num);
    return timer_set_counter_value(group_num, timer_num, 0);
}

/**
 * @brief Deinitialize the timer
 *
 * @param timer_info the secondary pointer of esp_hw_timer_info_t, the memory will be freed
 */
esp_err_t esp_hw_timer_deinit(uint8_t handle)
{
    /* stop the timer */
	int group_num,timer_num;
	
	esp_hw_timer_get_timer_from_handle(handle,&group_num,&timer_num);
	if((group_num>=TIMER_GROUP_MAX)||(timer_num>=TIMER_MAX))
	{
		return ESP_FAIL;
	}
    return timer_deinit(group_num, timer_num);
}
