/* Console example — declarations of command registration functions.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#ifndef __CUSTOM_LOG_H__
#define __CUSTOM_LOG_H__

#include <stdint.h>
#include <stdarg.h>
#include "sdkconfig.h"
#include "esp_log.h"

#ifdef __cplusplus
extern "C" {
#endif


#define CUSTOM_LOG_LEVEL_LOCAL(level, tag, format, ...) do {               \
        if ( custom_log_get_level() >= level ) ESP_LOG_LEVEL(level, tag, format, ##__VA_ARGS__); \
    } while(0)

#define CLOGE( tag, format, ... ) CUSTOM_LOG_LEVEL_LOCAL(ESP_LOG_ERROR,   tag, format, ##__VA_ARGS__)
#define CLOGW( tag, format, ... ) CUSTOM_LOG_LEVEL_LOCAL(ESP_LOG_WARN,    tag, format, ##__VA_ARGS__)
#define CLOGI( tag, format, ... ) CUSTOM_LOG_LEVEL_LOCAL(ESP_LOG_INFO,    tag, format, ##__VA_ARGS__)
#define CLOGD( tag, format, ... ) CUSTOM_LOG_LEVEL_LOCAL(ESP_LOG_DEBUG,   tag, format, ##__VA_ARGS__)
#define CLOGV( tag, format, ... ) CUSTOM_LOG_LEVEL_LOCAL(ESP_LOG_VERBOSE, tag, format, ##__VA_ARGS__)

esp_log_level_t custom_log_get_level(void);
void custom_log_set_level(esp_log_level_t level);
void initialize_custom_log(void);

#ifdef __cplusplus
}
#endif

#endif
