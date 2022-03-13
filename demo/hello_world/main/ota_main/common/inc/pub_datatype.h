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
#ifndef __PUB_DATATYPE_H__
#define __PUB_DATATYPE_H__

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>


#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Type Definitions
 *******************************************************************************/
 
/* portable character for multichar character set */
typedef char                    CHAR;
/* portable wide character for unicode character set */
typedef /*unsigned short*/CHAR          WCHAR;//DBG BY ZRF

/* portable 8-bit unsigned integer */
typedef unsigned char           INT8U;
/* portable 8-bit signed integer */
typedef signed char             INT8S;
/* portable 16-bit unsigned integer */
typedef unsigned short int      INT16U;
/* portable 16-bit signed integer */
typedef signed short int        INT16S;
/* portable 32-bit unsigned integer */
typedef unsigned int            INT32U;
/* portable 32-bit signed integer */
typedef signed int              INT32S;

/* portable 64-bit unsigned integer */
typedef unsigned long long      INT64U;
/* portable 64-bit signed integer */
typedef signed long long        INT64S;

typedef unsigned int            MOD_TYPE;
typedef unsigned int            SAP_TYPE; /* Service Access Pointer Identifier. */
typedef unsigned int            MSG_TYPE; /* Message identifier */

typedef unsigned short     DCL_DEV;
typedef unsigned char      DCL_UINT8;
typedef unsigned short     DCL_UINT16;
typedef unsigned long      DCL_UINT32;
typedef int 			   inv_error_t;
typedef unsigned int 	   size_t;

typedef	signed short int s16;/**< used for signed 16bit */
typedef	signed int s32;/**< used for signed 32bit */

/*unsigned integer types*/
typedef	unsigned char u8;/**< used for unsigned 8bit */
typedef	unsigned short int u16;/**< used for unsigned 16bit */
typedef	unsigned int u32;/**< used for unsigned 32bit */

/* boolean representation */
typedef enum 
{
    /* FALSE value */
    _FALSE,
    /* TRUE value */
    _TRUE
} _BOOL;


#ifdef __cplusplus
}
#endif
#endif
