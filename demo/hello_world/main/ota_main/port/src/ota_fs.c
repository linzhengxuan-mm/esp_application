/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <utime.h>
#include "dirent.h"
#include "unity.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_system.h"

#include "fat.h"
#include "ota_log.h"
#include "ota_fs.h"

static WCHAR * _Construct_FileName(const WCHAR * FileName,const WCHAR * BasePath)
{
	INT32U len=0;
	WCHAR *ConFileName=NULL;
	
	if (FileName)
	{
		len = strlen(FileName)+strlen(BasePath)+1;

		ConFileName = (WCHAR *)malloc(len);
		if(ConFileName)
		{
			memset(ConFileName,0x00,len);
			strcpy(ConFileName,BasePath);
			strcat(ConFileName,FileName);
			
			return ConFileName;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}
static void _Deconstruct_FileName(const WCHAR * FileName)
{
	if(FileName)
	{
		free(FileName);
	}
}
static int _EmptyDir(char *destDir)  
{  
    char sub_file_name[256];
    DIR *dp;  
    struct dirent *entry;  
    struct stat statbuf;  
    if ((dp = opendir(destDir)) == NULL)  
    {  
		//OTA_LOGE("cannot open directory: %s\n", destDir);
		return -1;  
    }  
    while ((entry = readdir(dp)) != NULL)  
    {  
    	memset(sub_file_name,0x00,sizeof(sub_file_name));
		strcpy(sub_file_name,destDir);
		strcat(sub_file_name,"/");
		strcat(sub_file_name,entry->d_name);
		
        stat(sub_file_name, &statbuf);  
        if (S_ISREG(statbuf.st_mode))
        {  
            remove(sub_file_name);  
        }
        else if (S_ISDIR(statbuf.st_mode))
        {  
            _EmptyDir(sub_file_name);
			rmdir(sub_file_name);
        }
		else
		{
			OTA_LOGE("invalid file type");
		}
    }
	closedir(dp);
    return 0;
}  

FS_HANDLE FS_Open(const WCHAR * FileName, INT32U Flag)
{
	FS_HANDLE fd=FS_FILE_INVALID_HANDLE;
	CHAR mode[8];
	WCHAR *file_name = _Construct_FileName(FileName,MOUNT_PATH);

	if (NULL==file_name)
	{
		OTA_LOGE("invalid file name!");
		return NULL;
	}
	
	memset(mode,0x00,sizeof(mode));
	if (Flag&FS_O_CREAT_E)
	{
		fd = fopen(file_name,"rb");
		if (fd == FS_FILE_INVALID_HANDLE) 
		{
			fd = fopen(file_name,"wb");
			if (fd == FS_FILE_INVALID_HANDLE) 
			{
				OTA_LOGE("file create error[%s]!",strerror(errno));
				goto error;
			}
		}
		else
		{
			OTA_LOGI("file exist");
		}
		fclose(fd);
	}

	if (Flag&FS_O_RDONLY_E)
	{
		strcpy(mode,"rb");
	}
	else if (Flag&FS_O_WRONLY_E)
	{
		strcpy(mode,"wb");
	}
	else
	{
		strcpy(mode,"rb+");
	}

    fd = fopen(file_name,mode);
    if (fd == FS_FILE_INVALID_HANDLE) 
	{
		OTA_LOGE("error[%s]!",strerror(errno));
		goto error;
    }
	else
	{
		if (Flag&FS_O_RDONLY_E)
		{
			fseek(fd, 0, SEEK_SET);
		}
		else
		{
			fseek(fd, 0, SEEK_END);
		}
	}
error:
	_Deconstruct_FileName(file_name);
	if(fd!=FS_FILE_INVALID_HANDLE)
	{
		fat_calc_magic_number(true);
	}
	else
	{
		fat_calc_magic_number(false);
	}
	return fd;
}
INT32S FS_Close(FS_HANDLE FileHandle)
{
	INT32S ret=-1;
	if (FileHandle!=FS_FILE_INVALID_HANDLE)
	{
	    ret = fclose(FileHandle);
		if (ret < 0)
		{
			OTA_LOGE("error[%s]!",strerror(errno));
		}
		return ret;
	}
	else
	{
		return -1;
	}
}
INT32S FS_Delete(const WCHAR * FileName)
{
	INT32S ret=-1;
	WCHAR *file_name = _Construct_FileName(FileName,MOUNT_PATH);

	if (file_name)
	{
		ret = access(file_name,F_OK);
		if (ret!=-1)
		{
			ret = remove(file_name);
			if (ret < 0)
			{
				OTA_LOGE("error[%s]!",strerror(errno));
			}
		}
		else
		{
			OTA_LOGE("%s,not exist",file_name);
		}
		_Deconstruct_FileName(file_name);
		return ret;
	}
	else
	{
		return -1;
	}
}
INT32S FS_Read(FS_HANDLE FileHandle, void * DataPtr, INT32U Length, INT32U * Read)
{
	INT32S readlen = 0;

	
	if ((FileHandle==FS_FILE_INVALID_HANDLE)||(DataPtr==NULL))
	{
		return -1;
	}
	
	*Read = 0;
	while(Length > FILE_BULK_SIZE)
	{
		readlen = 0;
		readlen = fread(DataPtr + *Read,1,FILE_BULK_SIZE,FileHandle);
		if(!readlen)
		{
			return -1;
		}
		*Read += readlen;
		Length -= readlen;
	}
	if(Length)
	{
		readlen = 0;
		readlen = fread(DataPtr + *Read,1,Length,FileHandle);
		if(!readlen)
		{
			return -1;
		}
		*Read += readlen;
	}
	return 0;
}

INT32S FS_Write(FS_HANDLE FileHandle, void * DataPtr, INT32U Length, INT32U * Written)
{
	INT32S writelen = 0;

	if ((FileHandle==FS_FILE_INVALID_HANDLE)||(DataPtr==NULL))
	{
		return -1;
	}
	
	*Written = 0;
	while(Length > FILE_BULK_SIZE)
	{
		writelen = 0;
		writelen = fwrite(DataPtr + *Written,1,FILE_BULK_SIZE,FileHandle);
		if(!writelen)
		{
			return -1;
		}
		*Written += writelen;
		Length -= writelen;
	}
	if(Length)
	{
		writelen = 0;
		writelen = fwrite(DataPtr + *Written,1,Length,FileHandle);
		if(!writelen)
		{
			return -1;
		}
		*Written += writelen;
	}
	return 0;
}

INT32S FS_Stat(const WCHAR * FileName, FS_STAT *pstat)
{
	INT32S ret=-1;
	WCHAR *file_name = _Construct_FileName(FileName,MOUNT_PATH);

	if(file_name)
	{
		ret = access(file_name,F_OK);
		if (ret!=-1)
		{
			ret = stat(file_name,pstat);
			if (ret < 0)
			{
				OTA_LOGE("error[%s]!",strerror(errno));
			}
		}
		else
		{
			OTA_LOGE("%s,not exist",file_name);
		}
		_Deconstruct_FileName(file_name);
		return ret;
	}
	else
	{
		return -1;
	}
}
INT32S FS_RemoveDir(const WCHAR * DirName)
{
	INT32S ret=-1;
	WCHAR *dir_name = _Construct_FileName(DirName,MOUNT_PATH);

	if(dir_name)
	{
		ret = access(dir_name,F_OK);
		if (ret!=-1)
		{
			_EmptyDir(dir_name);
			ret = rmdir(dir_name);
			if (ret < 0)
			{
				OTA_LOGE("error[%s]!",strerror(errno));
			}
		}
		else
		{
			OTA_LOGE("%s,not exist",dir_name);
		}
		_Deconstruct_FileName(dir_name);
		return ret;
	}
	else
	{
		return -1;
	}
}
INT32S FS_Rename(const WCHAR * FileName, const WCHAR * NewName)
{
	INT32S ret=-1;
	WCHAR *file_name = _Construct_FileName(FileName,MOUNT_PATH);
	WCHAR *file_new_name = _Construct_FileName(NewName,MOUNT_PATH);

	if((file_name)&&(file_new_name))
	{
		if(access(file_name,F_OK)!=-1)
		{
			ret = rename(file_name, file_new_name);
			if (ret < 0)
			{
				OTA_LOGE("error[%s]!",strerror(errno));
			}
		}
		else
		{
			OTA_LOGE("%s,not exist",file_name);
		}
	}
	_Deconstruct_FileName(file_name);
	_Deconstruct_FileName(file_new_name);
	return ret;
}
INT32S FS_Seek(FS_HANDLE FileHandle, INT32S Offset, INT32S Whence)
{
	if (FileHandle!=FS_FILE_INVALID_HANDLE)
	{
		return fseek(FileHandle, Offset, Whence);
	}
	else
	{
		return -1;
	}
}
INT32S FS_CreateDir(const WCHAR * DirName)
{
	INT32S ret=-1;
	WCHAR *dir_name = _Construct_FileName(DirName,MOUNT_PATH);

	if(dir_name)
	{
		ret = access(dir_name,F_OK);
		if(ret<0)
		{
			ret = mkdir(dir_name, 0755);
			if(ret==0)
			{
				fat_calc_magic_number(true);
			}
			else
			{
				fat_calc_magic_number(false);
				OTA_LOGE("error[%s]!",strerror(errno));
			}
		}
		_Deconstruct_FileName(dir_name);
		return ret;
	}
	else
	{
		return -1;
	}
}
INT32S FS_GetFileSize(FS_HANDLE FileHandle, INT32U * Size)
{
	if (FileHandle!=FS_FILE_INVALID_HANDLE)
	{
		fseek(FileHandle, 0, SEEK_END);
		*Size = ftell(FileHandle);
		return 0;
	}
	else
	{
		return -1;
	}
}
INT32S FS_GetFileSizeWithName(const WCHAR * FileName, INT32U * Size)
{
	INT32S ret=-1;
    struct stat st={0};
	WCHAR *file_name = _Construct_FileName(FileName,MOUNT_PATH);

	if(NULL==file_name)
	{
		return -1;
	}

	if((access(file_name,F_OK)!=-1)&&(stat(file_name, &st) == 0))
	{
		*Size = st.st_size;
		ret=0;
	}
	else
	{
		*Size = 0;
		ret=-1;
	}
	_Deconstruct_FileName(file_name);
	return ret;
}
INT32S FS_Truncate(const WCHAR *FileName, INT32S file_size)
{
	INT32S ret=-1;
	WCHAR *file_name = _Construct_FileName(FileName,MOUNT_PATH);

	if(NULL==file_name)
	{
		return -1;
	}
	
	ret = truncate(file_name, file_size);
	if (ret < 0)
	{
		OTA_LOGE("error[%s]!",strerror(errno));
	}
	_Deconstruct_FileName(file_name);
	return ret;
}
INT32S FS_SetAttributes(const WCHAR * FileName, INT8U Attributes)
{
	return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ota_fs_mount(void)
{
	if (fat_mount()<0)
	{
		fat_unmount();
		if (fat_mount()<0)
		{
			return OTA_FS_ERROR_MOUNT;
		}
		else
		{
			return OTA_FS_STATE_OK;
		}
	}

	return OTA_FS_STATE_OK;
}
int ota_fs_format(void)
{
    if (fat_format()==0)
    {
		fat_unmount();
		if (fat_mount()<0)
		{
			return OTA_FS_ERROR_MOUNT;
		}
		else
		{
			return OTA_FS_STATE_OK;
		}
	}
	else
	{
		return OTA_FS_ERROR_FORMAT;
	}
}
int ota_fs_unmount(void)
{
	return fat_unmount();
}
int ota_fs_usage(void)
{
    size_t out_total_bytes, out_free_bytes;
	
	fat_get_usage(&out_total_bytes,&out_free_bytes);
	return out_total_bytes;
}
int ota_fs_freespace(void)
{
    size_t out_total_bytes, out_free_bytes;
	
	fat_get_usage(&out_total_bytes,&out_free_bytes);
	return out_free_bytes;
}
int ota_fs_chipname(char *name)
{
	return -1;

}
int ota_fmount(void)
{
	return fat_mount();
}
int ota_funmount(void)
{
	return fat_unmount();
}
int ota_fformat(void)
{
    return fat_format();
}
int ota_fopen(OTA_FIL* fp, const char* path, unsigned int mode)
{
	CHAR mode_string[8];
	char *file_name = _Construct_FileName(path,MOUNT_PATH);

	if (NULL==file_name)
	{
		OTA_LOGE("invalid file name!");
		return -1;
	}
	
	memset(mode_string,0x00,sizeof(mode_string));
	if (mode&FS_O_CREAT_E)
	{
		*fp = fopen(file_name,"rb");
		if (*fp == NULL) 
		{
			*fp = fopen(file_name,"wb");
			if (*fp == NULL) 
			{
				OTA_LOGE("file create error[%s]!",strerror(errno));
				goto error;
			}
		}
		else
		{
			OTA_LOGI("file exist");
		}
		fclose(*fp);
	}

	if (mode&FS_O_RDONLY_E)
	{
		strcpy(mode_string,"rb");
	}
	else if (mode&FS_O_WRONLY_E)
	{
		strcpy(mode_string,"wb");
	}
	else
	{
		strcpy(mode_string,"rb+");
	}

	*fp = fopen(file_name,mode_string);
	if (*fp== NULL) 
	{
		OTA_LOGE("error[%s]!",strerror(errno));
		goto error;
	}
	else
	{
		if (mode&FS_O_RDONLY_E)
		{
			fseek(*fp, 0, SEEK_SET);
		}
		else
		{
			fseek(*fp, 0, SEEK_END);
		}
	}
error:
	_Deconstruct_FileName(file_name);
	if(*fp!=NULL)
	{
		fat_calc_magic_number(true);
		return 0;
	}
	else
	{
		fat_calc_magic_number(false);
		return -1;
	}
}

int ota_fclose(OTA_FIL* fp)
{
	int ret=-1;
	
	if (*fp!=NULL)
	{
		ret = fclose(*fp);
		if (ret < 0)
		{
			OTA_LOGE("error[%s]!",strerror(errno));
		}
	}
	
	return ret;
}

int ota_fread(OTA_FIL* fp, void* buff, unsigned int btr, unsigned int* br)
{
	int readlen = 0;

	if ((*fp==NULL)||(buff==NULL))
	{
		return -1;
	}
	
	readlen = fread(buff,1,btr,*fp);
    if(readlen >= 0)
    {
        if(br)
        {
			*br = readlen;
        }
        return 0;
    }
	else
	{
		OTA_LOGE("error[%s]!",strerror(errno));
		return -1;
	}
}
int ota_fwrite(OTA_FIL* fp, const void* buff, unsigned int btw, unsigned int* bw)
{
	int writelen = 0;

	if ((*fp==NULL)||(buff==NULL))
	{
		return -1;
	}

	writelen = fwrite(buff,1,btw,*fp);
    if(writelen >= 0)
    {
        if(bw) 
		{
			*bw = writelen;
        }
        return 0;
    }
	else
	{
		OTA_LOGE("error[%s]!",strerror(errno));
        return -1;
	}
}
int ota_fsize(OTA_FIL* fp)
{	
	if (*fp!=NULL)
	{
		fseek(*fp, 0, SEEK_END);
		return ftell(*fp);
	}
	else
	{
		return -1;
	}
}
int ota_flseek(OTA_FIL* fp, int ofs)
{
	if (*fp!=NULL)
	{
		return fseek(*fp, ofs, SEEK_SET);
	}
	else
	{
		return -1;
	}
}
int ota_ftruncate(const char* path,INT32S file_size)
{
	INT32S ret=-1;
	char *file_name = _Construct_FileName(path,MOUNT_PATH);
	
	ret = truncate(file_name, file_size);
	if (ret < 0)
	{
		OTA_LOGE("error[%s]!",strerror(errno));
	}
	_Deconstruct_FileName(file_name);
	return ret;
}

int ota_fsync(OTA_FIL* fp)
{
    //return lfs_file_sync(&m_tft_lfs, fp);
	return -1;
}
int ota_fopendir(OTA_DIR* dp, const char* path)
{
	int ret=-1;
	char *dir_name = _Construct_FileName(path,MOUNT_PATH);

	if(NULL == dir_name)
	{
		return -1;
	}
	
    dp = opendir(dir_name);
	if (dp!=NULL)
	{
		ret=0;
	}

	_Deconstruct_FileName(dir_name);
	return ret;
}
int ota_fclosedir(OTA_DIR* dp)
{
	return closedir(dp);
}
int ota_freaddir(OTA_DIR* dp, OTA_FILINFO* fno)
{
	struct dirent *entry;  

	entry = readdir(dp);
	if (entry!=NULL)
	{
		if(fno)
		{
			fno->type = entry->d_type;
			fno->size = entry->d_ino;
			strcpy(fno->name,entry->d_name);
		}
		return 0;
	}
	else
	{
		return -1;
	}
}
int ota_frewinddir(OTA_DIR* dp)
{
    rewinddir(dp);
	return 0;
}
int ota_fnextdir(OTA_DIR* dp)
{
	return -1;
}
int ota_fmkdir(const char* path)
{
	int ret=-1;
	char *dir_name = _Construct_FileName(path,MOUNT_PATH);

	if(dir_name)
	{
		ret = access(dir_name,F_OK);
		if(ret<0)
		{
			ret = mkdir(dir_name, 0755);
			if(ret==0)
			{
				fat_calc_magic_number(true);
			}
			else
			{
				fat_calc_magic_number(false);
				OTA_LOGE("error[%s]!",strerror(errno));
			}
		}
		_Deconstruct_FileName(dir_name);
		return ret;
	}
	else
	{
		return -1;
	}
}
int ota_funlink(const char* path)
{
	INT32S ret=-1;
	char *file_name = _Construct_FileName(path,MOUNT_PATH);

	if (file_name)
	{
		if (_EmptyDir(file_name)<0)
		{
			ret = access(file_name,F_OK);
			if (ret!=-1)
			{
				ret = remove(file_name);
				if (ret < 0)
				{
					OTA_LOGE("error[%s]!",strerror(errno));
				}
			}
			else
			{
				OTA_LOGE("%s,not exist",file_name);
			}
			_Deconstruct_FileName(file_name);
			return ret;
		}
		else
		{
			_Deconstruct_FileName(file_name);
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

int ota_frename(const char* path_old, const char* path_new)
{
	int ret=-1;
	char *file_name = _Construct_FileName(path_old,MOUNT_PATH);
	char *file_new_name = _Construct_FileName(path_new,MOUNT_PATH);

	if((file_name)&&(file_new_name))
	{
		if(access(file_name,F_OK)!=-1)
		{
			ret = rename(file_name, file_new_name);
			if (ret < 0)
			{
				OTA_LOGE("error[%s]!",strerror(errno));
			}
		}
		else
		{
			OTA_LOGE("%s,not exist",file_name);
		}
	}
	_Deconstruct_FileName(file_name);
	_Deconstruct_FileName(file_new_name);
	return ret;
}

int ota_fstat(const char* path, OTA_FILINFO* fno)
{
	INT32S ret=-1;
    struct stat statbuf;  
	char *file_name = _Construct_FileName(path,MOUNT_PATH);
	
	if (file_name)
	{
		ret = access(file_name,F_OK);
		if (ret!=-1)
		{
			ret = stat(file_name, &statbuf);
			if (ret < 0)
			{
				OTA_LOGE("error[%s]!",strerror(errno));
			}

			if(fno)
			{
				fno->size = statbuf.st_size;
				fno->type = statbuf.st_mode;
			}
		}
		else
		{
			OTA_LOGE("%s,not exist",file_name);
		}
		_Deconstruct_FileName(file_name);
		return ret;
	}
	else
	{
		return -1;
	}
}
int ota_fexist(const char* path)
{
	INT32S ret=-1;
	char *file_name = _Construct_FileName(path,MOUNT_PATH);

	if(access(file_name,F_OK)!=-1)
	{
		ret=0;
	}
	_Deconstruct_FileName(file_name);
	return ret;
}
