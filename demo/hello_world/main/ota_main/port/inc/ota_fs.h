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
#ifndef __OTA_FS_H__
#define __OTA_FS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "dirent.h"
#include "pub_datatype.h"


/** Flag bits to open a file. */
enum{
    FS_O_RDONLY_E  = 1,  /**< Open for read only. */
    FS_O_WRONLY_E  = 2,  /**< Open for write only. */
    FS_O_RDWR_E    = 4,  /**< Open for read and write. */
    FS_O_CREAT_E   = 8  /**< Create the file if it does not exist. */
};
#define FS_OPEN_DIR             0x00000800L
//-- FS_Open flags
#define FS_CREATE               FS_O_CREAT_E
/* The file is opened for read and write access. */
#define FS_READ_WRITE           FS_O_RDWR_E
/* The file is opened for read only access. */
#define FS_READ_ONLY            FS_O_RDONLY_E
/* The file is opened for write only access. */
#define FS_WRITE_ONLY           FS_O_WRONLY_E


/* FS_Seek Parameter */

/* seek begin. */
#define FS_FILE_BEGIN          	SEEK_SET
/* seek current. */
#define FS_FILE_CURRENT        	SEEK_CUR
/* seek end. */
#define FS_FILE_END        		SEEK_END

//-- Type definitions
#define FS_FILE_INVALID_HANDLE		NULL

#define FILE_BULK_SIZE      (32*1024)

typedef FILE* FS_HANDLE;

typedef struct stat FS_STAT;

/*---------------------------------------------------------------------------
 * Error Codes
 *---------------------------------------------------------------------------*/
typedef enum
{
    FS_NO_ERROR                    =    0,    /* No error */
    FS_ERROR_RESERVED              =   -1,    /* Reserved error, not used currently */
    FS_PARAM_ERROR                 =   -2,    /* User */
    FS_INVALID_FILENAME            =   -3,    /* User */
    FS_DRIVE_NOT_FOUND             =   -4,    /* User or Fatal */
    FS_TOO_MANY_FILES              =   -5,    /* User or Normal: use over max file handle number or more than 256 files in sort */
    FS_NO_MORE_FILES               =   -6,    /* Normal */
    FS_WRONG_MEDIA                 =   -7,    /* Fatal */
    FS_INVALID_FILE_SYSTEM         =   -8,    /* Fatal */
    FS_FILE_NOT_FOUND              =   -9,    /* User or Normal */
    FS_INVALID_FILE_HANDLE         =  -10,    /* User or Normal */
    FS_UNSUPPORTED_DEVICE          =  -11,    /* User */
    FS_UNSUPPORTED_DRIVER_FUNCTION =  -12,    /* User or Fatal */
    FS_CORRUPTED_PARTITION_TABLE   =  -13,    /* fatal */
    FS_TOO_MANY_DRIVES             =  -14,    /* not use so far */
    FS_INVALID_FILE_POS            =  -15,    /* User */
    FS_ACCESS_DENIED               =  -16,    /* User or Normal */
    FS_STRING_BUFFER_TOO_SMALL     =  -17,    /* User */
    FS_GENERAL_FAILURE             =  -18,    /* Normal */
    FS_PATH_NOT_FOUND              =  -19,    /* User */
    FS_FAT_ALLOC_ERROR             =  -20,    /* Fatal: disk crash */
    FS_ROOT_DIR_FULL               =  -21,    /* Normal */
    FS_DISK_FULL                   =  -22,    /* Normal */
    FS_TIMEOUT                     =  -23,    /* Normal: FS_CloseMSDC with nonblock */
    FS_BAD_SECTOR                  =  -24,    /* Normal: NAND flash bad block */
    FS_DATA_ERROR                  =  -25,    /* Normal: NAND flash bad block */
    FS_MEDIA_CHANGED               =  -26,    /* Normal */
    FS_SECTOR_NOT_FOUND            =  -27,    /* Fatal */
    FS_ADDRESS_MARK_NOT_FOUND      =  -28,    /* not use so far */
    FS_DRIVE_NOT_READY             =  -29,    /* Normal */
    FS_WRITE_PROTECTION            =  -30,    /* Normal: only for MSDC */
    FS_DMA_OVERRUN                 =  -31,    /* not use so far */
    FS_CRC_ERROR                   =  -32,    /* not use so far */
    FS_DEVICE_RESOURCE_ERROR       =  -33,    /* Fatal: Device crash */
    FS_INVALID_SECTOR_SIZE         =  -34,    /* Fatal */
    FS_OUT_OF_BUFFERS              =  -35,    /* Fatal */
    FS_FILE_EXISTS                 =  -36,    /* User or Normal */
    FS_LONG_FILE_POS               =  -37,    /* User : FS_Seek new pos over sizeof INT32S */
    FS_FILE_TOO_LARGE              =  -38,    /* User: filesize + pos over sizeof INT32S */
    FS_BAD_DIR_ENTRY               =  -39,    /* Fatal */
    FS_ATTR_CONFLICT               =  -40,    /* User: Can't specify FS_PROTECTION_MODE and FS_NONBLOCK_MOD */
    FS_CHECKDISK_RETRY             =  -41,    /* System: don't care */
    FS_LACK_OF_PROTECTION_SPACE    =  -42,    /* Fatal: Device crash */
    FS_SYSTEM_CRASH                =  -43,    /* Normal */
    FS_FAIL_GET_MEM                =  -44,    /* Normal */
    FS_READ_ONLY_ERROR             =  -45,    /* User or Normal */
    FS_DEVICE_BUSY                 =  -46,    /* Normal */
    FS_ABORTED_ERROR               =  -47,    /* Normal */
    FS_QUOTA_OVER_DISK_SPACE       =  -48,    /* Normal: Configuration Mistake */
    FS_PATH_OVER_LEN_ERROR         =  -49,    /* Normal */
    FS_APP_QUOTA_FULL              =  -50,    /* Normal */
    FS_VF_MAP_ERROR                =  -51,    /* User or Normal */
    FS_DEVICE_EXPORTED_ERROR       =  -52,    /* User or Normal */
    FS_DISK_FRAGMENT               =  -53,    /* Normal */
    FS_DIRCACHE_EXPIRED            =  -54,    /* Normal */
    FS_QUOTA_USAGE_WARNING         =  -55,    /* Normal or Fatal: System Drive Free Space Not Enought */
    FS_ERR_DIRDATA_LOCKED          =  -56,    /* Normal */
    FS_INVALID_OPERATION           =  -57,    /* Normal */
    FS_ERR_VF_PARENT_CLOSED        =  -58,    /* Virtual file's parent is closed */
    FS_ERR_UNSUPPORTED_SERVICE     =  -59,    /* The specified FS service is closed in this project. */

    FS_ERR_INVALID_JOB_ID          =  -81,

    FS_MSDC_MOUNT_ERROR            = -100,    /* Normal */
    FS_MSDC_READ_SECTOR_ERROR      = -101,    /* Normal */
    FS_MSDC_WRITE_SECTOR_ERROR     = -102,    /* Normal */
    FS_MSDC_DISCARD_SECTOR_ERROR   = -103,    /* Normal */
    FS_MSDC_PRESNET_NOT_READY      = -104,    /* System */
    FS_MSDC_NOT_PRESENT            = -105,    /* Normal */

    FS_EXTERNAL_DEVICE_NOT_PRESENT = -106,    /* Normal */
    FS_HIGH_LEVEL_FORMAT_ERROR     = -107,    /* Normal */

    FS_CARD_BATCHCOUNT_NOT_PRESENT = -110,    /* Normal */

    FS_FLASH_MOUNT_ERROR           = -120,    /* Normal */
    FS_FLASH_ERASE_BUSY            = -121,    /* Normal: only for nonblock mode */
    FS_NAND_DEVICE_NOT_SUPPORTED   = -122,    /* Normal: Configuration Mistake */
    FS_FLASH_OTP_UNKNOWERR         = -123,    /* User or Normal */
    FS_FLASH_OTP_OVERSCOPE         = -124,    /* User or Normal */
    FS_FLASH_OTP_WRITEFAIL         = -125,    /* User or Normal */
    FS_FDM_VERSION_MISMATCH        = -126,    /* System */
    FS_FLASH_OTP_LOCK_ALREADY      = -127,    /* User or Normal */
    FS_FDM_FORMAT_ERROR            = -128,    /* The format of the disk content is not correct */

    FS_FDM_USER_DRIVE_BROKEN       = -129,    /*User drive unrecoverable broken*/
    FS_FDM_SYS_DRIVE_BROKEN        = -130,    /*System drive unrecoverable broken*/
    FS_FDM_MULTIPLE_BROKEN         = -131,    /*multiple unrecoverable broken*/

    FS_LOCK_MUTEX_FAIL             = -141,    /* System: don't care */
    FS_NO_NONBLOCKMODE             = -142,    /* User: try to call nonblock mode other than NOR flash */
    FS_NO_PROTECTIONMODE           = -143,    /* User: try to call protection mode other than NOR flash */

    /*
     * If disk size exceeds FS_MAX_DISK_SIZE (unit is KB, defined in makefile),
     * FS_TestMSDC(), FS_GetDevStatus(FS_MOUNT_STATE_ENUM) and all access behaviors will
     * get this error code.
     */
    FS_DISK_SIZE_TOO_LARGE         = (FS_MSDC_MOUNT_ERROR),     /*Normal*/

    FS_MINIMUM_ERROR_CODE          = -65536 /* 0xFFFF0000 */
} fs_error_enum;

FS_HANDLE FS_Open(const WCHAR * FileName, INT32U Flag);
INT32S FS_Close(FS_HANDLE FileHandle);
INT32S FS_Delete(const WCHAR * FileName);
INT32S FS_Read(FS_HANDLE FileHandle, void * DataPtr, INT32U Length, INT32U * Read);
INT32S FS_Write(FS_HANDLE FileHandle, void * DataPtr, INT32U Length, INT32U * Written);
INT32S FS_Stat(const WCHAR * FileName, FS_STAT *pstat);
INT32S FS_RemoveDir(const WCHAR * DirName);
INT32S FS_Rename(const WCHAR * FileName, const WCHAR * NewName);
INT32S FS_Seek(FS_HANDLE FileHandle, INT32S Offset, INT32S Whence);
INT32S FS_CreateDir(const WCHAR * DirName);
INT32S FS_GetFileSize(FS_HANDLE FileHandle, INT32U * Size);
INT32S FS_GetFileSizeWithName(const WCHAR * FileName, INT32U * Size);
INT32S FS_Truncate(FS_HANDLE FileHandle, const CHAR *FileName, INT32S file_size);
INT32S FS_SetAttributes(const WCHAR * FileName, INT8U Attributes);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FILE OTA_FIL;

typedef DIR OTA_DIR;


typedef struct
{
    // Type of the file, either LFS_TYPE_REG or LFS_TYPE_DIR
    unsigned char type;

    // Size of the file, only valid for REG files. Limited to 32-bits.
    unsigned int size;

    // Name of the file stored as a null-terminated string. Limited to
    // LFS_NAME_MAX+1, which can be changed by redefining LFS_NAME_MAX to
    // reduce RAM. LFS_NAME_MAX is stored in superblock and must be
    // respected by other littlefs drivers.
    char name[255];
}OTA_FILINFO;

#define OTA_FS_ERROR_MOUNT		-3	/*无法挂载*/
#define OTA_FS_ERROR_DEVICE		-2	/*FLASH不识别*/
#define OTA_FS_ERROR_FORMAT		-1	/*无法格式化*/
#define OTA_FS_STATE_OK			0	/*正常*/
#define OTA_FS_STATE_FORMATED	1	/*重新格式化*/

int ota_fs_mount(void);
int ota_fs_format(void);
int ota_fs_unmount(void);
int ota_fs_usage(void);
int ota_fs_freespace(void);
int ota_fs_chipname(char *name);


int ota_fmount(void);
int ota_funmount(void);
int ota_fformat(void);
int ota_fopen(OTA_FIL* fp, const char* path, unsigned int mode);
int ota_fclose(OTA_FIL* fp);
int ota_fread(OTA_FIL* fp, void* buff, unsigned int btr, unsigned int* br);
int ota_fwrite(OTA_FIL* fp, const void* buff, unsigned int btw, unsigned int* bw);
int ota_fsize(OTA_FIL* fp);
int ota_flseek(OTA_FIL* fp, int ofs);
int ota_ftruncate(OTA_FIL* fp);
int ota_fsync(OTA_FIL* fp);
int ota_fopendir(OTA_DIR* dp, const char* path);
int ota_fclosedir(OTA_DIR* dp);
int ota_freaddir(OTA_DIR* dp, OTA_FILINFO* fno);
int ota_frewinddir(OTA_DIR* dp);
int ota_fnextdir(OTA_DIR* dp);
int ota_fmkdir(const char* path);
int ota_funlink(const char* path);
int ota_frename(const char* path_old, const char* path_new);
int ota_fstat(const char* path, OTA_FILINFO* fno);
bool ota_fexist(const char* path);


#ifdef __cplusplus
}
#endif
#endif

