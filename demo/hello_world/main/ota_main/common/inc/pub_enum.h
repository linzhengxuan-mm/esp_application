/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __PUB_ENUM_H__
#define __PUB_ENUM_H__


#ifdef __cplusplus
extern "C" {
#endif

typedef int ota_err_t;

/* Definitions for error constants. */
#define OTA_OK          				0       /*!< ota_err_t value indicating success (no error) */
#define OTA_FAIL        				-1      /*!< Generic ota_err_t code indicating failure */


#define OTA_TRUE          				1       /*!< Generic ota_err_t code indicating (true) */
#define OTA_FALSE          				0       /*!< Generic ota_err_t code indicating (false) */

#define OTA_ERR_NONE          			0x000   /*!< no error */
#define OTA_ERR_UNKNOW          		0x100   /*!< unkown error */
#define OTA_ERR_NO_MEM              	0x101   /*!< Out of memory */
#define OTA_ERR_INVALID_ARG         	0x102   /*!< Invalid argument */
#define OTA_ERR_INVALID_STATE       	0x103   /*!< Invalid state */
#define OTA_ERR_INVALID_SIZE        	0x104   /*!< Invalid size */
#define OTA_ERR_NOT_FOUND           	0x105   /*!< Requested resource not found */
#define OTA_ERR_NOT_SUPPORTED       	0x106   /*!< Operation or feature not supported */
#define OTA_ERR_TIMEOUT             	0x107   /*!< Operation timed out */
#define OTA_ERR_INVALID_RESPONSE    	0x108   /*!< Received response was invalid */
#define OTA_ERR_INVALID_CRC         	0x109   /*!< CRC or checksum was invalid */
#define OTA_ERR_INVALID_VERSION     	0x10A   /*!< Version was invalid */
#define OTA_ERR_INVALID_MAC         	0x10B   /*!< MAC address was invalid */
#define OTA_ERR_NOT_FINISHED        	0x10C   /*!< There are items remained to retrieve */

#define OTA_ERR_LOAD_OTA_FILE			0x10D   /*未加载OTA文件*/
#define OTA_ERR_INGORED_CASE			0x10E   /*忽略了的case, 未处理的case*/
#define OTA_ERR_DEFAULT_CASE     		0x10F 	/*默认的case处理*/
#define OTA_ERR_UNDEFINED_FUNC    		0x110  	/*未定义的函数, 处理方法*/
#define OTA_ERR_DEFAULT_ENTRY_PARAM 	0x111   /*默认入参为空*/
#define OTA_ERR_ENTRY_DEP_IS_NULL 		0x112 	/*默认入参的dep元素为空*/
#define OTA_ERR_HOST_GIVEN_NULL_BUFF	0x113   /*HOST提供的buffer为空, 说明这个buffer应该由HOST提供而不是OTA自动创建*/
#define OTA_ERR_HOST_FUNC_SET       	0x114 	/*未传入本地函数集合*/
#define OTA_ERR_INVALID_SOCKET_ID   	0x115 	/*socket ID无效*/
#define OTA_ERR_FOUND_SOCKET_ID     	0x116 	/*socket ID未找到*/



#ifdef __cplusplus
}
#endif

#endif
