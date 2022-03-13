// Copyright 2018 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef __OTA_LOG_H__
#define __OTA_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OTA_LOG_NONE,       /*!< No log output */
    OTA_LOG_ERROR,      /*!< Critical errors, software module can not recover on its own */
    OTA_LOG_WARN,       /*!< Error conditions from which recovery measures have been taken */
    OTA_LOG_INFO,       /*!< Information messages which describe normal flow of events */
    OTA_LOG_DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    OTA_LOG_VERBOSE     /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} ota_log_level_t;

void ota_log_init(void);
void ota_log_level_set(ota_log_level_t level);
ota_log_level_t ota_log_get_level(void);
void ota_log_write(ota_log_level_t level,const char *format, ...);
char *ota_log_system_timestamp(void);
unsigned int ota_log_timestamp(void);
unsigned int ota_log_early_timestamp(void);
void ota_log_buffer_hex_internal(const void *buffer, unsigned short buff_len, ota_log_level_t level);
void ota_log_buffer_char_internal(const void *buffer, unsigned short buff_len, ota_log_level_t level);
void ota_log_buffer_hexdump_internal(const void *buffer, unsigned short buff_len, ota_log_level_t log_level);



#define OTA_LOG_FORMAT(letter, format)  "[" #letter "]" "(%u)%s: " format "\n"
#define OTA_LOG_SYSTEM_TIME_FORMAT(letter, format)  "[" #letter "]" "(%s)%s: " format "\n"

#if 1
#define OTA_LOG_LEVEL(level,format, ...) do {                     \
        if (level==OTA_LOG_ERROR )          { ota_log_write(OTA_LOG_ERROR,      OTA_LOG_SYSTEM_TIME_FORMAT(E, format), ota_log_system_timestamp(), __func__, ##__VA_ARGS__); } \
        else if (level==OTA_LOG_WARN )      { ota_log_write(OTA_LOG_WARN,       OTA_LOG_SYSTEM_TIME_FORMAT(W, format), ota_log_system_timestamp(), __func__,  ##__VA_ARGS__); } \
        else if (level==OTA_LOG_DEBUG )     { ota_log_write(OTA_LOG_DEBUG,      OTA_LOG_SYSTEM_TIME_FORMAT(D, format), ota_log_system_timestamp(), __func__, ##__VA_ARGS__); } \
        else if (level==OTA_LOG_VERBOSE )   { ota_log_write(OTA_LOG_VERBOSE,    OTA_LOG_SYSTEM_TIME_FORMAT(V, format), ota_log_system_timestamp(), __func__, ##__VA_ARGS__); } \
        else                                { ota_log_write(OTA_LOG_INFO,       OTA_LOG_SYSTEM_TIME_FORMAT(I, format), ota_log_system_timestamp(), __func__, ##__VA_ARGS__); } \
    } while(0)
#else
#define OTA_LOG_LEVEL(level,format, ...) do {                     \
        if (level==OTA_LOG_ERROR )          { ota_log_write(OTA_LOG_ERROR,      OTA_LOG_FORMAT(E, format), ota_log_timestamp(), __func__, ##__VA_ARGS__); } \
        else if (level==OTA_LOG_WARN )      { ota_log_write(OTA_LOG_WARN,       OTA_LOG_FORMAT(W, format), ota_log_timestamp(), __func__, ##__VA_ARGS__); } \
        else if (level==OTA_LOG_DEBUG )     { ota_log_write(OTA_LOG_DEBUG,      OTA_LOG_FORMAT(D, format), ota_log_timestamp(), __func__, ##__VA_ARGS__); } \
        else if (level==OTA_LOG_VERBOSE )   { ota_log_write(OTA_LOG_VERBOSE,    OTA_LOG_FORMAT(V, format), ota_log_timestamp(), __func__, ##__VA_ARGS__); } \
        else                                { ota_log_write(OTA_LOG_INFO,       OTA_LOG_FORMAT(I, format), ota_log_timestamp(), __func__, ##__VA_ARGS__); } \
    } while(0)
#endif

#define OTA_LOG_LEVEL_LOCAL(level, format, ...) do {               \
        if ( ota_log_get_level() >= level ) OTA_LOG_LEVEL(level, format, ##__VA_ARGS__); \
    } while(0)
    
#define OTA_LOGE( format, ... ) OTA_LOG_LEVEL_LOCAL(OTA_LOG_ERROR,   format, ##__VA_ARGS__)
#define OTA_LOGW( format, ... ) OTA_LOG_LEVEL_LOCAL(OTA_LOG_WARN,    format, ##__VA_ARGS__)
#define OTA_LOGI( format, ... ) OTA_LOG_LEVEL_LOCAL(OTA_LOG_INFO,    format, ##__VA_ARGS__)
#define OTA_LOGD( format, ... ) OTA_LOG_LEVEL_LOCAL(OTA_LOG_DEBUG,   format, ##__VA_ARGS__)
#define OTA_LOGV( format, ... ) OTA_LOG_LEVEL_LOCAL(OTA_LOG_VERBOSE, format, ##__VA_ARGS__)


#define OTA_LOG_BUFFER_HEX_LEVEL(buffer, buff_len, level ) \
    do {\
        if ( ota_log_get_level() >= (level) ) { \
            ota_log_buffer_hex_internal(buffer, buff_len, level ); \
        } \
    } while(0)

#define OTA_LOG_BUFFER_CHAR_LEVEL(buffer, buff_len, level ) \
    do {\
        if ( ota_log_get_level() >= (level) ) { \
            ota_log_buffer_char_internal( buffer, buff_len, level ); \
        } \
    } while(0)



#define OTA_LOG_BUFFER_HEXDUMP( buffer, buff_len, level ) \
    do { \
        if ( ota_log_get_level() >= (level) ) { \
            ota_log_buffer_hexdump_internal( buffer, buff_len, level); \
        } \
    } while(0)

#define OTA_LOG_BUFFER_HEX(buffer, buff_len) \
    do { \
        if (ota_log_get_level() >= OTA_LOG_INFO) { \
            OTA_LOG_BUFFER_HEX_LEVEL( buffer, buff_len, OTA_LOG_INFO ); \
        }\
    } while(0)

#define OTA_LOG_BUFFER_CHAR( buffer, buff_len) \
    do { \
        if (ota_log_get_level() >= OTA_LOG_INFO) { \
            OTA_LOG_BUFFER_CHAR_LEVEL( buffer, buff_len, OTA_LOG_INFO ); \
        }\
    } while(0)

#ifdef __cplusplus
}
#endif
#endif

