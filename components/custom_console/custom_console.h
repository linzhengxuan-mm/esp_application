/* Console example — declarations of command registration functions.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#ifndef __CUSTOM_CONSOLE_H__
#define __CUSTOM_CONSOLE_H__


#include "esp_console.h"

#ifdef __cplusplus
extern "C" {
#endif

void custom_console_init(void);
void custom_console_register_cmd(const esp_console_cmd_t *cmd);
esp_err_t custom_console_cmd_reply(const char*cmd_str);

#ifdef __cplusplus
}
#endif

#endif
