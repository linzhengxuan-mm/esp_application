/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "diskio_wl.h"
#include "esp_vfs_fat.h"
#include "fat.h"


static const char *TAG = "APP_FAT";

#define FAT_PARTITON_LABEL "storage"
#define FAT_MAGIC_MAX_NUMBER 10
static wl_handle_t fat_wl_handle=WL_INVALID_HANDLE;
static uint8_t fat_magic_number =0;

static int fat_mount_internal(const char* base_path,const char* partition_label,wl_handle_t* pwl_handle)
{
    ESP_LOGI(TAG, "Mounting FAT filesystem");
    const esp_vfs_fat_mount_config_t mount_config = {
            .max_files = 4,
            .format_if_mount_failed = true,
            .allocation_unit_size = CONFIG_WL_SECTOR_SIZE
    };
    esp_err_t err = esp_vfs_fat_spiflash_mount(base_path, partition_label, &mount_config, pwl_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount %s FATFS (%s)", base_path,esp_err_to_name(err));
        return -1;
    }
    return 0;
}
static int fat_unmount_internal(const char* base_path,wl_handle_t wl_handle)
{
	esp_err_t err = esp_vfs_fat_spiflash_unmount(base_path,wl_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to unmount FATFS (%s)", esp_err_to_name(err));
        return -1;
    }
    return 0;
}
static int fat_get_usage_internal(wl_handle_t wl_handle,size_t* out_total_bytes, size_t* out_free_bytes)
{
    FATFS *fs;
    size_t free_clusters;

    BYTE pdrv = ff_diskio_get_pdrv_wl(wl_handle);
    if (pdrv == 0xff) {
        ESP_LOGE(TAG, "invalid pdrv");
        return -1;
    }
	
    char drv[3] = {(char)('0' + pdrv), ':', 0};
    int res = f_getfree(drv, &free_clusters, &fs);
    assert(res == FR_OK);

    size_t total_sectors = (fs->n_fatent - 2) * fs->csize;
    size_t free_sectors = free_clusters * fs->csize;
    // assuming the total size is < 4GiB, should be true for SPI Flash
    if (out_total_bytes != NULL) {
        *out_total_bytes = total_sectors * fs->ssize;
    }
    if (out_free_bytes != NULL) {
        *out_free_bytes = free_sectors * fs->ssize;
    }
	return 0;
}

static int recover_fat_internal(const char *base_path,const char* partition_label,wl_handle_t*pwl_handle)
{
	if (*pwl_handle != WL_INVALID_HANDLE)
	{
		if (fat_unmount_internal(base_path,*pwl_handle)<0) 
		{
			ESP_LOGE(TAG,"file system unmount fail");
			return -1;
		}
		*pwl_handle = WL_INVALID_HANDLE;
		vTaskDelay(200 / portTICK_PERIOD_MS);		
	}	
	return fat_mount_internal(base_path,partition_label,pwl_handle);
}
int fat_init(void)
{
    size_t total_bytes, free_bytes;

	if (fat_mount_internal(MOUNT_PATH,FAT_PARTITON_LABEL,&fat_wl_handle)!=-1)
	{
		if(fat_get_usage_internal(fat_wl_handle,&total_bytes,&free_bytes)!=-1)
		{
			ESP_LOGI(TAG, "FAT FS: %d kB total, %d kB free", total_bytes/1024, free_bytes/ 1024);
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}
void fat_calc_magic_number(bool is_true)
{
	if(is_true)
	{	
		fat_magic_number=0;
	}
	else
	{
		fat_magic_number++;
		if(fat_magic_number>=FAT_MAGIC_MAX_NUMBER)
		{
			ESP_LOGE(TAG,"file system fatal error,will recover");
			if (recover_fat_internal(MOUNT_PATH,FAT_PARTITON_LABEL,&fat_wl_handle)!=-1)
			{
				fat_magic_number = 0;
				ESP_LOGI(TAG,"file system recover done");
			}
			else
			{
				ESP_LOGE(TAG,"file system recover fail");
			}
		}
	}
}
int fat_mount(void)
{
	if (fat_wl_handle==WL_INVALID_HANDLE)
	{
		return fat_mount_internal(MOUNT_PATH,FAT_PARTITON_LABEL,&fat_wl_handle);
	}
	else
	{
		return 0;
	}
}
int fat_unmount(void)
{
	if (fat_wl_handle!=WL_INVALID_HANDLE)
	{
		if (fat_unmount_internal(MOUNT_PATH,fat_wl_handle)<0)
		{
			return -1;
		}
		else
		{
			fat_wl_handle = WL_INVALID_HANDLE;
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
int fat_format(void)
{
    const size_t workbuf_size = 4096;
    void *workbuf = NULL;
	FRESULT fresult;

	if (fat_wl_handle!=WL_INVALID_HANDLE)
	{
		BYTE pdrv = ff_diskio_get_pdrv_wl(fat_wl_handle);
		if (pdrv == 0xff) 
		{
			ESP_LOGE(TAG, "invalid pdrv");
            goto fail;
		}
		char drv[3] = {(char)('0' + pdrv), ':', 0};

        workbuf = calloc(1,workbuf_size);
        if (workbuf == NULL) 
		{
			ESP_LOGE(TAG, "no enough memory");
            goto fail;
        }
        fresult = f_mkfs(drv, FM_ANY | FM_SFD, CONFIG_WL_SECTOR_SIZE, workbuf, workbuf_size);
        if (fresult != FR_OK) {
            ESP_LOGE(TAG, "f_mkfs failed (%d)", fresult);
            goto fail;
        }
        free(workbuf);
		return 0;
	}
fail:
	free(workbuf);
	return -1;
}
int fat_get_usage(size_t* total_bytes, size_t* free_bytes)
{
    size_t out_total_bytes, out_free_bytes;
	
	if (fat_wl_handle!=WL_INVALID_HANDLE)
	{
		if(fat_get_usage_internal(fat_wl_handle,&out_total_bytes,&out_free_bytes)!=-1)
		{
			*total_bytes = out_total_bytes;
			*free_bytes = out_free_bytes;
			return 0;
		}
		else
		{
			*total_bytes = 0;
			*free_bytes = 0;
			return -1;
		}
	}
	else
	{
		*total_bytes = 0;
		*free_bytes = 0;
		return -1;
	}
}
