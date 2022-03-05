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
#include "esp_log.h"
#include "esp_err.h"

#include "ota_main.h"

static const char *TAG = "ota_main";

void ota_fs_api_test(void)
{
	FS_HANDLE fd;
    INT32U write_size = 0;
    INT32U read_size = 0;
    INT32U file_size = 0;
    WCHAR filepath[OTA_FILE_PATH_MAXLEN] = {0};
    WCHAR rename_filepath[OTA_FILE_PATH_MAXLEN] = {0};
	INT32S ret =0;


	if(FS_CreateDir("/datatx")<0)
	{
		ESP_LOGE(TAG,"dir:/datatx,create fail");
	}

	if (FS_CreateDir("/datatx/ota_upg")<0)
	{
		ESP_LOGE(TAG,"dir:/datatx/ota_upg,create exist");
	}
	//test 1 /datatx/hello.txt
	{
	    ota_wsprintf((WCHAR *)filepath,"/%s/%s", "datatx", "hello.txt");
		FS_GetFileSizeWithName(filepath,&file_size);
		ESP_LOGI(TAG,"%s,file_size:%d",filepath,file_size);
		fd = FS_Open(filepath,FS_CREATE|FS_READ_WRITE);
	    if(fd==FS_FILE_INVALID_HANDLE)
	    {
			ESP_LOGE(TAG,"%s,create/read/write open fail",filepath);
	    	return;
	    }
		const char filedata[] = "ABCDEFGHIJ";
		//FS_Seek(fd,0,FS_FILE_BEGIN);
		FS_Write(fd, (void*)filedata, strlen(filedata), &write_size);
		FS_GetFileSize(fd,&file_size);
		ESP_LOGI(TAG,"write_size:%d,file_size:%d",write_size,file_size);
		FS_Close(fd);

		//read test
		fd = FS_Open(filepath,FS_READ_ONLY);
	    if(fd==FS_FILE_INVALID_HANDLE)
	    {
			ESP_LOGE(TAG,"%s,read open fail",filepath);
	    	return;
		}
		char read_buf[16]={0};
		//FS_Seek(fd,0,FS_FILE_BEGIN);
		FS_Read(fd, (void*)read_buf, strlen(filedata), &read_size);
		ESP_LOGI(TAG,"read_size:%d,read content:%s",read_size,read_buf);
		FS_Close(fd);


		//rename
	    ota_wsprintf((WCHAR *)rename_filepath,"/%s/%s", "datatx", "hello2.txt");
		if(FS_Rename(filepath,rename_filepath)<0)
		{
			ESP_LOGE(TAG,"%s,rename fail",filepath);
			//FS_Delete(filepath);
		}
		else
		{
			FS_GetFileSizeWithName(rename_filepath,&file_size);
			ESP_LOGI(TAG,"%s,file_size:%d",rename_filepath,file_size);
			//FS_Delete(rename_filepath);
		}
	}

	//test2 /datatx/ota_upg/test.txt
	{
		memset(filepath,0x00,sizeof(filepath));
		ota_wsprintf((WCHAR *)filepath,"/%s/%s/%s", "datatx", "ota_upg","test.txt");
		FS_GetFileSizeWithName(filepath,&file_size);
		ESP_LOGI(TAG,"%s,file_size:%d",filepath,file_size);
		
		fd = FS_Open(filepath,FS_CREATE|FS_READ_WRITE);
		if(fd<0)
		{
			ESP_LOGE(TAG,"%s,create/read/write open fail",filepath);
	    	return;
		}
		const char filedata2[] = "0123456789";
		//FS_Seek(fd,0,FS_FILE_BEGIN);
		FS_Write(fd, (void*)filedata2, strlen(filedata2), &write_size);
		
		FS_GetFileSize(fd,&file_size);
		ESP_LOGI(TAG,"write_size:%d,file_size:%d",write_size,file_size);
		FS_Close(fd);

		//read test
		fd = FS_Open(filepath,FS_READ_ONLY);
	    if(fd==FS_FILE_INVALID_HANDLE)
	    {
			ESP_LOGE(TAG,"%s,read open fail",filepath);
	    	return;
		}
		char read_buf[16]={0};
		//FS_Seek(fd,0,FS_FILE_BEGIN);
		FS_Read(fd, (void*)read_buf, strlen(filedata2), &read_size);
		ESP_LOGI(TAG,"read_size:%d,read content:%s",read_size,read_buf);
		FS_Close(fd);

		
		//rename
		memset(rename_filepath,0x00,sizeof(rename_filepath));
		ota_wsprintf((WCHAR *)rename_filepath,"/%s/%s/%s", "datatx", "ota_upg","test2.txt");
		if(FS_Rename(filepath,rename_filepath)<0)
		{
			ESP_LOGE(TAG,"%s,rename fail",filepath);
			//FS_Delete(filepath);
		}
		else
		{
			FS_GetFileSizeWithName(rename_filepath,&file_size);
			ESP_LOGI(TAG,"%s,file_size:%d",rename_filepath,file_size);
			//FS_Delete(rename_filepath);
		}
	}

	#if 0
	if (FS_RemoveDir("/datatx/ota_upg")<0)
	{
		ESP_LOGE(TAG,"dir:/datatx/ota_upg,remove fail");
	}
	#endif
	
	if (FS_RemoveDir("/datatx")<0)
	{
		ESP_LOGE(TAG,"dir:/datatx,remove fail");
	}
}

void ota_main_init(void)
{
	ota_timer_handler_init();

	ota_fs_api_test();
}
