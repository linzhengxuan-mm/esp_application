set(srcs "hello_world_main.c")
set(include_dirs ".")

list(APPEND srcs "console_cmd/app_console_cmd.c")
list(APPEND include_dirs "console_cmd")

list(APPEND srcs "event_center/app_events.c")
list(APPEND include_dirs "event_center")

list(APPEND srcs "peripherals/board.c")
list(APPEND include_dirs "peripherals")

list(APPEND srcs "fat/fat.c")
list(APPEND include_dirs "fat")

list(APPEND srcs "network/network.c")
list(APPEND include_dirs "network")

list(APPEND srcs "ota_main/port/src/ota_log.c")
list(APPEND srcs "ota_main/port/src/ota_mem.c")
list(APPEND srcs "ota_main/port/src/ota_timer.c")
list(APPEND srcs "ota_main/port/src/ota_fs.c")
list(APPEND srcs "ota_main/port/src/ota_system.c")
list(APPEND srcs "ota_main/port/src/ota_modem.c")
list(APPEND include_dirs "ota_main/port/inc")

list(APPEND srcs "ota_main/common/src/vsnprintf.c")
list(APPEND srcs "ota_main/common/src/sscanf.c")
list(APPEND include_dirs "ota_main/common/inc")

list(APPEND srcs "ota_main/ota/src/ota_timer_handler.c")
list(APPEND srcs "ota_main/ota/src/ota_msg.c")
list(APPEND srcs "ota_main/ota/src/ota_main.c")
list(APPEND srcs "ota_main/ota/src/ota_main_thread.c")
list(APPEND include_dirs "ota_main/ota/inc")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "${include_dirs}")
