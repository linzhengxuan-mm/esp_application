/* To set the control period for DC motor Timer

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/timer.h"

/**
 * @brief Initialize the esp hw timer
 *
 */
uint8_t esp_hw_new_timer(bool auto_reload, double timer_interval_msec, timer_isr_handle_t *isr_handle,void *arg);

/**
 * @brief Set timer alarm period
 *
 * @param period Timer alarm period
 */
esp_err_t esp_hw_timer_set_period(uint8_t handle,double timer_interval_msec);

/**
 * @brief Start the timer
 */
esp_err_t esp_hw_timer_start(uint8_t handle);


/**
 * @brief Pause the timer and clear the counting value
 */
esp_err_t esp_hw_timer_stop(uint8_t handle);

/**
 * @brief Deinitialize the timer
 *
 * @param timer_info the secondary pointer of esp_hw_timer_info_t, the memory will be freed
 */
esp_err_t esp_hw_timer_deinit(uint8_t handle);

#ifdef __cplusplus
}
#endif
