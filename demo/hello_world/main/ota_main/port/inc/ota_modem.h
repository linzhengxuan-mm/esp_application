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
#ifndef __OTA_MODEM_H__
#define __OTA_MODEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#define OTA_MODEM_RESULT_CODE_SUCCESS 		"OK"              /*!< Acknowledges execution of a command */
#define OTA_MODEM_RESULT_CODE_CONNECT 		"CONNECT"         /*!< A connection has been established */
#define OTA_MODEM_RESULT_CODE_RING 			"RING"               /*!< Detect an incoming call signal from network */
#define OTA_MODEM_RESULT_CODE_NO_CARRIER 	"NO CARRIER"   /*!< Connection termincated or establish a connection failed */
#define OTA_MODEM_RESULT_CODE_ERROR 		"ERROR"             /*!< Command not recognized, command line maximum length exceeded, parameter value invalid */
#define OTA_MODEM_RESULT_CODE_NO_DIALTONE 	"NO DIALTONE" /*!< No dial tone detected */
#define OTA_MODEM_RESULT_CODE_BUSY 			"BUSY"               /*!< Engaged signal detected */
#define OTA_MODEM_RESULT_CODE_NO_ANSWER 	"NO ANSWER"     /*!< Wait for quiet answer */

typedef enum 
{
	OTA_MODEM_EVENT_POWER_UP,     				/*!< ota modem power on event*/
	OTA_MODEM_EVENT_POWER_DOWN,	 				/*!< ota modem power off event*/
	OTA_MODEM_EVENT_COM_ERROR,	 				/*!< ota modem communication error event*/
	OTA_MODEM_EVENT_MAX,         				/*!< ota modem event max index*/
} ota_modem_event_type;

typedef int (*ota_modem_received_ind)(void *buffer, unsigned int len);
typedef void (*ota_modem_event_ind)(ota_modem_event_type event);
typedef int (*ota_modem_line_handle)(const char *line);

int ota_modem_init(void);
int ota_modem_register_callback(ota_modem_received_ind received_cb,ota_modem_event_ind event_cb);
int ota_modem_send_data(char *data, unsigned int length);

int ota_modem_power_up(void);
int ota_modem_power_down(void);
int ota_modem_reset(void);
#ifdef __cplusplus
}
#endif
#endif

