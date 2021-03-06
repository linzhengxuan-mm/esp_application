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
#ifndef __FAT_H__
#define __FAT_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MOUNT_PATH "/data"

int fat_init(void);
void fat_calc_magic_number(bool is_true);
int fat_mount(void);
int fat_unmount(void);
int fat_format(void);
int fat_get_usage(size_t* total_bytes, size_t* free_bytes);
#ifdef __cplusplus
}
#endif
#endif

