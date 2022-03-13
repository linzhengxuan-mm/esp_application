/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "hal/cpu_hal.h" // for cpu_hal_get_cycle_count()
#include "esp_compiler.h"
#ifndef LOG_LOCAL_LEVEL
#define LOG_LOCAL_LEVEL  5
#endif
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_err.h"
#include "ota_log.h"

static const char *OTA_TAG = "ota";

static ota_log_level_t ota_log_level=OTA_LOG_INFO;

void ota_log_init(void)
{
	esp_log_level_set("*", OTA_LOG_INFO);
	esp_log_level_set(OTA_TAG, OTA_LOG_VERBOSE);
}
void ota_log_level_set(ota_log_level_t level)
{
	ota_log_level=level;
}
ota_log_level_t ota_log_get_level(void)
{
	return ota_log_level;
}
void ota_log_write(ota_log_level_t level,const char *format, ...)
{
    va_list list;
    va_start(list, format);
    esp_log_writev((esp_log_level_t)level, OTA_TAG, format, list);
    va_end(list);
}
void ota_log_buffer_hex_internal(const void *buffer, unsigned short buff_len, ota_log_level_t level)
{
	esp_log_buffer_hex_internal(OTA_TAG,buffer,buff_len,(esp_log_level_t)level);
}
void ota_log_buffer_char_internal(const void *buffer, unsigned short buff_len, ota_log_level_t level)
{
	esp_log_buffer_char_internal(OTA_TAG,buffer,buff_len,(esp_log_level_t)level);
}
void ota_log_buffer_hexdump_internal(const void *buffer, unsigned short buff_len, ota_log_level_t log_level)
{
	esp_log_buffer_hexdump_internal(OTA_TAG,buffer,buff_len,(esp_log_level_t)log_level);
}
char *ota_log_system_timestamp(void)
{
    static char buffer[18] = {0};
    static _lock_t bufferLock = 0;

    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        unsigned int timestamp = ota_log_early_timestamp();
        for (uint8_t i = 0; i < sizeof(buffer); i++) {
            if ((timestamp > 0) || (i == 0)) {
                for (uint8_t j = sizeof(buffer) - 1; j > 0; j--) {
                    buffer[j] = buffer[j - 1];
                }
                buffer[0] = (char)(timestamp % 10) + '0';
                timestamp /= 10;
            } else {
                buffer[i] = 0;
                break;
            }
        }
        return buffer;
    } else {
        struct timeval tv;
        struct tm timeinfo;

        gettimeofday(&tv, NULL);
        localtime_r(&tv.tv_sec, &timeinfo);

        _lock_acquire(&bufferLock);
        snprintf(buffer, sizeof(buffer),
                 "%02d:%02d:%02d.%03ld",
                 timeinfo.tm_hour,
                 timeinfo.tm_min,
                 timeinfo.tm_sec,
                 tv.tv_usec / 1000);
        _lock_release(&bufferLock);

        return buffer;
    }
}

unsigned int ota_log_timestamp(void)
{
    if (unlikely(xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED)) {
        return ota_log_early_timestamp();
    }
    static unsigned int base = 0;
    if (base == 0 && xPortGetCoreID() == 0) {
        base = ota_log_early_timestamp();
    }
    TickType_t tick_count = xPortInIsrContext() ? xTaskGetTickCountFromISR() : xTaskGetTickCount();
    return base + tick_count * (1000 / configTICK_RATE_HZ);
}

/* FIXME: define an API for getting the timestamp in soc/hal IDF-2351 */
unsigned int ota_log_early_timestamp(void)
{
#if CONFIG_IDF_TARGET_ESP32
    /* ESP32 ROM stores separate clock rate values for each CPU, but we want the PRO CPU value always */
    extern unsigned int g_ticks_per_us_pro;
    return cpu_hal_get_cycle_count() / (g_ticks_per_us_pro * 1000);
#else
    return cpu_hal_get_cycle_count() / (ets_get_cpu_frequency() * 1000);
#endif
}
