/* Console example — NVS commands

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_err.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "custom_console.h"
#include "app_events.h"
#include "modem_board.h"

#define APP_CONSOLE_CMD_PARAM_COUNT			10
#define APP_CONSOLE_CMD_DEFAULT_PWD 		"0000"

static const char *TAG = "app_console_cmd";

static struct {
    struct arg_str *cmd_value;
    struct arg_end *end;
} app_console_cmd_args;

typedef int (* app_console_cmd_cb_fun_t) (void **arg,uint16_t argc);

typedef struct
{
	char*cmd_type;
	app_console_cmd_cb_fun_t cmd_fun;
}app_console_cmd_item_info;

static int app_console_log_cmd_handle(void **arg,uint16_t argc);
static int app_console_modem_cmd_handle(void **arg,uint16_t argc);

const app_console_cmd_item_info app_console_cmd_table[]=
{
	{"LOG",app_console_log_cmd_handle},
	{"MODEM",app_console_modem_cmd_handle},

	//please add above
	{NULL,NULL}
};
/**
 * @brief Strip the tailed "\r\n"
 *
 * @param str string to strip
 * @param len length of string
 */
static void strip_cmd_cr_lf_tail(char *str, uint32_t len)
{
    if (str[len - 2] == '\r') {
        str[len - 2] = '\0';
    } else if (str[len - 1] == '\r') {
        str[len - 1] = '\0';
    }
}
static void split_cmd(char *src,const char *separator,char **dest,int *num) 
{
     char *pNext;
     int count = 0;
     if (src == NULL || strlen(src) == 0) 
        return;
     if (separator == NULL || strlen(separator) == 0) 
        return;
     pNext = (char *)strtok(src,separator); 
     while(pNext != NULL) {
          *dest++ = pNext;
          ++count;
         pNext = (char *)strtok(NULL,separator);
    }
    *num = count;
}
static int app_console_log_cmd_handle(void **arg,uint16_t argc)
{
	char**pArg = (char**)arg;
	
	if((!pArg)||(!(*pArg)))
	{
		ESP_LOGE(TAG,"[LOG]no param");
		return 1;
	}

	ESP_LOGI(TAG,"[LOG]param is %s",(char*)(*pArg));

	int log_level = atoi((char*)(*pArg));

	if (log_level<= ESP_LOG_VERBOSE)
	{
		esp_log_level_set("*", log_level);
		custom_console_cmd_reply("LOG,OK\r\n");
		return 0;
	}
	else
	{
		ESP_LOGE(TAG,"[LOG]invalid param value");
		return 1;
	}
}
static int app_console_modem_cmd_handle(void **arg,uint16_t argc)
{
	char modem_cmd[256];
	char**pArg = (char**)arg;
	int count=argc;
	
	if((!pArg)||(!(*pArg)))
	{
		ESP_LOGE(TAG,"[MODEM]no param");
		return 1;
	}

	memset(modem_cmd,0x00,sizeof(modem_cmd));
	strcpy(modem_cmd,(char*)(*pArg));
	count--;
	
	do
	{
		if(count>0)
		{
			pArg++;
			if (*pArg)
			{
				strcat(modem_cmd,",");
				strcat(modem_cmd,(char*)(*pArg));
			}
		}
		count--;
	}while(count>0);

	strcat(modem_cmd,"\r\n");
	ESP_LOGI(TAG,"[MODEM]cmd is %s",modem_cmd);

	if (modem_board_send_cmd(modem_cmd,1000,custom_console_cmd_reply)!=ESP_OK)
	{
		custom_console_cmd_reply("ERROR");
	}
	return 0;
}

static app_console_cmd_item_info* app_console_cmd_find_item(char *cmd_type)
{
	app_console_cmd_item_info*pInfo=app_console_cmd_table;
	
	do
	{
		if(!pInfo)
		{
			break;
		}
		if((!pInfo->cmd_type)||(!pInfo->cmd_fun))
		{
			pInfo = NULL;
			break;
		}

		if (strcmp(cmd_type,pInfo->cmd_type)==0)
		{
			break;
		}
		
		pInfo++;
	}while(1);

	return pInfo;
}

static int app_console_cmd_execute(char *cmd_type,void**arg,uint16_t argc)
{
	app_console_cmd_item_info*cmd_item_info=app_console_cmd_find_item(cmd_type);

	if (!cmd_item_info)
	{
		ESP_LOGE(TAG,"cmd no support");
		return 1;
	}

	if(cmd_item_info->cmd_fun)
	{
		return cmd_item_info->cmd_fun(arg,argc);
	}
	else
	{
		ESP_LOGE(TAG,"cmd no execute fun");
		return 1;
	}
}
static int app_console_cmd_process(const char *cmd_str)
{
	char*pData=NULL;
	uint16_t len=0;
	char *split_result[APP_CONSOLE_CMD_PARAM_COUNT] = {0};
	int num = 0;
	int ret =0;
	
	//cmd format:<cmd>,<pwd>[,param]#
	if((!cmd_str)||(strlen(cmd_str)<=2))
	{
		ESP_LOGE(TAG,"invalid cmd data");
		goto error;
	}
	len = strlen(cmd_str);
	len++;
	pData = (char*)calloc(1, len);
	if (!pData)
	{
		ESP_LOGE(TAG,"calloc memory fail");
		goto error;
	}
	memset(pData,0x00,len);
	memcpy(pData,cmd_str,len);
	strip_cmd_cr_lf_tail(pData,strlen(pData));
	len = strlen(pData);
	if (pData[len-1]!='#')
	{
		ESP_LOGE(TAG,"invalid cmd format,tail error");
		goto error;
	}
	else
	{
		pData[len-1] = '\0';
	}
	split_cmd(pData,",",split_result,&num);
	if (num<2)
	{
		ESP_LOGE(TAG,"invalid cmd format,no pwd");
		goto error;
	}
	
	ESP_LOGI(TAG,"cmd tpye:%s,pwd:%s",split_result[0],split_result[1]);
	if ((strlen(split_result[1])==0)||(strcmp(split_result[1],APP_CONSOLE_CMD_DEFAULT_PWD)!=0))
	{
		ESP_LOGE(TAG,"invalid pwd");
		goto error;
	}
	//to do cmd process
	if ((num-2)>0)
	{
		ret = app_console_cmd_execute(split_result[0],(void**)(split_result+2),num-2);
	}
	else
	{
		ret = app_console_cmd_execute(split_result[0],NULL,0);
	}
	//
	if(pData)
	{
		free(pData);
	}
	return ret;
error:
	if(pData)
	{
		free(pData);
	}
	return 1;
}
static int app_console_cmd_handler(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **) &app_console_cmd_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, app_console_cmd_args.end, argv[0]);
        return 1;
    }
    const char *key = app_console_cmd_args.cmd_value->sval[0];

	if(key)
	{
		ESP_LOGI(TAG,"console app cmd received:%s",key);
		return app_console_cmd_process(key);
	}
	else
	{
        return 1;
	}
}
void app_console_cmd_register(void)
{
	app_console_cmd_args.cmd_value = arg_str1(NULL, NULL, "<cmd_value>", "cmd full string");
	app_console_cmd_args.end = arg_end(2);

	const esp_console_cmd_t app_cmd = {
	    .command = "AT$OTAATCMD",
	    .help = "app at cmd.\n"
	    "Examples:\n"
	    " AT$OTAATCMD=\"cmd,pwd[,arg]#\"\n",
	    .hint = NULL,
	    .func = &app_console_cmd_handler,
	    .argtable = &app_console_cmd_args
	};
	custom_console_register_cmd(&app_cmd);
}
